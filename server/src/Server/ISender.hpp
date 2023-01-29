#pragma once

class ISender
{
public:
    virtual ~ISender() = default;

    virtual void sendToOne(uint64_t, std::vector<uint8_t> data) = 0;
    virtual void sendToAll(std::vector<uint8_t> data) = 0;
    virtual void sendToGroup() = 0;
};