#pragma once

#include <cstdint>

#define BYTE_ORDER_SPACE_BEGIN namespace ByteOrder {
#define BYTE_ORDER_SPACE_END }
#define USING_BYTE_ORDER_SPACE using namespace ByteOrder;

BYTE_ORDER_SPACE_BEGIN

// 小端模式
inline bool little() noexcept
{
	constexpr auto data = static_cast<std::uint16_t>(1);
	return *reinterpret_cast<const char*>(&data) != '\0';
}

inline std::uint8_t hton(std::uint8_t _data) noexcept
{
	return _data;
}

std::uint16_t hton(std::uint16_t _data);

std::uint32_t hton(std::uint32_t _data);

std::uint64_t hton(std::uint64_t _data);

inline std::uint8_t hton(std::int8_t _data) noexcept
{
	return static_cast<std::uint8_t>(_data);
}

inline std::uint16_t hton(std::int16_t _data)
{
	return hton(static_cast<std::uint16_t>(_data));
}

inline std::uint32_t hton(std::int32_t _data)
{
	return hton(static_cast<std::uint32_t>(_data));
}

inline std::uint64_t hton(std::int64_t _data)
{
	return hton(static_cast<std::uint64_t>(_data));
}

inline std::uint8_t hton(bool _data) noexcept
{
	return static_cast<std::uint8_t>(_data);
}

std::uint32_t hton(float _data);

std::uint64_t hton(double _data);

template <typename _Source, typename _Target>
_Target ntoh(_Source _data);

template <>
inline std::uint8_t ntoh(std::uint8_t _data) noexcept
{
	return _data;
}

template <>
std::uint16_t ntoh(std::uint16_t _data);

template <>
std::uint32_t ntoh(std::uint32_t _data);

template <>
std::uint64_t ntoh(std::uint64_t _data);

template <>
inline std::int8_t ntoh(std::uint8_t _data) noexcept
{
	return static_cast<std::int8_t>(_data);
}

template <>
inline std::int16_t ntoh(std::uint16_t _data)
{
	auto data = ntoh<std::uint16_t, std::uint16_t>(_data);
	return static_cast<std::int16_t>(data);
}

template <>
inline std::int32_t ntoh(std::uint32_t _data)
{
	auto data = ntoh<std::uint32_t, std::uint32_t>(_data);
	return static_cast<std::int32_t>(data);
}

template <>
inline std::int64_t ntoh(std::uint64_t _data)
{
	auto data = ntoh<std::uint64_t, std::uint64_t>(_data);
	return static_cast<std::int64_t>(data);
}

template <>
inline bool ntoh(std::uint8_t _data) noexcept
{
	return static_cast<bool>(_data);
}

template <>
float ntoh(std::uint32_t _data);

template <>
double ntoh(std::uint64_t _data);

BYTE_ORDER_SPACE_END
