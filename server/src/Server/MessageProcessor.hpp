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
        std::cout<<msg.serialize().dump(4)<<std::endl;
    }

    void consume(message::PublicMessage msg) override 
    {
        std::cout<<msg.serialize().dump(4)<<std::endl;
    }

    void consume(message::GroupMessage msg) override 
    {
        std::cout<<msg.serialize().dump(4)<<std::endl;
    }

    void consume(message::OnlineListMessage msg) override 
    {
        std::cout<<msg.serialize().dump(4)<<std::endl;
    }

    void consume(message::PingMessage msg) override 
    {
        std::cout<<msg.serialize().dump(4)<<std::endl;
    }

    void consume(message::ConnectMessage msg) override 
    {
        std::cout<<msg.serialize().dump(4)<<std::endl;
    }

    void consume(message::DisconnectMessage msg) override 
    {
        std::cout<<msg.serialize().dump(4)<<std::endl;
    }
};