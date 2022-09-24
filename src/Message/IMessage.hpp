#pragma once

#include <iostream>
#include <vector>

class IMessage
{
public:
    virtual std::vector<uint8_t> serialize() = 0;
    virtual void deSerialize(std::vector<uint8_t> data) = 0;
};


