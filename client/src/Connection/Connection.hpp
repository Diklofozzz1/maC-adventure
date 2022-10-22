#pragma once

#include <vector>
#include <string>
#include <memory>

#include "IConsumer.hpp"

using raw_consumer = IConsumer<std::vector<char>>; 

class Connection
{
public:
    Connection();
    ~Connection();
    void connect(const std::string & address, const uint16_t port);
    void disconnect();
    bool isConnected() const;
    // [[nodiscard]] std::shared_ptr<Connection> get_ptr(){ return shared_from_this();}

    void subscribe(std::shared_ptr<raw_consumer>);
    void unSubscribe();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};