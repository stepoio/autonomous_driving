//
// Created by stepo on 12/16/17.
//


/**
 * @file /platooning/src/wifi.cpp
 *
 * @brief Implements Listeners and Senders for traffic towards the network
 *
 * @author stepo
 **/

/*****************************************************************************
** Includes
*****************************************************************************/
// %Tag(FULLTEXT)%
#include "Wifi.hpp"

namespace platooning {


/*****************************************************************************
** Constructors
*****************************************************************************/


/**
 * @brief Template Nodelet
 */

  Wifi::Wifi() = default;


/*****************************************************************************
** Destructors
*****************************************************************************/

  Wifi::~Wifi() = default;


/*****************************************************************************
** Initializers
*****************************************************************************/

  /**
  * Set-up necessary publishers/subscribers
  */
  void Wifi::onInit() {

    name_ = "wifi";

    //subscribers of protocol nodelet
    sub_platoonProtocolOut_ = nh_.subscribe("platoonProtocolOut", 10,
                                            &Wifi::hndl_platoonProtocolOut, this);

    pub_platoonProtocolIn_ = nh_.advertise<platoonProtocolIn>("platoonProtocolIn", 10);

    try {
      //bind to local 10000 port, broadcast to 10000 port
      boost::function<void (std::pair<std::string, int32_t>)> cbfun( boost::bind( boost::mem_fn(&Wifi::hndl_wifi_receive), this, _1 ) );

      server_ptr_ = std::unique_ptr<UdpServer>( new UdpServer( cbfun
                                                 , udp::endpoint(udp::v4(),10000)
                                                 , udp::endpoint(ip::address_v4::broadcast(),10000)));
    } catch (std::exception &e) {
      NODELET_FATAL( std::string("[WIFI] udpserver init failed\n" + std::string(e.what())).c_str());
    }

    NODELET_INFO("[WIFI] init done");
  };


/*****************************************************************************
** Handlers
*****************************************************************************/

  /*
   * handling an event and publishing something
   */
  void Wifi::hndl_platoonProtocolOut(platooning::platoonProtocolOut msg) {
    std::cout << "calling server to send" << std::endl;

    server_ptr_->start_send(msg.payload, msg.message_type);
  }

  void Wifi::hndl_wifi_receive(std::pair<std::string, int32_t> message_pair)  {
    std::cout << "handling wifi receive" << std::endl;

    platooning::platoonProtocolIn outmsg;

    switch (message_pair.second) {
      case LV_BROADCAST:
      case FV_HEARTBEAT:
      case LV_REQUEST:
      case FV_REQUEST:
      case ACCEPT_RESPONSE:
      case REJECT_RESPONSE:
      case LEAVE_PLATOON:
        outmsg.payload = message_pair.first;
        std::cout << "wifi pubbung protocolin" << std::endl;

        pub_platoonProtocolIn_.publish(outmsg);
        break;

      default:
        NODELET_FATAL("[wifi] messagetype not recognized");
        break;

    }
  }


/*****************************************************************************
** HELPER CLASS
*****************************************************************************/


} // namespace platooning

PLUGINLIB_EXPORT_CLASS(platooning::Wifi, nodelet::Nodelet);
// %EndTag(FULLTEXT)%