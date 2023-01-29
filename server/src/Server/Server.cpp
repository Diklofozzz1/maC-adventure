#include "Server.hpp"
#include "ConnectionsManager.hpp"
#include "MessageProcessor.hpp"
#include "NetModel.hpp"
#include "TcpConnection.hpp"

#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>


class Server::Impl
{
public:
    Impl()
        : _processor(std::make_shared<MessageProcessor>()), _connectionsManager(std::make_shared<ConnectionsManager>())
    {
    }

    ~Impl()
    {
        stop();
    }

    void start(const std::string & address, const uint32_t port)
    {
        if (isStarted())
        {
            std::cout << "[SERVER] Server already started...\n";
            return;
        }

        _address = address;
        _port = port;

        _service_thread = std::thread(
            [this]()
            {
            auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(_address), _port);
            _acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(_io_service, endpoint);

            _isStarted = true;

            std::cout << "[SERVER MESSAGE] Server started...\n";

            _wait_for_connection();

            while (_isStarted)
            {
                _io_service.poll_one();
                std::this_thread::sleep_for(std::chrono::microseconds(1500));
            }
        });
    }


    void stop()
    {
        if (not isStarted())
        {
            std::cout << "[SERVER]: Already Stoped...\n";
            return;
        }

        if (_service_thread.joinable())
        {
            _service_thread.join();
        }

        _isStarted = false;

        std::cout << "[SERVER]: Stoped...\n";
    }

    bool isStarted()
    {
        return _isStarted;
    }


private:
    void _wait_for_connection()
    {
        auto new_connection = std::make_shared<TcpConnection>(_io_service,
            [this](uint64_t id) -> void
            {
            if (_connections.count(id))
            {
                _connections.erase(id);
                _connectionsManager->removeConnection(id);
            }
        });

        _acceptor->async_accept(
            new_connection->socket(),
            boost::bind(&Impl::_handle_accept, this, new_connection, boost::asio::placeholders::error));
    }

    void _handle_accept(std::shared_ptr<TcpConnection> new_connection, boost::system::error_code error)
    {
        if (not error)
        {
            try
            {
                new_connection->connect();

                _connections[new_connection->get_id()] = std::make_shared<NetModel>(_connectionsManager);

                auto netModel = std::make_shared<NetModel>(_connectionsManager);
                _connections[new_connection->get_id()] = netModel;

                netModel->subscribe(std::static_pointer_cast<IConsumer<message::PrivateMessage>>(_processor));
                netModel->subscribe(std::static_pointer_cast<IConsumer<message::PublicMessage>>(_processor));
                netModel->subscribe(std::static_pointer_cast<IConsumer<message::GroupMessage>>(_processor));
                netModel->subscribe(std::static_pointer_cast<IConsumer<message::OnlineListMessage>>(_processor));
                netModel->subscribe(std::static_pointer_cast<IConsumer<message::PingMessage>>(_processor));
                netModel->subscribe(std::static_pointer_cast<IConsumer<message::ConnectMessage>>(_processor));
                netModel->subscribe(std::static_pointer_cast<IConsumer<message::DisconnectMessage>>(_processor));

                new_connection->subscribe(netModel);

                _connectionsManager->addConnections(new_connection);

                for(auto& item : _connections)
                {
                    std::cout<<item.first<<" : "<<item.second<<std::endl;
                }
            }
            catch (const std::exception & err)
            {
                std::cerr << "[SERVER]: Error: \n" << err.what() << '\n';
            }
        }

        _wait_for_connection();
    }

private:
    bool _isStarted;

    std::string _address;
    uint32_t _port;

    std::thread _service_thread;

    std::map<uint64_t, std::shared_ptr<NetModel>> _connections;

    boost::asio::io_service _io_service;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor;

    std::shared_ptr<ConnectionsManager> _connectionsManager;
    std::shared_ptr<MessageProcessor> _processor;
};

Server::Server()
{
    _impl = std::make_unique<Server::Impl>();
}

Server::~Server() = default;

void Server::start(const std::string & address, const uint32_t port)
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

// void Server::onMessage()
// {
//     _impl->onMessage();
// }

// std::vector<Server::client> Server::connections()
// {
//     _impl->connections();
// }

// void Server::writeTo(client client, std::string data)
// {
//     _impl->writeTo(client, data);
// }