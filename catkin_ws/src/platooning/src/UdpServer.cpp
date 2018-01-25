//
// Created by stepo on 1/21/18.
//


#include "UdpServer.hpp"

UdpServer::UdpServer( boost::function<void(std::shared_ptr<std::vector<char>>)> receive_callback
    , udp::endpoint bind_endpoint
    , udp::endpoint remote_endpoint) {

  std::cout << "iothread" << std::endl;

  socket_ptr_ = std::unique_ptr<udp::socket>( new udp::socket( io_service_, bind_endpoint ));

  boost::asio::socket_base::broadcast enable_broadcast(true);
  socket_ptr_->set_option(enable_broadcast);
  boost::asio::socket_base::reuse_address reuse_address(true);
  socket_ptr_->set_option(reuse_address);

  remote_endpoint_ = std::move(remote_endpoint);

  std::cout << bind_endpoint.address() << bind_endpoint.port() << bind_endpoint.data() << std::endl;

  callback_ = receive_callback;

  start_receive();

  io_thread_ = boost::thread([this] { this->io_service_.run();});
}

void UdpServer::start_receive() {

  socket_ptr_->async_receive_from(
      boost::asio::buffer(recv_buffer_), remote_endpoint_,
      boost::bind(&UdpServer::handle_receive, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
  std::cout << "server started async recv" << std::endl;

}

/**
 **  @throws throws exception if msg to send is larger than max_recv_bytes
 **/

void UdpServer::start_send(platooning::platoonProtocolOut msg) {
  start_send(msg.payload, msg.message_type);
}

void UdpServer::start_send(std::string message, int32_t message_type) {

  std::cout << "start send check len of msg \"" << message << "\" type " << message_type << std::endl;

  if (message.length() + sizeof(message_type) + 1 > MAX_RECV_BYTES) {
    throw;
  }

  try {
    memcpy(send_buffer_.data(),&message_type,sizeof(message_type));

    memcpy(send_buffer_.data()+ sizeof(message_type),message.c_str(), message.length());

    send_buffer_[sizeof(int32_t) + message.length() ] = '\0';

    std::cout << "srv sending " << send_buffer_.data() << std::endl;
  } catch( std::exception &ex ) {
    std::cerr << "udpserver error stuffing sendbuffer " << ex.what() << std::endl;
  }

  try {
    socket_ptr_->async_send_to(
        boost::asio::buffer(send_buffer_), remote_endpoint_,
        boost::bind(&UdpServer::handle_send, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  std::cout << "srv send done" << std::endl;

}

void UdpServer::handle_receive(const boost::system::error_code &error,
                                      std::size_t size /*bytes_transferred*/) {
  std::cout << "udpserv handl recv " << !error <<  (error == boost::asio::error::message_size) << std::endl;

  if (!error || error == boost::asio::error::message_size) {

    std::shared_ptr<std::vector<char>> pbuf = std::shared_ptr<std::vector<char>>(
        new std::vector<char>(recv_buffer_.begin(), recv_buffer_.end()));


    callback_(pbuf);

    start_receive();

    std::cout << "srv recv done" << std::endl;
  } else {
    std::cerr << "[udpserver] error during handling receive:" << error.message();
  }
}

void UdpServer::handle_send(const boost::system::error_code &error,
                            std::size_t bytes /*bytes_transferred*/) {
  if( error ) {
    std::cerr << "server handlesend eror " << error.message() << std::endl;
  } else {
    std::cout << "server sent " << bytes << " bytes" << std::endl;
  }
}


