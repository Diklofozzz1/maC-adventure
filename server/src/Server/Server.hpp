#pragma once

#include<memory>
#include<string>
#include<vector>


class Server
{
public:
    Server();
    ~Server();
    
    void start(const std::string &address, const uint32_t port);
    void stop();

    bool isStarted();

    // void onMessage() override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};