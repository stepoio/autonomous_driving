//
// Created by stepo on 3/4/18.
//

#include <platooning_sim/topics.hpp>
#include "platooning_sim/gazeboadapter.hpp"

namespace platooning_sim {

gazeboadapter::gazeboadapter() :
	io_worker_(io_service_), gazupdate_send_timer_(io_service_), detect_dead_datasource_timer(io_service_) {

	thread_pool_.create_thread([this] {
		try {
			io_service_.run();
		} catch ( std::exception& ex ) {
			NODELET_ERROR("[%s] creating ioservice/work in thread failed", name_.c_str());
			ros::shutdown();
		}
	});

}

gazeboadapter::~gazeboadapter() {

}

void gazeboadapter::onInit() {

	p1gazupdate.id = 1;
	p1gazupdate.speed = 0;
	p1gazupdate.distance = 0;

	p2gazupdate.id = 2;
	p2gazupdate.speed = 0;
	p2gazupdate.distance = 0;

	p3gazupdate.id = 3;
	p3gazupdate.speed = 0;
	p3gazupdate.distance = 0;

	pub_p1_control_ = nh_.advertise<prius_msgs::Control>(topics::p1_control, 1);
	pub_p2_control_ = nh_.advertise<prius_msgs::Control>(topics::p2_control, 1);
	pub_p3_control_ = nh_.advertise<prius_msgs::Control>(topics::p3_control, 1);

	sub_p1_speed_ = nh_.subscribe(topics::p1_speed,1,&gazeboadapter::hndl_p1_speed, this);
	sub_p2_speed_ = nh_.subscribe(topics::p2_speed,1,&gazeboadapter::hndl_p2_speed, this);
	sub_p3_speed_ = nh_.subscribe(topics::p3_speed,1,&gazeboadapter::hndl_p3_speed, this);

	sub_p1_sonar_front_ = nh_.subscribe(topics::p1_front_sonar_left_middle_range,1,&gazeboadapter::hndl_p1_sonar,this);
	sub_p2_sonar_front_ = nh_.subscribe(topics::p2_front_sonar_left_middle_range,1,&gazeboadapter::hndl_p2_sonar,this);
	sub_p3_sonar_front_ = nh_.subscribe(topics::p3_front_sonar_left_middle_range,1,&gazeboadapter::hndl_p3_sonar,this);

	sub_p1_camera_front_ = nh_.subscribe(topics::p1_front_camera_image_raw,1,&gazeboadapter::hndl_p1_camera,this);
	sub_p2_camera_front_ = nh_.subscribe(topics::p2_front_camera_image_raw,1,&gazeboadapter::hndl_p2_camera,this);
	sub_p3_camera_front_ = nh_.subscribe(topics::p3_front_camera_image_raw,1,&gazeboadapter::hndl_p3_camera,this);

	boost::function<void(boost::shared_ptr<std::pair<std::string, uint32_t>>)>
		cbfun(boost::bind(boost::mem_fn(&gazeboadapter::hndl_recv_udp), this, _1));

	server_ = std::unique_ptr<UdpServer>(
		new UdpServer(
			cbfun, udp::endpoint(udp::v4(), 13000), udp::endpoint(boost::asio::ip::address_v4::broadcast(), 13001)));
	server_->set_filter_own_broadcasts(false);

	gazupdate_send_timer_.expires_from_now(BROADCAST_FREQ);
	gazupdate_send_timer_.async_wait(boost::bind(&gazeboadapter::send_gazupdate, this,
	                                             boost::asio::placeholders::error));

	detect_dead_datasource_timer.expires_from_now(SOURCECHECK_FREQ);
	detect_dead_datasource_timer.async_wait(boost::bind(&gazeboadapter::check_dead_datasrc, this,
	                                             boost::asio::placeholders::error));

	//start timers
	ROS_WARN("init done");
}

void gazeboadapter::hndl_recv_udp(boost::shared_ptr<std::pair<std::string, uint32_t>> packet) {

	platooning::stmupdate msg;

	switch (packet->second) {
		case STMSIM_UPDATE:
			process_stmsim( platooning::MessageTypes::decode_json<platooning::stmupdate>(packet->first));
			break;
		default: break;
	}

}
void gazeboadapter::hndl_p1_sonar(const sensor_msgs::Range &msg) {
	src_flags_ |= P1_RANGE;
	p1gazupdate.distance = msg.range;
}

void gazeboadapter::hndl_p1_camera(const sensor_msgs::Image &msg) {

}

void gazeboadapter::hndl_p1_speed(const prius_msgs::Speed &msg) {
	src_flags_ |= P1_SPEED;
	p1gazupdate.speed = msg.speed;
}

void gazeboadapter::hndl_p2_sonar(const sensor_msgs::Range &msg) {
	src_flags_ |= P2_RANGE;
	p2gazupdate.distance = msg.range;
}

void gazeboadapter::hndl_p2_camera(const sensor_msgs::Image &msg) {

}

void gazeboadapter::hndl_p2_speed(const prius_msgs::Speed &msg) {
	src_flags_ |= P2_SPEED;
	p2gazupdate.speed = msg.speed;
}

void gazeboadapter::hndl_p3_sonar(const sensor_msgs::Range &msg) {
	src_flags_ |= P3_RANGE;
	p3gazupdate.distance = msg.range;
}

void gazeboadapter::hndl_p3_camera(const sensor_msgs::Image &msg) {

}

void gazeboadapter::hndl_p3_speed(const prius_msgs::Speed &msg) {
	src_flags_ |= P3_SPEED;
	p3gazupdate.speed = msg.speed;
}

void gazeboadapter::send_gazupdate(const boost::system::error_code &e) {

	if (boost::asio::error::operation_aborted == e) {
		return;
	}

    //std::cout << "id " << p1gazupdate.id << " speed " << p1gazupdate.speed << " dist " << p1gazupdate.distance << std::endl;
	//std::cout << "id " << p2gazupdate.id << " speed " << p2gazupdate.speed << " dist " << p2gazupdate.distance << std::endl;


	std::string msg = platooning::MessageTypes::encode_message(p1gazupdate);
	server_->start_send( msg, GAZ_UPDATE );

	msg = platooning::MessageTypes::encode_message(p2gazupdate);
	server_->start_send( msg, GAZ_UPDATE );

	msg = platooning::MessageTypes::encode_message(p3gazupdate);
	server_->start_send( msg, GAZ_UPDATE );

	gazupdate_send_timer_.expires_from_now(BROADCAST_FREQ);
	gazupdate_send_timer_.async_wait(boost::bind(&gazeboadapter::send_gazupdate, this,
	                                            boost::asio::placeholders::error));
}

void gazeboadapter::process_stmsim(const platooning::stmupdate &stmupdate) {

	auto c = boost::shared_ptr<prius_msgs::Control>( new prius_msgs::Control);

	if( stmupdate.acceleration < 0 ) {
		c->shift_gears = prius_msgs::Control::REVERSE;
	} else if( stmupdate.acceleration == 0) {
		c->shift_gears = prius_msgs::Control::NEUTRAL;
		c->throttle = 0;
	} else {
		c->shift_gears = prius_msgs::Control::FORWARD;
	}
	//maybe between +-0.87? handwheelhigh +-7.85?
	c->steer = stmupdate.steeringAngle;

	//assumes accel between -1 and 1
	//todo use brake instead of accell
	if( stmupdate.acceleration < 0 ) {
		c-> throttle = stmupdate.acceleration * -1;
		c-> brake = stmupdate.acceleration * -1;
	} else if( stmupdate.acceleration == 0 ) {
		c->throttle = 0;
	} else if( stmupdate.acceleration > 0) {
		c->throttle = stmupdate.acceleration;
	}

	if( stmupdate.id == 3 ) {
		pub_p3_control_.publish(c);
	}

	if( stmupdate.id == 2 ) {
		pub_p2_control_.publish(c);
	}

	if( stmupdate.id == 1 ) {
		std::cout << "id:" << stmupdate.id << " gear:" << (int)c->shift_gears
		          << " throttle:" << c->throttle
		          << " brake:" << c->brake << " speed " << p1gazupdate.speed << std::endl;
		pub_p1_control_.publish(c);
	}
}
void gazeboadapter::check_dead_datasrc(const boost::system::error_code &e) {

	if (boost::asio::error::operation_aborted == e) {
		NODELET_ERROR("[%s] check_dead_datasrc timer cancelled", name_.c_str());
		return;
	}

	if( (src_flags_ & P1_RANGE) != P1_RANGE ) {
		NODELET_ERROR("[%s] P1_RANGE not updated", name_.c_str());
	}

	if( (src_flags_ & P2_RANGE) != P2_RANGE ) {
		NODELET_ERROR("[%s] P2_RANGE not updated", name_.c_str());
	}

	if( (src_flags_ & P3_RANGE) != P3_RANGE ) {
		NODELET_ERROR("[%s] P3_RANGE not updated", name_.c_str());
	}

	if( (src_flags_ & P1_SPEED) != P1_SPEED ) {
		NODELET_ERROR("[%s] P1_SPEED not updated", name_.c_str());
	}

	if( (src_flags_ & P2_SPEED) != P2_SPEED ) {
		NODELET_ERROR("[%s] P2_SPEED not updated", name_.c_str());
	}

	if( (src_flags_ & P3_SPEED) != P3_SPEED ) {
		NODELET_ERROR("[%s] P3_SPEED not updated", name_.c_str());
	}

	src_flags_ = 0;

	detect_dead_datasource_timer.expires_from_now(SOURCECHECK_FREQ);
	detect_dead_datasource_timer.async_wait(boost::bind(&gazeboadapter::check_dead_datasrc, this,
	                                                    boost::asio::placeholders::error));

}

}

PLUGINLIB_EXPORT_CLASS(platooning_sim::gazeboadapter, nodelet::Nodelet);
// %EndTag(FULLTEXT)%