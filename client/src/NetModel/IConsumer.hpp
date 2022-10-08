#pragma once

template<class T>
class IConsumer
{
public:
    virtual void consume(T) = 0;
};