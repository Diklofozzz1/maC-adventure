#pragma once

#include <memory>

#include <IConsumer.hpp>

template <class T>
class IDataProvider
{
public:
    virtual void subcribe(std::shared_ptr<IConsumer<T>>) = 0;
};