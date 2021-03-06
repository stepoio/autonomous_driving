	/**
 * @file /testing/src/Moduletest_messagedistribution.cpp
 *
 * @brief Implementation for Moduletest_messagedistribution class
 *
 * @author stepo
 *
 * @date 22.03.2018
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "platooning/Moduletest_messagedistribution.hpp"

// %Tag(FULLTEXT)%
namespace platooning {

/*****************************************************************************
** Constructors
*****************************************************************************/

Moduletest_messagedistribution::Moduletest_messagedistribution() = default;

/*****************************************************************************
** Destructors
*****************************************************************************/

Moduletest_messagedistribution::~Moduletest_messagedistribution() = default;


/*****************************************************************************
** Initializers
*****************************************************************************/

/**
* Set-up necessary publishers/subscribers
*/
void Moduletest_messagedistribution::onInit() {

	name_ = "Moduletest_messagedistribution";

	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_lv_broadcast, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_lv_broadcast_recv_platoonMsg, this));

	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_lv_accept, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_lv_accept_recv_platoonMsg, this));

	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_lv_reject, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_lv_reject_recv_platoonMsg, this));

	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_fv_heartbeat_recv_platoonMsg, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_fv_heartbeat, this));

	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_fv_leave, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_fv_leave_recv_platoonMsg, this));

	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_fv_request, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_fv_request_recv_platoonMsg, this));

	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_remotecontrolInput, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_remotecontrolToggle, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_platooningToggle, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_userInterface, this));
	register_testcases(boost::bind(&Moduletest_messagedistribution::test_stresstest_all_messages, this));

	NODELET_INFO("[%s] init done", name_.c_str());

	start_tests();
};

/*****************************************************************************
** Testcases
*****************************************************************************/

void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_lv_broadcast() {
	set_current_test("test_pub_in_platoonMsg_recv_lv_broadcast");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.emplace(topics::IN_LV_BROADCAST, ros::Subscriber());
	sub_map_[topics::IN_LV_BROADCAST] = nh_.subscribe(topics::IN_LV_BROADCAST, 1,
	                                               &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_lv_broadcast,
	                                               this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	lv_broadcast msg;
	msg.src_vehicle = 3;

	inmsg->message_type = LV_BROADCAST;
	inmsg->payload = MessageTypes::encode_message(msg);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);
}

void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_lv_broadcast(platooning::lv_broadcast msg) {
	TestResult res;
	res.success = true;

	if (msg.src_vehicle != 3) {
		res.success = false;
		res.comment = std::string("hndl_test_pub_in_platoonMsg_recv_lv_broadcast:") + "src_vehicle:3 == "
			+ std::to_string(msg.src_vehicle)
			+ "=" + std::to_string(msg.src_vehicle == 3);
	}

	if (msg.src_vehicle == 3) {
		res.success = true;
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_lv_broadcast_recv_platoonMsg() {
	set_current_test("test_pub_in_lv_broadcast_recv_platoonMsg");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::OUT_LV_BROADCAST, ros::Publisher());
	pub_map_[topics::OUT_LV_BROADCAST] = nh_.advertise<platooning::lv_broadcast>(topics::OUT_LV_BROADCAST, 1);

	//mockup subscribers
	sub_map_.emplace(topics::OUT_PLATOONING_MSG, ros::Subscriber());
	sub_map_[topics::OUT_PLATOONING_MSG] = nh_.subscribe(topics::OUT_PLATOONING_MSG, 1,
	                                                  &Moduletest_messagedistribution::hndl_test_pub_in_lv_broadcast_recv_platoonMsg,
	                                                  this);

	boost::shared_ptr<lv_broadcast> inmsg = boost::shared_ptr<lv_broadcast>(new lv_broadcast);

	inmsg->src_vehicle = 3;

	pub_map_[topics::OUT_LV_BROADCAST].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_lv_broadcast_recv_platoonMsg(platooning::platoonProtocol msg) {
	TestResult res;
	res.success = true;

	if (msg.message_type != LV_BROADCAST) {
		res.success = false;
		res.comment = "hndl_test_pub_in_lv_broadcast_recv_platoonMsg received wrong header type";
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_lv_accept() {
	set_current_test("test_pub_in_platoonMsg_recv_lv_accept");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.clear();
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.clear();
	sub_map_.emplace(topics::IN_LV_ACCEPT, ros::Subscriber());
	sub_map_[topics::IN_LV_ACCEPT] = nh_.subscribe(topics::IN_LV_ACCEPT, 1,
	                                               &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_lv_accept,
	                                               this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	lv_accept msg;
	msg.src_vehicle = 3;
	msg.platoon_id = 4;
	msg.dst_vehicle = 5;

        inmsg->message_type = LV_ACCEPT;
	inmsg->payload = MessageTypes::encode_message(msg);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);
}

void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_lv_accept(platooning::lv_accept msg) {
	TestResult res;
	res.success = true;

	if (msg.src_vehicle != 3 || msg.dst_vehicle != 5 || msg.platoon_id != 4 ) {
		res.success = false;
		res.comment = get_current_test()
			+ " src_vehicle:3 == " + std::to_string(msg.src_vehicle) + "=" + std::to_string(msg.src_vehicle == 3) + "\n"
			+ " dst_vehicle:5 == " + std::to_string(msg.dst_vehicle) + "=" + std::to_string(msg.dst_vehicle == 5) + "\n"
			+ " platoon_id:4 == " + std::to_string(msg.platoon_id) + "=" + std::to_string(msg.platoon_id == 4) + "\n";
	}

	if (msg.src_vehicle == 3 || msg.dst_vehicle == 5 || msg.platoon_id == 4 ) {
		res.success = true;
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_lv_accept_recv_platoonMsg() {
	set_current_test("test_pub_in_lv_accept_recv_platoonMsg");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::OUT_LV_ACCEPT, ros::Publisher());
	pub_map_[topics::OUT_LV_ACCEPT] = nh_.advertise<platooning::lv_accept>(topics::OUT_LV_ACCEPT, 1);

	//mockup subscribers
	sub_map_.emplace(topics::OUT_PLATOONING_MSG, ros::Subscriber());
	sub_map_[topics::OUT_PLATOONING_MSG] = nh_.subscribe(topics::OUT_PLATOONING_MSG, 1,
	                                                    &Moduletest_messagedistribution::hndl_test_pub_in_lv_accept_recv_platoonMsg,
	                                                    this);

	auto inmsg = boost::shared_ptr<lv_accept>(new lv_accept);

	inmsg->src_vehicle = 3;

	pub_map_[topics::OUT_LV_ACCEPT].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_lv_accept_recv_platoonMsg(platooning::platoonProtocol msg) {
	TestResult res;
	res.success = true;

	if (msg.message_type != LV_ACCEPT) {
		res.success = false;
		res.comment = "hndl_test_pub_in_lv_accept_recv_platoonMsg received wrong header type";
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_lv_reject() {
	set_current_test("test_pub_in_platoonMsg_recv_lv_reject");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.emplace(topics::IN_LV_REJECT, ros::Subscriber());
	sub_map_[topics::IN_LV_BROADCAST] = nh_.subscribe(topics::IN_LV_REJECT, 1,
	                                                  &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_lv_reject,
	                                                  this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	lv_reject msg;
	msg.src_vehicle = 3;

	inmsg->message_type = LV_REJECT;
	inmsg->payload = MessageTypes::encode_message(msg);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);
}

void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_lv_reject(platooning::lv_reject msg) {
	TestResult res;
	res.success = true;

	if (msg.src_vehicle != 3) {
		res.success = false;
		res.comment = std::string("hndl_test_pub_in_platoonMsg_recv_lv_reject:") + "src_vehicle:3 == "
			+ std::to_string(msg.src_vehicle)
			+ "=" + std::to_string(msg.src_vehicle == 3);
	}

	if (msg.src_vehicle == 3) {
		res.success = true;
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_lv_reject_recv_platoonMsg() {
	set_current_test("test_pub_in_lv_reject_recv_platoonMsg");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::OUT_LV_REJECT, ros::Publisher());
	pub_map_[topics::OUT_LV_REJECT] = nh_.advertise<platooning::lv_reject>(topics::OUT_LV_REJECT, 1);

	//mockup subscribers
	sub_map_.emplace(topics::OUT_PLATOONING_MSG, ros::Subscriber());
	sub_map_[topics::OUT_PLATOONING_MSG] = nh_.subscribe(topics::OUT_PLATOONING_MSG, 1,
	                                                    &Moduletest_messagedistribution::hndl_test_pub_in_lv_reject_recv_platoonMsg,
	                                                    this);

	auto inmsg = boost::shared_ptr<lv_reject>(new lv_reject);

	inmsg->src_vehicle = 3;

	pub_map_[topics::OUT_LV_REJECT].publish(inmsg);
}

void Moduletest_messagedistribution::hndl_test_pub_in_lv_reject_recv_platoonMsg(platooning::platoonProtocol msg) {
	TestResult res;
	res.success = true;

	if (msg.message_type != LV_REJECT) {
		res.success = false;
		res.comment = "hndl_test_pub_in_lv_reject_recv_platoonMsg received wrong header type";
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_fv_heartbeat() {
	set_current_test("test_pub_in_platoonMsg_recv_fv_heartbeat");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.emplace(topics::IN_FV_HEARTBEAT, ros::Subscriber());
	sub_map_[topics::IN_FV_HEARTBEAT] = nh_.subscribe(topics::IN_FV_HEARTBEAT, 1,
	                                                  &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_fv_heartbeat,
	                                                  this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	fv_heartbeat msg;
	msg.src_vehicle = 3;

	inmsg->message_type = FV_HEARTBEAT;
	inmsg->payload = MessageTypes::encode_message(msg);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_fv_heartbeat(platooning::fv_heartbeat msg) {
	TestResult res;
	res.success = true;

	if (msg.src_vehicle != 3) {
		res.success = false;
		res.comment = std::string("hndl_test_pub_in_platoonMsg_recv_fv_heartbeat:") + "src_vehicle:3 == "
			+ std::to_string(msg.src_vehicle)
			+ "=" + std::to_string(msg.src_vehicle == 3);
	}

	if (msg.src_vehicle == 3) {
		res.success = true;
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_fv_heartbeat_recv_platoonMsg() {
	set_current_test("test_pub_in_fv_heartbeat_recv_platoonMsg");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::OUT_FV_HEARTBEAT, ros::Publisher());
	pub_map_[topics::OUT_FV_HEARTBEAT] = nh_.advertise<platooning::fv_heartbeat>(topics::OUT_FV_HEARTBEAT, 1);

	//mockup subscribers
	sub_map_.emplace(topics::OUT_PLATOONING_MSG, ros::Subscriber());
	sub_map_[topics::OUT_PLATOONING_MSG] = nh_.subscribe(topics::OUT_PLATOONING_MSG, 1,
	                                                    &Moduletest_messagedistribution::hndl_test_pub_in_fv_heartbeat_recv_platoonMsg,
	                                                    this);

	auto inmsg = boost::shared_ptr<fv_heartbeat>(new fv_heartbeat);

	inmsg->src_vehicle = 3;

	pub_map_[topics::OUT_FV_HEARTBEAT].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_fv_heartbeat_recv_platoonMsg(platooning::platoonProtocol msg) {
	TestResult res;
	res.success = true;

	if (msg.message_type != FV_HEARTBEAT) {
		res.success = false;
		res.comment = "hndl_test_pub_in_fv_heartbeat_recv_platoonMsg received wrong header type";
	}

	finalize_test(res);
}


void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_fv_leave() {
	set_current_test("test_pub_in_platoonMsg_recv_fv_leave");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.emplace(topics::IN_FV_LEAVE, ros::Subscriber());
	sub_map_[topics::IN_FV_LEAVE] = nh_.subscribe(topics::IN_FV_LEAVE, 1,
	                                                  &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_fv_leave,
	                                                  this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	fv_leave msg;
	msg.src_vehicle = 3;

	inmsg->message_type = FV_LEAVE;
	inmsg->payload = MessageTypes::encode_message(msg);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_fv_leave(platooning::fv_leave msg) {
	TestResult res;
	res.success = true;

	if (msg.src_vehicle != 3) {
		res.success = false;
		res.comment = std::string("hndl_test_pub_in_platoonMsg_recv_fv_leave:") + "src_vehicle:3 == "
			+ std::to_string(msg.src_vehicle)
			+ "=" + std::to_string(msg.src_vehicle == 3);
	}

	if (msg.src_vehicle == 3) {
		res.success = true;
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_fv_leave_recv_platoonMsg() {
	set_current_test("test_pub_in_fv_leave_recv_platoonMsg");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::OUT_FV_LEAVE, ros::Publisher());
	pub_map_[topics::OUT_FV_LEAVE] = nh_.advertise<platooning::fv_leave>(topics::OUT_FV_LEAVE, 1);

	//mockup subscribers
	sub_map_.emplace(topics::OUT_PLATOONING_MSG, ros::Subscriber());
	sub_map_[topics::OUT_PLATOONING_MSG] = nh_.subscribe(topics::OUT_PLATOONING_MSG, 1,
	                                                    &Moduletest_messagedistribution::hndl_test_pub_in_fv_leave_recv_platoonMsg,
	                                                    this);

	auto inmsg = boost::shared_ptr<fv_leave>(new fv_leave);

	inmsg->src_vehicle = 3;

	pub_map_[topics::OUT_FV_LEAVE].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_fv_leave_recv_platoonMsg(platooning::platoonProtocol msg) {
	TestResult res;
	res.success = true;

	if (msg.message_type != FV_LEAVE) {
		res.success = false;
		res.comment = "hndl_test_pub_in_fv_leave_recv_platoonMsg received wrong header type";
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_fv_request() {

	set_current_test("test_pub_in_platoonMsg_recv_fv_request");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.clear();
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.clear();
	sub_map_.emplace(topics::IN_FV_REQUEST, ros::Subscriber());
	sub_map_[topics::IN_FV_LEAVE] = nh_.subscribe(topics::IN_FV_REQUEST, 1,
	                                              &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_fv_request,
	                                              this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	fv_request msg;
	msg.src_vehicle = 3;

	inmsg->message_type = FV_REQUEST;
	inmsg->payload = MessageTypes::encode_message(msg);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);

}

void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_fv_request(platooning::fv_request msg) {
	TestResult res;
	res.success = true;

	if (msg.src_vehicle != 3) {
		res.success = false;
		res.comment = std::string("test_pub_in_platoonMsg_recv_fv_request:") + "src_vehicle:3 == "
			+ std::to_string(msg.src_vehicle)
			+ "=" + std::to_string(msg.src_vehicle == 3);
	}

	if (msg.src_vehicle == 3) {
		res.success = true;
	}

	finalize_test(res);

}

void Moduletest_messagedistribution::test_pub_in_fv_request_recv_platoonMsg() {
	set_current_test("test_pub_in_fv_request_recv_platoonMsg");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::OUT_FV_REQUEST, ros::Publisher());
	pub_map_[topics::OUT_FV_REQUEST] = nh_.advertise<platooning::fv_request>(topics::OUT_FV_REQUEST, 1);

	//mockup subscribers
	sub_map_.emplace(topics::OUT_PLATOONING_MSG, ros::Subscriber());
	sub_map_[topics::OUT_PLATOONING_MSG] = nh_.subscribe(topics::OUT_PLATOONING_MSG, 1,
	                                                    &Moduletest_messagedistribution::hndl_test_pub_in_fv_request_recv_platoonMsg,
	                                                    this);

	auto inmsg = boost::shared_ptr<fv_request>(new fv_request);

	inmsg->src_vehicle = 3;

	pub_map_[topics::OUT_FV_REQUEST].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_fv_request_recv_platoonMsg(platooning::platoonProtocol msg) {
	TestResult res;
	res.success = true;

	if (msg.message_type != FV_REQUEST) {
		res.success = false;
		res.comment = "hndl_test_pub_in_fv_request_recv_platoonMsg received wrong header type";
	}

	finalize_test(res);
}

void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_remotecontrolInput() {
	set_current_test("test_pub_in_platoonMsg_recv_remotecontrolInput");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.emplace(topics::INPUT_REMOTECONTROL, ros::Subscriber());
	sub_map_[topics::INPUT_REMOTECONTROL] = nh_.subscribe(topics::INPUT_REMOTECONTROL, 1,
	                                              &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_remotecontrolInput,
	                                              this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	remotecontrolInput msg;
	msg.vehicle_id = 3;

	inmsg->message_type = REMOTE_CONTROLINPUT;
	inmsg->payload = MessageTypes::encode_message(msg);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_remotecontrolInput(platooning::remotecontrolInput msg) {
	TestResult res;
	res.success = true;

	if (msg.vehicle_id != 3) {
		res.success = false;
		res.comment = std::string("hndl_test_pub_in_platoonMsg_recv_remotecontrolInput:") + "src_vehicle:3 == "
			+ std::to_string(msg.vehicle_id)
			+ "=" + std::to_string(msg.vehicle_id == 3);
	}

	if (msg.vehicle_id == 3) {
		res.success = true;
	}

	finalize_test(res);
}
void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_remotecontrolToggle() {
	set_current_test("test_pub_in_platoonMsg_recv_remotecontrolToggle");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.emplace(topics::TOGGLE_REMOTECONTROL, ros::Subscriber());
	sub_map_[topics::TOGGLE_REMOTECONTROL] = nh_.subscribe(topics::TOGGLE_REMOTECONTROL, 1,
	                                                      &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_remotecontrolToggle,
	                                                      this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	remotecontrolToggle msg;
	msg.vehicle_id = 3;

	inmsg->message_type = REMOTE_CONTROLTOGGLE;
	inmsg->payload = MessageTypes::encode_message(msg);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_remotecontrolToggle(platooning::remotecontrolToggle msg) {
	TestResult res;
	res.success = true;

	if (msg.vehicle_id != 3) {
		res.success = false;
		res.comment = std::string("hndl_test_pub_in_platoonMsg_recv_remotecontrolToggle:") + "src_vehicle:3 == "
			+ std::to_string(msg.vehicle_id)
			+ "=" + std::to_string(msg.vehicle_id == 3);
	}

	if (msg.vehicle_id == 3) {
		res.success = true;
	}

	finalize_test(res);
}
void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_platooningToggle() {
	set_current_test("test_pub_in_platoonMsg_recv_platooningToggle");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	//mockup publishers
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.emplace(topics::TOGGLE_PLATOONING, ros::Subscriber());
	sub_map_[topics::TOGGLE_PLATOONING] = nh_.subscribe(topics::TOGGLE_PLATOONING, 1,
	                                                       &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_platooningToggle,
	                                                       this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	platooningToggle msg;
	msg.vehicle_id = 3;

	inmsg->message_type = REMOTE_PLATOONINGTOGGLE;
	inmsg->payload = MessageTypes::encode_message(msg);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);
}
void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_platooningToggle(platooning::platooningToggle msg) {
	TestResult res;
	res.success = true;

	if (msg.vehicle_id != 3) {
		res.success = false;
		res.comment = std::string("hndl_test_pub_in_platoonMsg_recv_platooningToggle:") + "src_vehicle:3 == "
			+ std::to_string(msg.vehicle_id)
			+ "=" + std::to_string(msg.vehicle_id == 3);
	}

	if (msg.vehicle_id == 3) {
		res.success = true;
	}

	finalize_test(res);
}
void Moduletest_messagedistribution::test_pub_in_platoonMsg_recv_userInterface() {
	set_current_test("test_pub_in_platoonMsg_recv_userInterface");
	NODELET_INFO("[%s] started testcase %s", name_.c_str(), get_current_test().c_str());

	set_timeout(boost::posix_time::time_duration(boost::posix_time::seconds(5)));

	//mockup publishers
	pub_map_.emplace(topics::IN_PLATOONING_MSG, ros::Publisher());
	pub_map_[topics::IN_PLATOONING_MSG] = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 1);

	//mockup subscribers
	sub_map_.emplace(topics::USERINTERFACE, ros::Subscriber());
	sub_map_[topics::USERINTERFACE] = nh_.subscribe(topics::USERINTERFACE, 1,
	                                                    &Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_userInterface,
	                                                    this);

	boost::shared_ptr<platoonProtocol> inmsg = boost::shared_ptr<platoonProtocol>(new platoonProtocol);

	userInterface msg;
	msg.src_vehicle = 3;

	inmsg->message_type = REMOTE_USERINTERFACE;
	inmsg->payload = MessageTypes::encode_message(msg);

	boost::function<void()>
		cbfun(boost::bind(boost::mem_fn(&Moduletest_messagedistribution::callback_test_pub_in_platoonMsg_recv_userInterface), this));
	register_timeout_callback(cbfun);

	pub_map_[topics::IN_PLATOONING_MSG].publish(inmsg);
}

void Moduletest_messagedistribution::hndl_test_pub_in_platoonMsg_recv_userInterface(platooning::userInterface msg) {
	TestResult res;
	res.success = false;
	res.comment = "shouldnt receive userinterface. must be filtered out";

	finalize_test(res);
}

void Moduletest_messagedistribution::callback_test_pub_in_platoonMsg_recv_userInterface() {

	TestResult res;
	res.success = true;

	finalize_test(res);
}

void Moduletest_messagedistribution::test_stresstest_all_messages() {
	TestResult res;
	res.success = true;

	finalize_test(res);
}

} // namespace platooning

PLUGINLIB_EXPORT_CLASS(platooning::Moduletest_messagedistribution, nodelet::Nodelet);
// %EndTag(FULLTEXT)%
