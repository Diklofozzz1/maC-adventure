#include "IConsumer.hpp"
#include "IDataProvider.hpp"
#include "Message.hpp"

class MessageProcessor
    : public IConsumer<message::PrivateMessage>
    , public IConsumer<message::PublicMessage>
    , public IConsumer<message::GroupMessage>
    , public IConsumer<message::OnlineListMessage>
    , public IConsumer<message::PingMessage>
    , public IConsumer<message::ConnectMessage>
    , public IConsumer<message::DisconnectMessage>
{
public:
    void consume(message::PrivateMessage msg) override 
    {
        // TODO: handle message
    }

    void consume(message::PublicMessage msg) override 
    {
        // TODO: handle message
    }

    void consume(message::GroupMessage msg) override 
    {
        // TODO: handle message
    }

    void consume(message::OnlineListMessage msg) override 
    {
        // TODO: handle message
    }

    void consume(message::PingMessage msg) override 
    {
        // TODO: handle message
    }

    void consume(message::ConnectMessage msg) override 
    {
        // TODO: handle message
    }

    void consume(message::DisconnectMessage msg) override 
    {
        // TODO: handle message
    }
};