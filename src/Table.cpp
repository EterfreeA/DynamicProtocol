#include "Table.h"
#include "ByteOrder.h"

#include <cfloat>

PROTOCOL_SPACE_BEGIN

USING_BYTE_ORDER_SPACE

std::size_t Table::size(const char* _data, std::size_t _size, \
	SizePool* _pool)
{
	constexpr auto EXTRA_SIZE = sizeOfValue<NumberType>() \
		+ sizeOfValue<FieldType>();

	static_assert(EXTRA_SIZE <= MAX_SIZE);
	constexpr auto RESIDUE_SIZE = MAX_SIZE - EXTRA_SIZE;

	auto size = sizeOfValue(_data, _size, _pool);
	return size > 0 && size <= RESIDUE_SIZE ? \
		EXTRA_SIZE + size : 0;
}

bool Table::similar(FIELD_TYPE _left, FIELD_TYPE _right) noexcept
{
	return _left == _right \
		|| _left > FIELD_TYPE::INVALID && _left < FIELD_TYPE::BASE \
		&& _right > FIELD_TYPE::INVALID && _right < FIELD_TYPE::BASE;
}

template <typename _Type>
std::optional<SizeType> Table::next(SizeType _size, \
	SizeType _offset, const _Type& _value) noexcept
{
	constexpr auto VALUE_SIZE = sizeOfValue<_Type>();
	if (_size - _offset < VALUE_SIZE)
		return std::nullopt;

	constexpr auto TYPE_SIZE = sizeOfValue<FieldType>();
	constexpr auto SIZE = TYPE_SIZE + VALUE_SIZE;
	static_assert(SIZE <= MAX_SIZE);
	return SIZE;
}

template <>
bool Table::fetch(FIELD_TYPE& _type, \
	const char* _data)
{
	FieldType type = 0;
	if (!fetch(type, _data))
		return false;

	_type = static_cast<FIELD_TYPE>(type);
	return true;
}

Table& Table::set(const char* _data, \
	std::size_t _size)
{
	set(static_cast<SizeType>(_size));
	_stream.append(_data, _size);
	return *this;
}

Table& Table::set(const char* _string, SizePool* _pool)
{
	std::size_t size = 0;
	bool found = false;
	if (_pool != nullptr)
		if (auto iterator = _pool->find(_string); \
			iterator != _pool->end())
		{
			constexpr auto SIZE = sizeOfValue<SizeType>();
			size = iterator->second - SIZE;
			found = true;
		}

	if (!found) size = std::strlen(_string);
	return set(_string, size);
}

bool Table::set(NumberType _number, FIELD_TYPE _type, \
	const char* _data, std::size_t _size)
{
	auto totalSize = size(_data, _size);
	if (totalSize <= 0) return false;

	auto streamSize = _stream.size();
	if (streamSize > MAX_SIZE - totalSize)
		return false;

	_stream.reserve(streamSize + totalSize);
	set(_number).set(_type).set(_data, _size);
	return true;
}

std::optional<SizeType> Table::next(SizeType _size, \
	SizeType _offset) const
{
	constexpr auto TYPE_SIZE = sizeOfValue<FieldType>();
	if (_size - _offset < TYPE_SIZE) return std::nullopt;

	FIELD_TYPE type = FIELD_TYPE::INVALID;
	if (!fetch(type, _stream.data() + _offset))
		return std::nullopt;

	_offset += TYPE_SIZE;
	switch (type)
	{
	case FIELD_TYPE::UINT8:
		return next<std::uint8_t>(_size, _offset, 0);
	case FIELD_TYPE::UINT16:
		return next<std::uint16_t>(_size, _offset, 0);
	case FIELD_TYPE::UINT32:
		return next<std::uint32_t>(_size, _offset, 0);
	case FIELD_TYPE::UINT64:
		return next<std::uint64_t>(_size, _offset, 0);
	case FIELD_TYPE::INT8:
		return next<std::int8_t>(_size, _offset, 0);
	case FIELD_TYPE::INT16:
		return next<std::int16_t>(_size, _offset, 0);
	case FIELD_TYPE::INT32:
		return next<std::int32_t>(_size, _offset, 0);
	case FIELD_TYPE::INT64:
		return next<std::int64_t>(_size, _offset, 0);
	case FIELD_TYPE::BOOLEAN:
		return next(_size, _offset, false);
	case FIELD_TYPE::FLOAT:
		return next(_size, _offset, FLT_MIN);
	case FIELD_TYPE::DOUBLE:
		return next(_size, _offset, DBL_MIN);
	case FIELD_TYPE::STRING:
	case FIELD_TYPE::TABLE:
	case FIELD_TYPE::VECTOR:
	case FIELD_TYPE::MAPPING:
		if (constexpr auto FIELD_SIZE = sizeOfValue<SizeType>(); \
			_size - _offset >= FIELD_SIZE)
		{
			if (SizeType size = 0; \
				fetch(size, _stream.data() + _offset))
			{
				_offset += FIELD_SIZE;
				if (_size - _offset >= size)
					return TYPE_SIZE + FIELD_SIZE + size;
			}
		}
		break;
	default:
		break;
	}
	return std::nullopt;
}

void Table::parse() const
{
	if (_offset >= MAX_SIZE \
		|| _offset >= _stream.size())
		return;

	SizeType size = MAX_SIZE;
	if (_stream.size() < MAX_SIZE)
		size = static_cast<decltype(size)>(_stream.size());

	auto offset = static_cast<decltype(size)>(_offset);
	while (offset < size)
	{
		constexpr auto SIZE = sizeOfValue<NumberType>();
		if (size - offset < SIZE)
			break;

		NumberType number = 0;
		if (!fetch(number, _stream.data() + offset))
			break;

		auto optional = next(size, offset += SIZE);
		if (!optional) break;

		_mapping.emplace(number, offset);
		offset += optional.value();
	}
	_offset = offset;
}

const char* Table::get(NumberType _number, \
	FIELD_TYPE _type) const
{
	auto iterator = _mapping.find(_number);
	if (iterator == _mapping.end())
		return nullptr;

	constexpr auto SIZE = sizeOfValue<FieldType>();

	auto data = _stream.data() + iterator->second;
	auto type = FIELD_TYPE::INVALID;
	return fetch(type, data) \
		&& similar(type, _type) ? \
		data + SIZE : nullptr;
}

Table& Table::assign(const std::string& _stream)
{
	this->_stream = _stream;
	_offset = 0;
	_mapping.clear();
	return *this;
}

Table& Table::assign(std::string&& _stream) noexcept
{
	this->_stream = std::move(_stream);
	_offset = 0;
	_mapping.clear();
	return *this;
}

Table& Table::assign(const char* _data, \
	std::size_t _size)
{
	_stream.assign(_data, _size);
	_offset = 0;
	_mapping.clear();
	return *this;
}

void Table::clear() noexcept
{
	_stream.clear();
	_offset = 0;
	_mapping.clear();
}

template <>
bool Table::get(NumberType _number, std::string& _string) const
{
	parse();

	auto data = get(_number, typeOfValue(_string));
	if (data == nullptr) return false;

	SizeType size = 0;
	constexpr auto SIZE = sizeOfValue<decltype(size)>();
	return fetch(size, data) ? \
		get(_string, data + SIZE, size) : false;
}

template <>
bool Table::get(NumberType _number, Table& _table) const
{
	parse();

	auto data = get(_number, typeOfValue(_table));
	if (data == nullptr) return false;

	SizeType size = 0;
	constexpr auto SIZE = sizeOfValue<decltype(size)>();
	return fetch(size, data) ? \
		get(_table, data + SIZE, size) : false;
}

PROTOCOL_SPACE_END
