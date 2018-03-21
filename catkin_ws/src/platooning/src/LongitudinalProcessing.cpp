//
// Created by stepo on 12/16/17.
//


/**
 * @file /platooning/src/platooning.cpp
 *
 * @brief Nodelet implementation of RemoteContol
 *
 * @author stepo
 **/

/*****************************************************************************
** Includes
*****************************************************************************/
// %Tag(FULLTEXT)%
#include "platooning/LongitudinalProcessing.hpp"

namespace platooning {


/*****************************************************************************
** Constructors
*****************************************************************************/


/**
 * @brief Template Nodelet
 */

LongitudinalProcessing::LongitudinalProcessing()
	: io_worker_(io_service_), detect_dead_datasource_timer(io_service_) {

	thread_pool_.create_thread([this] { io_service_.run(); });

};

/*****************************************************************************
** Destructors
*****************************************************************************/

LongitudinalProcessing::~LongitudinalProcessing() {};


/*****************************************************************************
** Initializers
*****************************************************************************/

/**
* Set-up necessary publishers/subscribers
*/
void LongitudinalProcessing::onInit() {

	sub_current_speed_ = nh_.subscribe(topics::CURRENT_SPEED, 1,
	                                   &LongitudinalProcessing::hndl_current_velocity, this);

	sub_target_speed_ = nh_.subscribe(topics::TARGET_SPEED, 1,
	                                  &LongitudinalProcessing::hndl_targetSpeed, this);

	sub_distance_to_obj_ = nh_.subscribe(topics::SENSOR_DISTANCE_TO_OBJ, 1,
	                                     &LongitudinalProcessing::hndl_distance_from_sensor, this);

	sub_target_distance_ = nh_.subscribe(topics::TARGET_DISTANCE, 1,
	                                     &LongitudinalProcessing::hndl_target_distance, this);

	pub_acceleration_ = nh_.advertise<platooning::acceleration>(topics::ACCELERATION, 1);

	previous_distance_timestamp_ = boost::posix_time::min_date_time;
	current_distance_timestamp_ = boost::posix_time::min_date_time;

	current_distance_ = 0;
	current_velocity_ = 0;
	target_velocity_ = 0;

	NODELET_INFO("[%s] init done", name_.c_str());

}

/*****************************************************************************
** Handlers
*****************************************************************************/

void LongitudinalProcessing::hndl_distance_from_sensor(const platooning::distance &msg) {
	//NODELET_INFO(  "[%s]  recv distance from sensor.", name_.c_str());


	detect_dead_datasource_timer.cancel();
	detect_dead_datasource_timer.expires_from_now(SOURCECHECK_FREQ);
	detect_dead_datasource_timer.async_wait(boost::bind(&LongitudinalProcessing::check_dead_datasrc, this,
	                                                    boost::asio::placeholders::error));

	try {
		//check if we received new data
		previous_distance_ = current_distance_;
		current_distance_ = msg.distance;

		previous_distance_timestamp_ = current_distance_timestamp_;
		current_distance_timestamp_ = boost::posix_time::microsec_clock::local_time();

		if (previous_distance_timestamp_ == boost::posix_time::min_date_time) {
			return;
		}

		update_velocity();
	} catch (std::exception &ex) {
		NODELET_ERROR("[%s] hndl_distance_from_sensor crash with %s", name_.c_str(), ex.what());
	}
}

void LongitudinalProcessing::hndl_target_distance(const platooning::targetDistance &msg) {
	//NODELET_INFO( "[%s] recv targetDistance.", name_.c_str());
	try {
		if (msg.distance != -spring_.get_target_position()) {


			if (msg.distance < 0.5) {
				NODELET_ERROR("[%s] target distance shorter than 0.5. Setting to 1.", name_.c_str());
				spring_.set_target_position(-1);

			} else {
				spring_.set_target_position(-msg.distance);
			}
		}
	} catch (std::exception &ex) {
		NODELET_ERROR("[%s] hndl_target_distance crash with %s", name_.c_str(), ex.what());
	}
};

void LongitudinalProcessing::hndl_current_velocity(const platooning::speed &msg) {

	//NODELET_INFO( "[%s] recv speed.", name_.c_str());
	try {
		if (msg.speed != current_velocity_) {
			current_velocity_ = msg.speed;
		}
	} catch (std::exception &ex) {
		NODELET_ERROR("[%s] hndl_current_velocity crash with %s", name_.c_str(), ex.what());
	}
}

void LongitudinalProcessing::hndl_targetSpeed(const platooning::targetSpeed &msg) {
	//NODELET_INFO( "[%s] recv targetSpeed.", name_.c_str());
	try {
		if (msg.target_speed != target_velocity_) {
			target_velocity_ = msg.target_speed;
		}
	} catch (std::exception &ex) {
		NODELET_ERROR("[%s] hndl_targetSpeed crash with %s", name_.c_str(), ex.what());
	}
}

void LongitudinalProcessing::update_velocity() {

	//wait for second ranging
	if (previous_distance_timestamp_ == boost::posix_time::min_date_time) {
		std::cout << "wait for second ranging" << std::endl;
		return;
	}

	boost::mutex::scoped_lock l(calc_mutex_);

	auto outmsg = boost::shared_ptr<platooning::acceleration>(new platooning::acceleration);

	//calculate time_step in seconds since velocity is in m/s and distance in m
	float time_step = (current_distance_timestamp_ - previous_distance_timestamp_).total_milliseconds() / 1000.0f;

	//distance change in the last time_step
	float range_diff = current_distance_ - previous_distance_;
	//change in range / timestep should be the relative velocity
	float relative_velocity = range_diff / time_step;
	//add current velocity to
	float spring_velocity =
		current_velocity_ + spring_.calulate_velocity(-current_distance_, relative_velocity, time_step);

	//modify current velocity with spring value to approach target distance
	float calculated_velocity = current_velocity_ + spring_velocity;

	float accel = std::max(-5.0f, std::min(calculated_velocity, target_velocity_ * 1.4f)) - current_velocity_;

	gazsim brake weglassen? hoffen dass reverse throttle unser brake wird? ipd auf 5 momentan?? crashes nun nur longcontrol?

	NODELET_INFO(
		"ms: %f time_step: %f range_diff: %f relative_vel: %f spring_vel %f current_vel: %f calc_vel: %f\ndist:%f target:%f accel: %f",
		(float) (current_distance_timestamp_ - previous_distance_timestamp_).total_milliseconds(),
		time_step,
		range_diff,
		relative_velocity,
		spring_velocity,
		current_velocity_,
		calculated_velocity,
		current_distance_,
		-spring_.get_target_position(),
		accel);

	//clamp calculated velocity to -5 and target_speed *1.4 so we dont drive so fast
	outmsg->accelleration = accel;

	pub_acceleration_.publish(outmsg);

}
void LongitudinalProcessing::check_dead_datasrc(const boost::system::error_code &e) {

	if (boost::asio::error::operation_aborted == e) {
		//NODELET_ERROR("[%s] check_dead_datasrc timer cancelled", name_.c_str());
		return;
	}

	NODELET_ERROR("[%s] distance data source hasnt delivered in %i ms. halting",
	              name_.c_str(),
	              (int) SOURCECHECK_FREQ.total_milliseconds());
	auto outmsg = boost::shared_ptr<platooning::acceleration>(new platooning::acceleration);
	outmsg->accelleration = 0;
	pub_acceleration_.publish(outmsg);


//is always started on every range receipt
//detect_dead_datasource_timer->expires_from_now(SOURCECHECK_FREQ);
//detect_dead_datasource_timer->async_wait(boost::bind(&LongitudinalProcessing::check_dead_datasrc, this,
//                                                     boost::asio::placeholders::error));

}

LongitudinalProcessing::CritiallyDampenedSpring::CritiallyDampenedSpring() {
	spring_constant_ = 3;
	target_relative_position_ = -1;

}

//https://stackoverflow.com/questions/5100811/algorithm-to-control-acceleration-until-a-position-is-reached#
//https://en.wikipedia.org/wiki/PID_controller
//doesnt work. will always trail way behind where it should be while moving: https://jsfiddle.net/BfLAh/3175/
float LongitudinalProcessing::CritiallyDampenedSpring::calulate_velocity(const Distance &current_position,
                                                                         const Velocity &relative_velocity,
                                                                         const float &time_step) {

	float current_to_target = target_relative_position_ - current_position;
	float spring_force = current_to_target * spring_constant_;
	float damping_force = -relative_velocity * 2 * sqrt(spring_constant_);
	float force = spring_force + damping_force;
	float new_velocity = relative_velocity + force * time_step_;
	//float displacement = new_velocity * time_step_;
	//float predicted_distance = current_position - displacement;
	return new_velocity;
}
} // namespace platooning

PLUGINLIB_EXPORT_CLASS(platooning::LongitudinalProcessing, nodelet::Nodelet);
// %EndTag(FULLTEXT)%
