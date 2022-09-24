#include "Message.hpp"

std::vector<uint8_t> Message::serialize() 
{
    std::vector<uint8_t> data;

    data.push_back(_header.creator_name.size());
    data.insert(data.end(), _header.creator_name.begin(),  _header.creator_name.end());

    data.push_back(_header.receiver_name.size());
    data.insert(data.end(), _header.receiver_name.begin(),  _header.receiver_name.end());

    data.push_back(_body.data.size());
    data.insert(data.end(), _body.data.begin(),  _body.data.end());

    return data;
};

void Message::deSerialize(std::vector<uint8_t> data)
{
    auto extractor = [](std::vector<uint8_t> &data) -> std::string
    {
        if (not data.size())
        {
            throw std::runtime_error("Recived data is empty!");
        };

        auto field_size = data.at(0);
        if(data.size() < field_size) 
        {
            throw std::runtime_error("Data size is smaller then field size!");
        }

        data.erase(data.begin());

        std::string result;

        result.insert(result.end(), data.begin(), data.begin() + field_size);
        data.erase(data.begin(), data.begin() + field_size);

        return  std::move(result);
    };

    _header.creator_name = extractor(data);
    _header.receiver_name = extractor(data);
    _body.data = extractor(data);
};