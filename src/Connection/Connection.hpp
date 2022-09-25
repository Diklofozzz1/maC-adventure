#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp> 

class Connection
    : public std::enable_shared_from_this<Connection> // пока не придумал, зачем, но пусть пока будет
{
public:
    void connect(const std::string & address, const uint16_t port);
    void disconnect();
    bool isConnected() const;
    [[nodiscard]] std::shared_ptr<Connection> get_ptr(){ return shared_from_this();}

private:
    Connection()=default;
    class Impl;
    std::unique_ptr<Impl> _impl;
};


