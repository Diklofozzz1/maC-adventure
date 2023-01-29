#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <nlohmann/json.hpp>

#include "IMessage.hpp"
#include "MessageTypes.hpp"


struct Header
{
    struct BlobInfo
    {
        bool haveBlob = false;
        uint64_t blobStartPosition = 0;
        uint64_t blobEndPosition = 0;
        types::message::BlobType blobType = types::message::BlobType::Unknown;
    };

    struct BaseInfo
    {
        uint64_t from;
        uint64_t to;
        std::string date;

        BlobInfo blobInfo;
    };

    Header() = default;

    Header(BaseInfo const & info, types::message::TypeCode code) : typeCode(code), info(info)
    {
    }

    types::message::TypeCode typeCode = types::message::TypeCode::NoType;
    BaseInfo info;
};

struct Body
{
    std::string data;
};

template<types::message::TypeCode Type> class HeaderOnlyMessage : public IMessage
{
public:
    virtual ~HeaderOnlyMessage() = default;

    HeaderOnlyMessage(Header::BaseInfo const & info) : _header(info, Type)
    {
    }

    HeaderOnlyMessage(nlohmann::json const & raw)
    {
        deserialize(raw);
    }

    void deserialize(nlohmann::json const & json_deser) override
    {   
        using namespace types::message;
        
        _header.typeCode = strToTypeCode(json_deser[strings::Header][strings::TypeCode].get<std::string>());
        
        const auto & baseInfo = json_deser[strings::Header][strings::BaseInfo];
        _header.info.from =  baseInfo[strings::From].get<uint64_t>();
        _header.info.to =  baseInfo[strings::To].get<uint64_t>();
        _header.info.date =  baseInfo[strings::Date].get<std::string>();

        const auto & baseBlob = json_deser[strings::Header][strings::BlobInfo];
        _header.info.blobInfo.haveBlob = baseBlob[strings::HaveBlob].get<bool>();
        _header.info.blobInfo.blobStartPosition = baseBlob[strings::BlobStartPosition].get<uint64_t>();
        _header.info.blobInfo.blobStartPosition = baseBlob[strings::BlobEndPosition].get<uint64_t>();
        _header.info.blobInfo.blobType = strToBlobType(baseBlob[strings::BlobType].get<std::string>());
    }
            
    nlohmann::json serialize() override
    {
        using namespace types::message;

        nlohmann::json json;

        auto & header = json[strings::Header];
        header[strings::TypeCode] = typeCodeToStr(_header.typeCode);

        auto & baseInfo = header[strings::BaseInfo];
        baseInfo[strings::From] = _header.info.from;
        baseInfo[strings::To] = _header.info.to;
        baseInfo[strings::Date] = _header.info.date;

        auto & blobInfo = header[strings::BlobInfo];
        blobInfo[strings::HaveBlob] = _header.info.blobInfo.haveBlob;
        blobInfo[strings::BlobStartPosition] =_header.info.blobInfo.blobStartPosition; 
        blobInfo[strings::BlobEndPosition] = _header.info.blobInfo.blobStartPosition;
        blobInfo[strings::BlobType] = blobTypeToStr(_header.info.blobInfo.blobType);

        return json;
    }

    types::message::TypeCode type() const noexcept
    {
        return _header.typeCode;
    }

    Header::BaseInfo baseInfo() const noexcept
    {
        return _header.info;
    }

protected:
    Header _header;
};

template<types::message::TypeCode Type> class HeaderWithBodyMessage : public IMessage
{
public:
    virtual ~HeaderWithBodyMessage() = default;

    HeaderWithBodyMessage(Header::BaseInfo const & info, Body body)
        : _header(info, Type), _body(body)
    {
    }

    HeaderWithBodyMessage(nlohmann::json const & raw)
    {
        deserialize(raw);
    }

    void deserialize(nlohmann::json const & json_deser) override
    { 
        using namespace types::message;
        
        _header.typeCode = strToTypeCode(json_deser[strings::Header][strings::TypeCode].get<std::string>());
        
        const auto & baseInfo = json_deser[strings::Header][strings::BaseInfo];
        _header.info.from =  baseInfo[strings::From].get<uint64_t>();
        _header.info.to =  baseInfo[strings::To].get<uint64_t>();
        _header.info.date =  baseInfo[strings::Date].get<std::string>();

        const auto & baseBlob = json_deser[strings::Header][strings::BlobInfo];
        _header.info.blobInfo.haveBlob = baseBlob[strings::HaveBlob].get<bool>();
        _header.info.blobInfo.blobStartPosition = baseBlob[strings::BlobStartPosition].get<uint64_t>();
        _header.info.blobInfo.blobStartPosition = baseBlob[strings::BlobEndPosition].get<uint64_t>();
        _header.info.blobInfo.blobType = strToBlobType(baseBlob[strings::BlobType].get<std::string>());

        _body.data = json_deser[strings::Body][strings::Data].get<std::string>();
    }

    nlohmann::json serialize() override
    {
        using namespace types::message;

        nlohmann::json json;

        auto & header = json[strings::Header];
        header[strings::TypeCode] = typeCodeToStr(_header.typeCode);

        auto & baseInfo = header[strings::BaseInfo];
        baseInfo[strings::From] = _header.info.from;
        baseInfo[strings::To] = _header.info.to;
        baseInfo[strings::Date] = _header.info.date;

        auto & blobInfo = header[strings::BlobInfo];
        blobInfo[strings::HaveBlob] = _header.info.blobInfo.haveBlob;
        blobInfo[strings::BlobStartPosition] =_header.info.blobInfo.blobStartPosition; 
        blobInfo[strings::BlobEndPosition] = _header.info.blobInfo.blobStartPosition;
        blobInfo[strings::BlobType] = blobTypeToStr(_header.info.blobInfo.blobType);

        auto & body = json[strings::Body];
        body[strings::Data] =  _body.data;

        return json;
    }

    types::message::TypeCode type() const noexcept
    {
        return _header.typeCode;
    }

    Header::BaseInfo baseInfo() const noexcept
    {
        return _header.info;
    }

    Body body() const noexcept
    {
        return _body;
    }

protected:
    Header _header;
    Body _body;
};

// class BlobMessage
//     : public HeaderWithBodyMessage<types::message::TypeCode::WithBlob>
// {
// public:
//     void deserialize(nlohmann::json const& raw) override
//     {
//         /* base HeaderWithBodyMessage deserialize */
//         if (_header.info.blobInfo.haveBlob)
//         {
//             // deserialize base message info
//             HeaderWithBodyMessage::deserialize({raw.begin(), std::next(raw.begin(),
//             _header.info.blobInfo.blobStartPosition - 1)});

//             // deserialize blob
//             if (not _blob.empty())
//                 _blob.clear();

//             _blob.assign(std::next(raw.begin(), _header.info.blobInfo.blobStartPosition),
//                 std::next(raw.begin(), _header.info.blobInfo.blobEndPosition));
//         }
//     }

//     nlohmann::json serialize() override {
//         if (not _cachedRaw.empty())
//             return _cachedRaw;

//         /*
//             base HeaderWithBodyMessage serialize:
//                 fill raw and send it.
//         */
//         return _cachedRaw;
//     }

//     std::pair<Header::BlobInfo, nlohmann::json> blob() const noexcept { return {}; }

// protected:
//     nlohmann::json _blob;
// };

namespace message
{

using PrivateMessage = HeaderWithBodyMessage<types::message::TypeCode::One2One>;
using PublicMessage = HeaderWithBodyMessage<types::message::TypeCode::One2All>;
using GroupMessage = HeaderWithBodyMessage<types::message::TypeCode::One2Group>;
using OnlineListMessage = HeaderWithBodyMessage<types::message::TypeCode::OnlineInfo>;

using PingMessage = HeaderOnlyMessage<types::message::TypeCode::Ping>;
using ConnectMessage = HeaderOnlyMessage<types::message::TypeCode::Connect>;
using DisconnectMessage = HeaderOnlyMessage<types::message::TypeCode::Disconnect>;

} // namespace message