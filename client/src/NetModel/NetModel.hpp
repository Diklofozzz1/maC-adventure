#pragma once

#include<iostream>
#include<vector>
#include<memory>

#include "IDataProvider.hpp"
#include "IConsumer.hpp"
#include "Connection.hpp"
#include "Message.hpp"

// using message_consumer = std::shared_ptr<IConsumer<Message>>;

class NetModel
    : public IConsumer<std::vector<char>>
    // , public IDataProvider<Message>
    , private std::enable_shared_from_this<NetModel>
{
public:
    NetModel(std::shared_ptr<Connection> connection)
    {
        _connection = connection;
        // _connection->subscribe(shared_from_this());
    };

    void consume(std::vector<char> data){
        using namespace types::message;

        std::string jsonStr(data.begin(), data.end());
        const auto json = nlohmann::json::parse(jsonStr);

        const auto typecode
            = strToTypeCode(json[strings::Header][strings::TypeCode].get<std::string>());

        std::cout << json.dump(4) << std::endl;
        
        // Message msg;
        // std::vector<uint8_t> castData(data.begin(), data.end());
        // msg.deSerialize(castData);
               
        // for(auto consumer: _messageConsumers)
        // {
        //     consumer->consume(msg);
        // }
    }
    
    // void subscribe(message_consumer consumer) override
    // {
    //     _messageConsumers.push_back(consumer);
    // }   

private:
    std::shared_ptr<Connection> _connection;
    // std::vector<message_consumer> _messageConsumers;
};