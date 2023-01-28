#pragma once 

template<class T>
class IConsumer
{
public:
    virtual ~IConsumer() = default;
    virtual void consume(T) = 0;
};
