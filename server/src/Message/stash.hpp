#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

enum class TypeCode
{
    NoType = 0,

    One2One,
    One2All,
    One2Group,

    Ping,
    Connect,
    Disconnect,

    OnlineInfo,

    WithBlob
};

enum class BlobType
{
    Unknown = 0,

    PNG,
    JPEG,
    GIF,

    WAV,
    MP4,

    TXT
};

template <class T>
class IMessage
{
public:
    virtual ~IMessage() = default;

    virtual void deserialize(std::vector<std::byte> const&) = 0;
    virtual std::vector<std::byte> serialize() = 0;
};

struct Header
{
    struct BlobInfo
    {
        bool haveBlob = false;
        uint64_t blobStartPosition = -1;
        uint64_t blobEndPosition = -1;
        BlobType blobType = BlobType::Unknown;
    };

    struct BaseInfo
    {
        std::string from;
        std::string to;
        std::string date;

        BlobInfo blobInfo;
    };

    Header(BaseInfo const& info, TypeCode code)
        : type(code), info(info) {}

    TypeCode type = TypeCode::NoType;
    BaseInfo info;
};

struct Body
{
    std::string data;
};

template <TypeCode Type>
class HeaderOnlyMessage
    : public IMessage<HeaderOnlyMessage<Type>>
{
public:
    virtual ~HeaderOnlyMessage() = default;

    HeaderOnlyMessage(Header::BaseInfo const& info) 
        : _header(info, Type) {}

    HeaderOnlyMessage(std::vector<std::byte> const& raw) 
    { deserialize(raw); }

    void deserialize(std::vector<std::byte> const&) override { /* base header only deserialize */ }
    
    std::vector<std::byte> serialize() override { 
        if (not _cachedRaw.empty())
            return _cachedRaw;
        
        /* 
            base HeaderOnlyMessage serialize:
                fill raw and send it.
        */
        return _cachedRaw; 
    }
    TypeCode type() const noexcept { return _header.type; }
    Header::BaseInfo baseInfo() const noexcept { return _header.info; }

protected:
    Header _header;

    std::vector<std::byte> _cachedRaw;
};

template <TypeCode Type>
class HeaderWithBodyMessage
    : public IMessage<HeaderWithBodyMessage<Type>>
{
public:
    virtual ~HeaderWithBodyMessage() = default;

    HeaderWithBodyMessage(Header::BaseInfo const& info, Body body) 
        : _header(info, Type), _body(body) {}

    HeaderWithBodyMessage(std::vector<std::byte> const& raw) 
    { deserialize(raw); }
    
    void deserialize(std::vector<std::byte> const&) override { /* base HeaderWithBodyMessage deserialize */ }
    
    std::vector<std::byte> serialize() override { 
        if (not _cachedRaw.empty())
            return _cachedRaw;
        
        /* 
            base HeaderWithBodyMessage serialize:
                fill raw and send it.
        */
        return _cachedRaw; 
    }

    TypeCode type() const noexcept { return _header.type; }
    Header::BaseInfo baseInfo() const noexcept { return _header.info; }
    Body body() const noexcept { return _body; }

protected:
    Header _header;
    Body    _body;

    std::vector<std::byte> _cachedRaw;
};

class BlobMessage
    : public HeaderWithBodyMessage<TypeCode::WithBlob>
{
public:
    void deserialize(std::vector<std::byte> const& raw) override 
    { 
        /* base HeaderWithBodyMessage deserialize */
        if (_header.info.blobInfo.haveBlob)
        {
            // deserialize base message info
            HeaderWithBodyMessage::deserialize({raw.begin(), std::next(raw.begin(), _header.info.blobInfo.blobStartPosition - 1)});
            
            // deserialize blob
            if (not _blob.empty())
                _blob.clear();
            
            _blob.assign(std::next(raw.begin(), _header.info.blobInfo.blobStartPosition), 
                std::next(raw.begin(), _header.info.blobInfo.blobEndPosition));
        }
    }
    
    std::vector<std::byte> serialize() override { 
        if (not _cachedRaw.empty())
            return _cachedRaw;

        /* 
            base HeaderWithBodyMessage serialize:
                fill raw and send it.
        */
        return _cachedRaw; 
    }

    std::pair<Header::BlobInfo, std::vector<std::byte>> blob() const noexcept { return {}; }

protected:
    std::vector<std::byte> _blob;
};

namespace message
{

using PrivateMessage    = HeaderWithBodyMessage<TypeCode::One2One>;
using PublicMessage     = HeaderWithBodyMessage<TypeCode::One2All>;
using GroupMessage      = HeaderWithBodyMessage<TypeCode::One2Group>;
using OnlineListMessage = HeaderWithBodyMessage<TypeCode::OnlineInfo>;

using PingMessage       = HeaderOnlyMessage<TypeCode::Ping>;
using ConnectMessage    = HeaderOnlyMessage<TypeCode::Connect>;
using DisconnectMessage = HeaderOnlyMessage<TypeCode::Disconnect>;

}
