#include <array>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>
#include<boost/system/error_code.hpp>

#include "Connection.hpp"
#include "Message.hpp"

const size_t MAX_BUFFER_SIZE = 65535;

class Connection::Impl
{
public: 
    void connect(const std::string & address, const uint16_t port) // нужна ли мне здесь асинхронщина, если по задумке сервер 1 (не нужно)
    {
        if(isStarted())
        {
            throw std::runtime_error("Socket already in use!");
        }

        _isStarted = true;

        _worker = std::thread([this, address, port](){
            while(isStarted())
                {
                //синхронный коннект
                _socket = std::make_unique<boost::asio::ip::tcp::socket>(_ioService);
                _endpoint = std::make_unique<boost::asio::ip::tcp::endpoint>(boost::asio::ip::address_v4::from_string(address),port);
                boost::system::error_code ec;
                _socket->connect(*_endpoint, ec);
                if (ec)
                {
                    throw std::runtime_error("Error with connection: " + std::to_string(ec.value()));
                }
                _isConnected = true;
                _listener(); 

                while(_isStarted and _isConnected)
                {
                    // Message msg;

                    // msg._body.data = "Здарова, заебал!";
                    // write(msg.serialize());

                    message::PrivateMessage msg(
                        Header::BaseInfo {}, 
                        Body {"hi beatch"}
                    ); 

                    auto jsonStr = msg.serialize().dump();
                    std::vector<uint8_t> byteStr (jsonStr.begin(), jsonStr.end());
                    write(byteStr);

                    _ioService.poll_one();
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }

                _socket.release();

                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
        });
       
        // асинхронный коннект 
        // _socket.async_connect(*_endpoint.get(), [this](boost::system::error_code error){
        //     if(not error)
        //     {
        //         _isConnected = true;
        //         _listener(); 
        //     }
        //     throw std::runtime_error("Error with connection: " + std::to_string(error.value()));
        // });
    }

    void disconnect()
    {
        if(isConnected())
        {   
            boost::system::error_code error;
            _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
            if(error)
            {
                throw std::runtime_error("Error with disconnection: " + std::to_string(error.value()));
            }
        }
        throw std::runtime_error("Socket already disconnected!");
    }

    void write(std::vector<uint8_t> data)
    {
        if(not isConnected())
            throw std::runtime_error("Client is not connected!");

        _socket->write_some(boost::asio::buffer(data));
    }

    bool isConnected() const
    {
        return _isConnected;
    }

    bool isStarted() const
    {
        return _isStarted;
    }

    void subscribe(std::shared_ptr<raw_consumer> consumer)
    {
        _consumers.push_back(consumer);
    }

private:
    void _listener()
    {
        if (not _isConnected)
        {
            throw std::runtime_error("Client is not connected");
        }

        _socket->async_receive(boost::asio::buffer( _buffer, _buffer.size()), [this](boost::system::error_code error, size_t reseivedBytes) 
        {
            if(not error and reseivedBytes > 0)
            {
                std::vector<char> receivedData(_buffer.begin(), _buffer.begin() + reseivedBytes);
                
                for(auto consumer: _consumers)
                {
                    consumer->consume(receivedData);
                }
            }
            else if (error == boost:: asio::error::eof)
            {
                throw std::runtime_error("Connection was closed by remote peer!");
            }
            else 
            {
                throw std::runtime_error("Error with listener: " + std::to_string(error.value()));
            }

            _listener();
        });
        
    }

private:
    boost::asio::io_service _ioService;
    std::unique_ptr<boost::asio::ip::tcp::socket> _socket;
    std::unique_ptr<boost::asio::ip::tcp::endpoint> _endpoint;

    std::vector<std::shared_ptr<raw_consumer>> _consumers;

    std::thread _worker;

    bool _isConnected;
    bool _isStarted;
    std::array<uint8_t, MAX_BUFFER_SIZE> _buffer;
};

Connection::Connection()
{
    _impl = std::make_unique<Connection::Impl>();
}

Connection::~Connection() = default;

void Connection::write(std::vector<uint8_t> data)
{
    _impl->write(data);
}

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
    return _impl->isConnected();
}

void Connection::subscribe(std::shared_ptr<raw_consumer> consumer)
{
    _impl->subscribe(consumer);
};