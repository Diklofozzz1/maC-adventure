#include "TcpConnection.hpp"

#include<memory>
#include<string>
#include<vector>
#include<chrono>
#include<functional>

#include <boost/asio.hpp>

const size_t MAX_BUFFER_SIZE = 65535;

class TcpConnection::Impl
     : std::enable_shared_from_this<TcpConnection::Impl>
{
public:
    Impl(boost::asio::io_service& io_service, 
    refuseHandler handler,
    uint64_t id = std::chrono::system_clock::now().time_since_epoch().count())
    : _socket(io_service)
    , _id(id)
    , _handler(handler)
    {}

    ~Impl()
    {
        disconnect();
    }

    uint64_t get_id()
    {
        return _id;
    }

    void connect()
    {
        if(isConnected())
            throw std::runtime_error("Already connected!");
        
        _isConnected = true;
        _listener();
    }

    void disconnect()
    {
        if(isConnected())
        {
            _isConnected = false;
            boost::system::error_code error;
            _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
            _socket.close();
        }
    }

    bool isConnected()
    {
        return _isConnected;
    }

    boost::asio::ip::tcp::socket &socket()
    {
        return _socket;
    }

    void write(std::vector<uint8_t> data)
    {   
        if(not isConnected())
            throw std::runtime_error("");

        _socket.write_some(boost::asio::buffer(data));
    };

    void subscribe(std::shared_ptr<raw_consumer> consumer)
    {
        _consumers.push_back(consumer);
    }

private:
    void _listener()
    {
        if(not isConnected())
        {
            throw std::runtime_error("There is no connection");
        }

        _socket.async_receive(boost::asio::buffer(_buffer, _buffer.size()), [this](boost::system::error_code error, size_t reseivedBytes){
            if(not error and reseivedBytes > 0)
            {
                std::vector<uint8_t> recivedData(_buffer.begin(), _buffer.begin() + reseivedBytes);
                
                for(auto consumer: _consumers)
                {
                    consumer->consume(recivedData);
                }
            }
            else if (error == boost::asio::error::eof)
            {
                _disconnectionHandler();
            }
            else 
            {
                throw std::runtime_error("Error with listener: " + std::to_string(error.value()));
            }

            _listener();
        });
    }

    // Кастыль...
    void _disconnectionHandler()
    {
        // throw std::runtime_error("Connection was closed by remote peer!");
        std::cout<<"Connection with: "<<_id<<" was closed by remote peer!"<<std::endl;
        _handler(_id);
    }

private:
    uint64_t _id = 0;

    boost::asio::ip::tcp::socket _socket;

    bool _isConnected;
    std::array<uint8_t, MAX_BUFFER_SIZE> _buffer;
    std::vector<std::shared_ptr<raw_consumer>> _consumers;

    std::function<void(uint64_t)> _handler;
};

TcpConnection::TcpConnection(boost::asio::io_service& io_service, refuseHandler handler)
{
    // boost::asio::io_service io_service;
    _impl = std::make_unique<TcpConnection::Impl>(io_service, handler);
}

TcpConnection::~TcpConnection() = default;

uint64_t TcpConnection::get_id()
{
    return _impl->get_id();
}

void TcpConnection::connect()
{
    _impl->connect();
}

boost::asio::ip::tcp::socket &TcpConnection::socket()
{
    return _impl->socket();
}

void  TcpConnection::disconnect()
{
    _impl->disconnect();
}

bool TcpConnection::isConnected() const
{
    return _impl->isConnected();
}

void TcpConnection::subscribe(std::shared_ptr<raw_consumer> consumer)
{
    _impl->subscribe(consumer);
};

void TcpConnection::write(std::vector<uint8_t> data)
{
    _impl->write(data);
};