//
// Created by stepo on 1/22/18.
//
#include "Moduletest.hpp"

Moduletest::Moduletest() : testcase_timer_(io_) {
	timeout_ = boost::posix_time::seconds(10);
}

Moduletest::~Moduletest() {
	try {
		NODELET_WARN(std::string("[" + name_ + "] killing " + std::to_string(threadpool_.size()) + " threads").c_str());

		threadpool_.interrupt_all();
		threadpool_.join_all();
	} catch (std::exception &ex) {
		NODELET_ERROR(std::string("[" + name_ + "] threw " + ex.what()).c_str());
	}

	expects_timeout_ = false;

}

void Moduletest::register_testcases(boost::function<void()> test_case_fun) {
	testcases_to_run_.emplace_back(test_case_fun);
}

void Moduletest::register_timeout_callback( boost::function<void()> cb, bool is_expected ) {
	timeout_callback_ = std::move(cb);
	expects_timeout_ = true;
}

void Moduletest::finalize_test(TestResult result) {

	try {
		testcase_timer_.cancel();

		std::stringstream ss;
		std::ofstream of;
		of.open(test_result_filepath_, std::ios::app);

		std::time_t t = std::time(nullptr);
		std::put_time(std::localtime(&t), "%c %Z");
		ss << "[" << name_ << "]["
		   << current_test_ << "]["
		   << (result.success ? "SUCCESS" : "FAILURE") << "] "
		   << result.comment << std::endl;

		if (!result.success) {
			NODELET_ERROR(ss.str().c_str());
		} else {
			NODELET_WARN(ss.str().c_str());
		}

		of << "[" << t << "]" << ss.str();

		of.close();

		expects_timeout_ = false;

		start_tests();
	} catch (std::exception &ex) {
		NODELET_FATAL((std::string("[" + name_ + "] threw ") + ex.what()).c_str());
	}
}

void Moduletest::hndl_testcase_timeout(const boost::system::error_code &ec) {

	try {
		if (ec == boost::asio::error::operation_aborted) {
			return;
		}

		if (ec == boost::asio::error::timed_out && timeout_callback_ == nullptr ) {
			TestResult res;
			res.success = false;
			res.comment = "testcase timeout";

			finalize_test(res);
		}

		if( ec == boost::asio::error::timed_out && timeout_callback_ != nullptr ) {

			timeout_callback_();

			TestResult res;
			res.success = true;
			res.comment = "expected testcase timeout";

			finalize_test(res);
		}

	} catch (std::exception &ex) {
		NODELET_FATAL((std::string("[" + name_ + "] threw ") + ex.what()).c_str());
	}

}

void Moduletest::start_tests() {

	try {
		if (testcases_to_run_.empty()) {
			ros::shutdown();
			return;
		}

		boost::function<void()> test_case_fun = testcases_to_run_.front();
		testcases_to_run_.pop_front();

		threadpool_.create_thread([test_case_fun] {
			test_case_fun();
		});

		testcase_timer_.expires_from_now(timeout_);
		testcase_timer_.async_wait(boost::bind(&Moduletest::hndl_testcase_timeout, this,
											   boost::asio::placeholders::error));

		threadpool_.create_thread([this] {
			this->io_.run();
		});
	} catch (std::exception &ex) {
		NODELET_FATAL((std::string("[" + name_ + "] threw ") + ex.what()).c_str());
	}
}
void Moduletest::set_current_test(std::string str) {
	current_test_ = std::move(str);
	NODELET_WARN( std::string( "[" + name_ + "][" + current_test_ + "] started").c_str());
}

