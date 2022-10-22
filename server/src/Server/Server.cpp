#include "Server.hpp"
#include "TcpConnection.hpp"
#include "NetModel.hpp"

#include<memory>
#include<thread>
#include<string>
#include<vector>
#include<map>

#include <boost/asio.hpp>
#include <boost/bind.hpp>


class Server::Impl
{
public: 
    ~Impl() 
    { 
        stop();
    }
    
    void start(const std::string &address, const uint32_t port)
    {   
        if(isStarted())
        {
            std::cout << "[SERVER] Server already started...\n";
            return;
        }   
        
        _address = address;
        _port = port;

        _service_thread = std::thread([this]()
        {
            _isStarted = true;
            
            auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(_address), _port);
            _acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(_io_service, endpoint);

            std::cout << "[SERVER MESSAGE] Server started...\n";

            _wait_for_connection();
                        
            while(_isStarted)
            {
                _io_service.poll_one();
                std::this_thread::sleep_for(std::chrono::microseconds(1500));
            }
        });
    } 


    void stop()
    {
        if(not isStarted())
        {
            std::cout<<"[SERVER]: Already Stooped...\n";
            return;
        }

        if(_service_thread.joinable())
        {
            _service_thread.join();
        }
    
        _isStarted = false;

        std::cout<<"[SERVER]: Stooped...\n";
    }

    bool isStarted()
    {
        return _isStarted;
    }

    // void writeTo(client client, std::string data)
    // {

    // }
    
private:
    void _wait_for_connection()
    {
        auto new_connection = std::make_shared<TcpConnection>(_io_service);

        _acceptor->async_accept(new_connection->socket(), boost::bind(
            &Impl::_handle_accept,
            this,
            new_connection,
            boost::asio::placeholders::error
        ));
    }

    void _handle_accept(std::shared_ptr<TcpConnection> new_connection, boost::system::error_code error)
    {
        if(not error)
        {
            try
            {
                new_connection->connect();
                
                connections[new_connection->get_id()] = std::make_shared<NetModel>(new_connection);

                new_connection->subscribe(connections[new_connection->get_id()]);
            }
            catch(const std::exception& err)
            {
                std::cerr <<"[SERVER]: Error: \n"<< err.what() << '\n';
            }
        }

        _wait_for_connection();
    }

private:
    bool _isStarted;
    
    std::string _address;
    uint32_t _port;

    std::thread _service_thread;

    std::map<uint64_t, std::shared_ptr<NetModel>> connections;

    boost::asio::io_service _io_service;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor;
};

Server::Server()
{
    _impl = std::make_unique<Server::Impl>();
}

Server::~Server() = default;

void Server::start(const std::string &address, const uint32_t port)
{
    _impl->start(address, port);
}

void Server::stop()
{
    _impl->stop();
}

bool Server::isStarted()
{
    return _impl->isStarted();
}

// std::vector<Server::client> Server::connections()
// {
//     _impl->connections();
// }

// void Server::writeTo(client client, std::string data)
// {
//     _impl->writeTo(client, data);
// }