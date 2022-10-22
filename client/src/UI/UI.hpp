#pragma once

#include <iostream>
#include "IConsumer.hpp"
#include "Message.hpp"

class UI
    : public IConsumer<Message>
{
public:
    void consume(Message msg) override
    {
        std::cout<<msg.str()<<std::endl;
    }
};