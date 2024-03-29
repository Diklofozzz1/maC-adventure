#pragma once

#include <memory>

#include "IConsumer.hpp"

template<class T> class IDataProvider
{
public:
    virtual ~IDataProvider() = default;
    virtual void subscribe(std::shared_ptr<IConsumer<T>>) = 0;
};
