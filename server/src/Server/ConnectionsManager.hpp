#pragma once

#include "NetModel.hpp"
#include "ISender.hpp"

#include<map>

class ConnectionsManager
: public ISender
{
public:
    ConnectionsManager()
    {
    }

    void addConnections(uint64_t id, std::shared_ptr<NetModel> netModel)
    {
        _connections[id] = netModel;
    }

    void removeConnection(uint64_t id)
    {
        if(_connections.count(id))
        {
            _connections.erase(id);
        }
    }
    
    void sendToOne() override 
    {

    }

    void sendToAll() override
    {

    }
    
    void sendToGroup() override
    {

    }
    
private:
    std::map<uint64_t, std::shared_ptr<NetModel>> _connections;
};
