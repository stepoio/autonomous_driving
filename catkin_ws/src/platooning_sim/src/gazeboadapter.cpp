//
// Created by stepo on 3/4/18.
//

#include "gazeboadapter.hpp"

namespace platooning_sim {

gazeboadapter::gazeboadapter() {
	std::cout << "HEEELLLLOOOOOO000" << std::endl;
	for (uint32_t i = 0; i < MAXVEHICLES; i++) {
		vehicles_enabled_[i] = false;
	}
}

gazeboadapter::~gazeboadapter() {

}

void gazeboadapter::onInit() {

	pub_ = nh_.advertise<platooning::platoonProtocol>(topics::IN_PLATOONING_MSG, 100);

	boost::function<void(std::pair<std::string, uint32_t>)>
		cbfun(boost::bind(boost::mem_fn(&gazeboadapter::hndl_recv_udp), this, _1));

	server_ = std::unique_ptr<UdpServer>(
		new UdpServer(
			cbfun, udp::endpoint(udp::v4(), 13000), udp::endpoint(boost::asio::ip::address_v4::broadcast(), 13001)));
	server_->set_filter_own_broadcasts(false);
}

void gazeboadapter::hndl_recv_udp(std::pair<std::string, uint32_t> packet) {

	platooning::stmupdate msg;

	switch (packet.second) {
		case STMSIM_UPDATE: platooning::decode_json(packet.first, msg);

			break;
		default: break;
	}

}
}

PLUGINLIB_EXPORT_CLASS(platooning_sim::gazeboadapter, nodelet::Nodelet);
// %EndTag(FULLTEXT)%