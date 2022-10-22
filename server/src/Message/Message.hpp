#pragma once

#include <iostream>
#include <string>

#include "IMessage.hpp"

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

    std::string str()
    {
        return _body.data;
    }

private:
    Header _header;
    Body _body;
};