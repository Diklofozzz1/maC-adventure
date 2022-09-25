#include "Connection.hpp"

#include <boost/asio.hpp> 

class Connection::Impl
{
public:
    void connect(const std::string & address, const uint16_t port)
    {
        if(isConnected())
        {
            throw std::runtime_error("Socket already in use!");
        }
        
        _endpoint = std::make_unique<boost::asio::ip::tcp::endpoint>(boost::asio::ip::address_v4::from_string(address),port);
        
        _socket.async_connect(*_endpoint.get(), [this](boost::system::error_code error){
            if(not error)
            {
                _isConnected = true;
                _listening(); // todo: сделать метод прослушивания сообщений с сервера.
            }
            throw std::runtime_error("Error with connection: " + std::to_string(error.value()));
        });
    }

    void disconnect()
    {
        if(isConnected())
        {   
            boost::system::error_code error;
            _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
            if(error)
            {
                throw std::runtime_error("Error with disconnection: " + std::to_string(error.value()));
            }
        }
        throw std::runtime_error("Socket already disconnected!");
    };

    bool isConnected() const
    {
        return _isConnected;
    };

private:
    boost::asio::io_service _ioService;
    boost::asio::ip::tcp::socket _socket;
    std::unique_ptr<boost::asio::ip::tcp::endpoint> _endpoint;

    bool _isConnected;
};

void Connection::connect(const std::string & address, const uint16_t port)
{
    _impl->connect(address, port);
}

void  Connection::disconnect()
{
    _impl->disconnect();
}

bool Connection::isConnected() const
{
    _impl->isConnected();
}