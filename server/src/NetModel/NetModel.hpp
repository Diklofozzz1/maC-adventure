#pragma once 

#include<memory>
#include<vector>

#include "IConsumer.hpp"
#include "Message.hpp"
#include "IDataProvider.hpp"
#include "ISender.hpp"

#include <nlohmann/json.hpp>

using PrivateMessageConsumer    = std::shared_ptr<IConsumer<message::PrivateMessage>>;
using PublicMessageConsumer     = std::shared_ptr<IConsumer<message::PublicMessage>>;
using GroupMessageConsumer      = std::shared_ptr<IConsumer<message::GroupMessage>>;
using OnlineListMessageConsumer = std::shared_ptr<IConsumer<message::OnlineListMessage>>;
using PingMessageConsumer       = std::shared_ptr<IConsumer<message::PingMessage>>;
using ConnectMessageConsumer    = std::shared_ptr<IConsumer<message::ConnectMessage>>;
using DisconnectMessageConsumer = std::shared_ptr<IConsumer<message::DisconnectMessage>>;

class NetModel
    : public IConsumer<std::vector<uint8_t>>
    , public IDataProvider<message::PrivateMessage>
    , public IDataProvider<message::PublicMessage>
    , public IDataProvider<message::GroupMessage>
    , public IDataProvider<message::OnlineListMessage>
    , public IDataProvider<message::PingMessage>
    , public IDataProvider<message::ConnectMessage>
    , public IDataProvider<message::DisconnectMessage>
{
public:
    NetModel(std::shared_ptr<ISender> sender)
    : _sender(sender)
    {
    }

    void consume(std::vector<uint8_t> data) override
    {   
        using namespace types::message;

        std::string jsonStr(data.begin(), data.end());
        const auto json = nlohmann::json::parse(jsonStr);

        const auto typecode = strToTypeCode(json[strings::Header][strings::TypeCode].get<std::string>());
        
        using namespace message;

        switch(typecode)
        {
            case TypeCode::One2One: 
            {   
                for (auto const & consumer : _privateMessageConsumers)
                    consumer->consume(PrivateMessage(json));
            }
            case TypeCode::One2All: 
            {
                for (auto const & consumer : _publicMessageConsumers)
                    consumer->consume(PublicMessage(json));
            }
            case TypeCode::One2Group: 
            {
                for (auto const & consumer : _groupMessageConsumers)
                    consumer->consume(GroupMessage(json));
            }
            case TypeCode::Ping: 
            {
                for (auto const & consumer : _onlineListMessageConsumers)
                    consumer->consume(OnlineListMessage(json));
            }
            case TypeCode::Connect: 
            {
                for (auto const & consumer : _pingMessageConsumers)
                    consumer->consume(PingMessage(json));
            }
            case TypeCode::Disconnect: 
            {
                for (auto const & consumer : _connectMessageConsumers)
                    consumer->consume(ConnectMessage(json));
            }
            case TypeCode::OnlineInfo: 
            {
                for (auto const & consumer : _disconnectMessageConsumers)
                    consumer->consume(DisconnectMessage(json));
            }
            case TypeCode::WithBlob:  
            default: break;
        }
    }

    void subscribe(PrivateMessageConsumer consumer) override
    {
        _privateMessageConsumers.push_back(std::move(consumer));
    }

    void subscribe(PublicMessageConsumer consumer) override
    {
        _publicMessageConsumers.push_back(std::move(consumer));
    }

    void subscribe(GroupMessageConsumer consumer) override
    {
        _groupMessageConsumers.push_back(std::move(consumer));
    }

    void subscribe(OnlineListMessageConsumer consumer) override
    {
        _onlineListMessageConsumers.push_back(std::move(consumer));
    }

    void subscribe(PingMessageConsumer consumer) override
    {
        _pingMessageConsumers.push_back(std::move(consumer));
    }

    void subscribe(ConnectMessageConsumer consumer) override
    {
        _connectMessageConsumers.push_back(std::move(consumer));
    }

    void subscribe(DisconnectMessageConsumer consumer) override
    {
        _disconnectMessageConsumers.push_back(std::move(consumer));
    }

private:
    std::vector<PrivateMessageConsumer>    _privateMessageConsumers;
    std::vector<PublicMessageConsumer>     _publicMessageConsumers;
    std::vector<GroupMessageConsumer>      _groupMessageConsumers;
    std::vector<OnlineListMessageConsumer> _onlineListMessageConsumers;
    std::vector<PingMessageConsumer>       _pingMessageConsumers;
    std::vector<ConnectMessageConsumer>    _connectMessageConsumers;
    std::vector<DisconnectMessageConsumer> _disconnectMessageConsumers;

    std::shared_ptr<ISender> _sender;
};