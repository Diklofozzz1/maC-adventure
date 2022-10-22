#include <memory>
#include <chrono>

#include <Connection.hpp>
#include <NetModel.hpp>
#include <UI.hpp>

#include<boost/asio.hpp>

using namespace std::chrono_literals;

int main (){
    auto connection = std::make_shared<Connection>();
    auto net_model = std::make_shared<NetModel>(connection);
    auto ui = std::make_shared<UI>();

    connection->subscribe(net_model);
    connection->connect("127.0.0.1", 4000);
    
    net_model->subscribe(ui);

    boost::asio::io_context context;
    while(true)
    {
        if (context.stopped()) 
            context.restart();
    
        if (!context.poll()) 
            std::this_thread::sleep_for(10ms);
    }
}