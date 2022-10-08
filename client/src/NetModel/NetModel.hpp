#pragma once

#include<iostream>
#include<vector>
#include<memory>

#include "IConsumer.hpp"
#include "Connection.hpp"
#include "Message.hpp"

using message_consumer = std::shared_ptr<IConsumer<Message>>;

class NetModel
    : public IConsumer<std::vector<char>>
    , public IDataProvider<Message>
    , private std::enable_shared_from_this<Connection>
{
public:
    NetModel(std::shared_ptr<Connection> connection)
    {
        _connection = connection;
        _connection->subscribe(shared_from_this());
    };

    void consume(std::vector<char> data){
        Message msg;
        std::vector<uint8_t> castData(data.begin(), data.end());
        msg.deSerialize(castData);
               
        for(auto consumer: _messageConsumers)
        {
            consumer->consume(msg);
        }
    }

    void subcribe(message_consumer consumer) override
    {
        _messageConsumers.push_back(consumer);
    }   

private:
    std::shared_ptr<Connection> _connection;
    std::vector<message_consumer> _messageConsumers;
};