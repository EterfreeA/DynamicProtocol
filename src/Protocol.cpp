#include "Core.hpp"
#include "Protocol.hpp"

#include <iostream>

PROTOCOL_SPACE_BEGIN

SizeType sizeOfType(FIELD_TYPE _type) noexcept
{
	switch (_type)
	{
	case FIELD_TYPE::UINT8:
		return sizeOfValue<std::uint8_t>();
	case FIELD_TYPE::UINT16:
		return sizeOfValue<std::uint16_t>();
	case FIELD_TYPE::UINT32:
		return sizeOfValue<std::uint32_t>();
	case FIELD_TYPE::UINT64:
		return sizeOfValue<std::uint64_t>();
	case FIELD_TYPE::INT8:
		return sizeOfValue<std::int8_t>();
	case FIELD_TYPE::INT16:
		return sizeOfValue<std::int16_t>();
	case FIELD_TYPE::INT32:
		return sizeOfValue<std::int32_t>();
	case FIELD_TYPE::INT64:
		return sizeOfValue<std::int64_t>();
	case FIELD_TYPE::BOOLEAN:
		return sizeOfValue<bool>();
	case FIELD_TYPE::FLOAT:
		return sizeOfValue<float>();
	case FIELD_TYPE::DOUBLE:
		return sizeOfValue<double>();
	default:
		std::cerr << "Invalid type[" << static_cast<FieldType>(_type) << ']' \
			<< std::source_location::current() << std::endl;
		break;
	}
	return 0;
}

SizeType sizeOfValue(const char* _data, \
	std::size_t _size, SizePool* _pool)
{
	USING_BYTE_ORDER_SPACE;

	if (_data == nullptr && _size != 0 \
		|| _size > MAX_SIZE) return 0;

	auto size = static_cast<SizeType>(_size);
	constexpr auto SIZE = sizeOfValue<decltype(size)>();
	constexpr auto RESIDUE_SIZE = MAX_SIZE - SIZE;
	if (size > RESIDUE_SIZE) return 0;

	size += SIZE;
	if (_pool != nullptr)
		_pool->emplace(_data, size);
	return size;
}

PROTOCOL_SPACE_END
