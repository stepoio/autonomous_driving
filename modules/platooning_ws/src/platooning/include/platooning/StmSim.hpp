//
// Created by stepo on 12/16/17.
//

/**
 * @file doxygen_c.h
 * @author My Self
 * @date 9 Sep 2012
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef PLATOONING_STMSIM_HPP
#define PLATOONING_STMSIM_HPP

/*****************************************************************************
** Includes
*****************************************************************************/

#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>
#include <ros/ros.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <algorithm>

#include "Topics.hpp"
#include "MessageTypes.hpp"
#include "UdpServer.hpp"
#include "ServiceTypes.hpp"
#include "Services.hpp"

namespace platooning {


/**
 * @brief Example showing how to document a function with Doxygen.
 *
 * Description of what the function does. This part may refer to the parameters
 * of the function, like @p param1 or @p param2. A word of code can also be
 * inserted like @c this which is equivalent to <tt>this</tt> and can be useful
 * to say that the function returns a @c void or an @c int. If you want to have
 * more than one word in typewriter font, then just use @<tt@>.
 * We can also include text verbatim,
 * @verbatim like this@endverbatim
 * Sometimes it is also convenient to include an example of usage:
 * @code
 * BoxStruct *out = Box_The_Function_Name(param1, param2);
 * printf("something...\n");
 * @endcode
 * Or,
 * @code{.py}
 * pyval = python_func(arg1, arg2)
 * print pyval
 * @endcode
 * when the language is not the one used in the current source file (but
 * <b>be careful</b> as this may be supported only by recent versions
 * of Doxygen). By the way, <b>this is how you write bold text</b> or,
 * if it is just one word, then you can just do @b this.
 * @param param1 Description of the first parameter of the function.
 * @param param2 The second one, which follows @p param1.
 * @return Describe what the function returns.
 * @see Box_The_Second_Function
 * @see Box_The_Last_One
 * @see http://website/
 * @note Something to note.
 * @warning Warning.
 */

class StmSim : public nodelet::Nodelet {
  public:
	void onInit();

	StmSim();

	~StmSim();

  private:
	ros::NodeHandle nh_; /**< Some documentation for the member nh_. */
	std::string name_ = "StmSim";
	std::unique_ptr<UdpServer> server_ptr_;
	boost::thread_group thread_pool_;

	ros::Subscriber sub_vehicle_control_;

	ros::Publisher pub_current_speed_;
	ros::Publisher pub_camera_;
	ros::Publisher pub_distanceToObj_;

	float current_velocity_ = 0;
	float steering_angle_ = 0;
	uint32_t vehicle_id_ = 1;

	/**
	 * @brief to achieve X does Y
	 * @param msg incoming topic message
	 */
	void hndl_gazupdate(boost::shared_ptr<std::pair<std::string, uint32_t>> message_pair);

	void hndl_vehicleControl(const vehicleControl &);


};


} // namespace platooning

#endif //PLATOONING_STMSIM_HPP
