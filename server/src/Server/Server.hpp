#pragma once

#include<memory>
#include<string>
#include<vector>

class Server
{
public:
    void start(const std::string &address, const uint32_t port);
    void stop();

    bool isStarted();

    
private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};