#pragma once

#include <iostream>
#include <vector>

class IMessage
{
public:
    virtual ~IMessage() = default;

    virtual void deserialize(nlohmann::json const&) = 0;
    virtual nlohmann::json serialize() = 0;
};