#pragma once

#include <string>
#include <stdexcept>

namespace types::message
{

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

namespace strings
{
    constexpr const char* Header = "Header";
    constexpr const char* Body = "Body";
    constexpr const char* TypeCode = "TypeCode";
    constexpr const char* BlobType = "BlobType";
    constexpr const char* BlobInfo = "BlobInfo";

    constexpr const char* Data = "Data";

    constexpr const char* BaseInfo = "BaseInfo";
    constexpr const char* From = "From";
    constexpr const char* To = "To";
    constexpr const char* Date = "Date";

    constexpr const char* HaveBlob = "HaveBlob";
    constexpr const char* BlobStartPosition = "BlobStartPosition";
    constexpr const char* BlobEndPosition = "BlobEndPosition";

    constexpr const char* One2One = "One2One";
    constexpr const char* One2All = "One2All";
    constexpr const char* One2Group = "One2Group";
    constexpr const char* Ping = "Ping";
    constexpr const char* Connect = "Connect";
    constexpr const char* Disconnect = "Disconnect";
    constexpr const char* OnlineInfo = "OnlineInfo";
    constexpr const char* WithBlob = "WithBlob";

    constexpr const char* Unknown = "Unknown";
    constexpr const char* PNG = "PNG";
    constexpr const char* JPEG = "JPEG";
    constexpr const char* GIF = "GIF";
    constexpr const char* WAV = "WAV";
    constexpr const char* MP4 = "MP4";
    constexpr const char* TXT = "TXT";

}

inline std::string blobTypeToStr(BlobType type)
{
    switch(type)
    {
        case BlobType::Unknown: return strings::Unknown;
        case BlobType::PNG: return strings::PNG;
        case BlobType::JPEG: return strings::JPEG;
        case BlobType::GIF: return strings::GIF;
        case BlobType::WAV: return strings::WAV;
        case BlobType::MP4: return strings::MP4;
        case BlobType::TXT: return strings::TXT;
        default: break;
    }

    throw std::runtime_error("Cant convert BlobType to str");
}


inline BlobType strToBlobType(std::string const& type)
{
    if (type == strings::Unknown) return BlobType::Unknown;
    if (type == strings::PNG) return BlobType::PNG;
    if (type == strings::JPEG) return BlobType::JPEG;
    if (type == strings::GIF) return BlobType::GIF;
    if (type == strings::WAV) return BlobType::WAV;
    if (type == strings::MP4) return BlobType::MP4;
    if (type == strings::TXT) return BlobType::TXT;

    throw std::runtime_error("Cant convert str to BlobType");
}

inline std::string typeCodeToStr(TypeCode code)
{
    switch(code)
    {
        case TypeCode::One2One: return strings::One2One;
        case TypeCode::One2All: return strings::One2All;
        case TypeCode::One2Group: return strings::One2Group;
        case TypeCode::Ping: return strings::Ping;
        case TypeCode::Connect: return strings::Connect;
        case TypeCode::Disconnect: return strings::Disconnect;
        case TypeCode::OnlineInfo: return strings::OnlineInfo;
        case TypeCode::WithBlob:  return strings::WithBlob;
        default: break;
    }

    throw std::runtime_error("Cant convert TypeCode to str");
}

inline TypeCode strToTypeCode(std::string const& code)
{
    if (code == strings::One2One) return TypeCode::One2One;
    if (code == strings::One2All) return TypeCode::One2All;
    if (code == strings::One2Group) return TypeCode::One2Group;
    if (code == strings::Ping) return TypeCode::Ping;
    if (code == strings::Connect) return TypeCode::Connect;
    if (code == strings::Disconnect) return TypeCode::Disconnect;
    if (code == strings::OnlineInfo) return TypeCode::OnlineInfo;
    if (code == strings::WithBlob) return TypeCode::WithBlob;

    throw std::runtime_error("Cant convert str to TypeCode");
}

}