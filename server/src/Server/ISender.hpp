#pragma once 

class ISender
{
public:
    virtual ~ISender() = default;
    
    virtual void sendToOne() = 0;
    virtual void sendToAll()= 0;
    virtual void sendToGroup()= 0;
};