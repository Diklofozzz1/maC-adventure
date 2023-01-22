#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>

#include "IConsumer.hpp"

using raw_consumer = IConsumer<std::vector<uint8_t>>; 

using refuseHandler = std::function<void(uint64_t)>;

class TcpConnection
    : private std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(boost::asio::io_service& io_service, refuseHandler handler);
    ~TcpConnection();

    uint64_t get_id();
    void connect();
    void disconnect();
    bool isConnected() const;
    boost::asio::ip::tcp::socket &socket();
    [[nodiscard]] std::shared_ptr<TcpConnection> get_ptr(){ return shared_from_this();}

    void write(std::vector<uint8_t> data);

    void subscribe(std::shared_ptr<raw_consumer>);
    void unSubscribe();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};
