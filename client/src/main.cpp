#include <memory>
#include <chrono>

#include <Connection.hpp>
#include <NetModel.hpp>
#include <UI.hpp>
// #include "mainwindow.hpp"

#include<boost/asio.hpp>

// #include <QApplication>
// #include <QTextCodec>

using namespace std::chrono_literals;

int main (int argc, char **argv){
    bool flag = true;

    std::shared_ptr<Connection> connection;

    try
    {
        connection = std::make_shared<Connection>();
        auto net_model = std::make_shared<NetModel>(connection);
        auto ui = std::make_shared<UI>();

        connection->subscribe(net_model);
        connection->connect("0.0.0.0", 4000);
        
        // net_model->subscribe(ui);
    }
    catch(const std::exception& e)
    {
        flag = false;
        std::cerr << e.what() << '\n';
    }
    
    boost::asio::io_context context;
    while(flag)
    {
        if (context.stopped()) 
            context.restart();
    
        if (!context.poll()) 
            std::this_thread::sleep_for(10ms);
    }

    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    // QApplication app(argc, argv);
    // Window window(connection);
    // window.show();
    // return app.exec();
}