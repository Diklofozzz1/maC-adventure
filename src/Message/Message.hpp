#pragma once

#include <iostream>
#include <array>
#include <string>

#include "IMessage.hpp"

// #include <boost/asio.hpp> 

// todo: посмотреть возможность десириализации при помощи конструктора класса 

class Message
    : public IMessage
{
public:
    struct Header
    {
        std::string creator_name = "";
        std::string receiver_name = "";
    };

    struct Body
    {
        std::string data = "";
    };

    std::vector<uint8_t> serialize() override;
    void deSerialize(std::vector<uint8_t> data) override;

private:
    Header _header;
    Body _body;
};
