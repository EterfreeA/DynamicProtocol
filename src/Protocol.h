#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>

#define PROTOCOL_SPACE_BEGIN namespace Protocol {
#define PROTOCOL_SPACE_END }
#define USING_PROTOCOL_SPACE using namespace Protocol;

PROTOCOL_SPACE_BEGIN

using FieldType = std::uint8_t;
enum class FIELD_TYPE : FieldType
{
	INVALID, \
	UINT8, UINT16, UINT32, UINT64, \
	INT8, INT16, INT32, INT64, \
	BOOLEAN, FLOAT, DOUBLE, \
	BASE, \
	STRING, TABLE, \
	VECTOR, MAPPING, \
	COMPLEX
};

using NumberType = std::uint16_t;
using SizeType = std::uint32_t;

using SizePool = std::unordered_map<const void*, std::size_t>;

inline constexpr NumberType MAX_NUMBER = UINT16_MAX;
inline constexpr SizeType MAX_SIZE = UINT32_MAX;

inline constexpr auto typeOfValue(const std::uint8_t&) noexcept
{
	return FIELD_TYPE::UINT8;
}

inline constexpr auto typeOfValue(const std::uint16_t&) noexcept
{
	return FIELD_TYPE::UINT16;
}

inline constexpr auto typeOfValue(const std::uint32_t&) noexcept
{
	return FIELD_TYPE::UINT32;
}

inline constexpr auto typeOfValue(const std::uint64_t&) noexcept
{
	return FIELD_TYPE::UINT64;
}

inline constexpr auto typeOfValue(const std::int8_t&) noexcept
{
	return FIELD_TYPE::INT8;
}

inline constexpr auto typeOfValue(const std::int16_t&) noexcept
{
	return FIELD_TYPE::INT16;
}

inline constexpr auto typeOfValue(const std::int32_t&) noexcept
{
	return FIELD_TYPE::INT32;
}

inline constexpr auto typeOfValue(const std::int64_t&) noexcept
{
	return FIELD_TYPE::INT64;
}

inline constexpr auto typeOfValue(const bool&) noexcept
{
	return FIELD_TYPE::BOOLEAN;
}

inline constexpr auto typeOfValue(const float&) noexcept
{
	return FIELD_TYPE::FLOAT;
}

inline constexpr auto typeOfValue(const double&) noexcept
{
	return FIELD_TYPE::DOUBLE;
}

inline constexpr auto typeOfValue(const char*) noexcept
{
	return FIELD_TYPE::STRING;
}

inline constexpr auto typeOfValue(const std::string&) noexcept
{
	return FIELD_TYPE::STRING;
}

SizeType sizeOfType(FIELD_TYPE _type) noexcept;

SizeType sizeOfValue(const char* _data, \
	std::size_t _size, SizePool* _pool = nullptr);

inline auto sizeOfValue(const char* _string, SizePool* _pool = nullptr)
{
	return sizeOfValue(_string, std::strlen(_string), _pool);
}

PROTOCOL_SPACE_END
