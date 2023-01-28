#pragma once

#include "ISender.hpp"
#include "TcpConnection.hpp"

#include <map>

class ConnectionsManager : public ISender
{
public:
    ConnectionsManager()
    {
    }

    void addConnections(std::shared_ptr<TcpConnection> connection)
    {
        _connections[connection->get_id()] = connection;
    }

    void removeConnection(uint64_t id)
    {
        if (_connections.count(id))
        {
            _connections.erase(id);
        }
    }

    void sendToOne(uint64_t id, std::vector<uint8_t> data) override
    {
        if (_connections.count(id))
        {
            std::cerr << "[SERVER]: Error: \n" << "There are no connection exist with id " << id << std::endl;
            return;
        }
        const auto connection = _connections.at(id);
        connection->write(data); 
    }

    void sendToAll() override
    {
    }

    void sendToGroup() override
    {
    }

private:
    std::map<uint64_t, std::shared_ptr<TcpConnection>> _connections;
};
