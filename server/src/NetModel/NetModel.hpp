#pragma once 

#include<memory>
#include<vector>

#include "IConsumer.hpp"
#include "Message.hpp"
#include "IDataProvider.hpp"
#include "TcpConnection.hpp"

using message_consumer = std::shared_ptr<IConsumer<Message>>;

class NetModel
    : public IConsumer<std::vector<uint8_t>>
    , public IDataProvider<Message>
    , private std::enable_shared_from_this<NetModel>
{
public:
    NetModel(std::shared_ptr<TcpConnection> connection)
    {
        _connection = connection;
        // _connection->subscribe(shared_from_this());
    }

    void consume(std::vector<uint8_t> data)
    {
        Message msg;
        msg.deSerialize(data);

        for(auto consumer: _messageConsumers)
        {
            consumer->consume(msg);
        }

        std::cout<<msg.str()<<std::endl; // Печатаем прилетевшее сообщение 
    }

    void subscribe(message_consumer consumer) override
    {
        _messageConsumers.push_back(consumer);
    }

private:
    std::vector<message_consumer> _messageConsumers;
    std::shared_ptr<TcpConnection> _connection;
};