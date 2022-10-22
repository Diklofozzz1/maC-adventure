#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>

#include "IConsumer.hpp"

using raw_consumer = IConsumer<std::vector<uint8_t>>; 

class TcpConnection
    : private std::enable_shared_from_this<TcpConnection>
{
public:
    uint64_t get_id();
    void connect();
    void disconnect();
    bool isConnected() const;
    boost::asio::ip::tcp::socket &socket();
    [[nodiscard]] std::shared_ptr<TcpConnection> get_ptr(){ return shared_from_this();}

    void subscribe(std::shared_ptr<raw_consumer>);
    void unSubscribe();

private:
    TcpConnection()=default;
    class Impl;
    std::unique_ptr<Impl> _impl;
};
