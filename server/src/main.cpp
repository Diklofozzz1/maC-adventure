#include "Server.hpp"

#include <boost/asio.hpp>
#include<thread>
#include<chrono>

using namespace std::chrono_literals;

int main()
{
    Server server;
    server.start("0.0.0.0", 4000);

    boost::asio::io_context context;
    while(true)
    {
        if (context.stopped()) 
            context.restart();
    
        if (!context.poll()) 
            std::this_thread::sleep_for(10ms);
    }
}