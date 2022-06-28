#include "ByteOrder.h"

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#elif defined(linux) || defined(__linux) || defined(__linux__)
#define LINUX
#include <arpa/inet.h>

#else
//#error "This platform is currently not supported."
#error "Currently only supports Windows and Linux!"
#endif

#include <algorithm>

BYTE_ORDER_SPACE_BEGIN

//template <typename _Type>
//static _Type swap(_Type _data) noexcept
//{
//	constexpr auto size = sizeof(_Type) - 1;
//
//	auto low = reinterpret_cast<char*>(&_data);
//	auto high = low + size;
//
//	while (low < high)
//	{
//		auto byte = *low;
//		*low++ = *high;
//		*high-- = byte;
//	}
//	return _data;
//}

template <typename _Type>
static _Type swap(_Type _data)
{
	auto begin = reinterpret_cast<char*>(&_data);
	auto end = begin + sizeof _data;
	std::reverse(begin, end);
	return _data;
}

std::uint16_t hton(std::uint16_t _data)
{
#ifdef _WIN32
	static_assert(sizeof _data == sizeof(u_short), \
		"The size of _data is not equal to the size of u_short");
	return htons(_data);

#elif defined LINUX
	return htons(_data);
#endif
}

std::uint32_t hton(std::uint32_t _data)
{
#ifdef _WIN32
	static_assert(sizeof _data == sizeof(u_long), \
		"The size of _data is not equal to the size of u_long");
	return htonl(_data);

#elif defined LINUX
	return htonl(_data);
#endif
}

std::uint64_t hton(std::uint64_t _data)
{
#ifdef _WIN32
	return htonll(_data);

#elif defined LINUX
	return little() ? swap(_data) : _data;
#endif
}

std::uint32_t hton(float _data)
{
#ifdef _WIN32
	return htonf(_data);

#elif defined LINUX
	auto data = *reinterpret_cast<std::uint32_t*>(&_data);
	return htonl(data);
#endif
}

std::uint64_t hton(double _data)
{
#ifdef _WIN32
	return htond(_data);

#elif defined LINUX
	if (little())
		_data = swap(_data);
	return *reinterpret_cast<std::uint64_t*>(&_data);
#endif
}

template <typename _Source, typename _Target>
_Target ntoh(_Source _data)
{
	auto data = static_cast<_Target>(_data);
	return little() ? swap(data) : data;
}

template <>
std::uint16_t ntoh(std::uint16_t _data)
{
#ifdef _WIN32
	static_assert(sizeof _data == sizeof(u_short), \
		"The size of _data is not equal to the size of u_short");
	return ntohs(_data);

#elif defined LINUX
	return ntohs(_data);
#endif
}

template <>
std::uint32_t ntoh(std::uint32_t _data)
{
#ifdef _WIN32
	static_assert(sizeof _data == sizeof(u_long), \
		"The size of _data is not equal to the size of u_long");
	return ntohl(_data);

#elif defined LINUX
	return ntohl(_data);
#endif
}

template <>
std::uint64_t ntoh(std::uint64_t _data)
{
#ifdef _WIN32
	return ntohll(_data);

#elif defined LINUX
	return little() ? swap(_data) : _data;
#endif
}

template <>
float ntoh(std::uint32_t _data)
{
#ifdef _WIN32
	return ntohf(_data);

#elif defined LINUX
	_data = ntohl(_data);
	return *reinterpret_cast<float*>(&_data);
#endif
}

template <>
double ntoh(std::uint64_t _data)
{
#ifdef _WIN32
	return ntohd(_data);

#elif defined LINUX
	if (little())
		_data = swap(_data);
	return *reinterpret_cast<double*>(&_data);
#endif
}

BYTE_ORDER_SPACE_END
