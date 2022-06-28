#pragma once

#include "Table.h"
#include "ByteOrder.h"

PROTOCOL_SPACE_BEGIN

template <typename _Element, typename _Container>
std::size_t Table::size(const _Container& _container, SizePool* _pool)
{
	auto size = sizeOfValue(_container, _pool);
	if (size <= 0) return 0;

	constexpr auto EXTRA_SIZE = sizeOfValue<NumberType>() \
		+ sizeOfValue<FieldType>();

	static_assert(EXTRA_SIZE <= MAX_SIZE);
	constexpr auto RESIDUE_SIZE = MAX_SIZE - EXTRA_SIZE;

	return size <= RESIDUE_SIZE ? EXTRA_SIZE + size : 0;
}

template <typename _Type>
constexpr std::size_t Table::size() noexcept
{
	constexpr auto SIZE = sizeOfValue<NumberType>() \
		+ sizeOfValue<FieldType>() + sizeOfValue<_Type>();

	static_assert(SIZE <= MAX_SIZE);
	return SIZE;
}

template <typename _Type>
bool Table::fetch(_Type& _value, const char* _data)
{
	USING_BYTE_ORDER_SPACE;

	using Type = decltype(hton(_value));
	auto value = *reinterpret_cast<const Type*>(_data);
	_value = ntoh<Type, _Type>(value);
	return true;
}

template <typename _Source, typename _Target>
bool Table::convert(_Target& _value, const char* _data)
{
	_Source value = 0;
	if (!fetch(value, _data)) return false;

	_value = static_cast<_Target>(value);
	return true;
}

template <typename _Container>
bool Table::getContainer(_Container& _container, \
	const char* _data, SizeType _size, \
	FIELD_TYPE _type, SizeType _capacity)
{
	reserve(_container, _capacity);

	constexpr auto SIZE = sizeOfValue<decltype(_size)>();
	for (decltype(_capacity) index = 0; \
		index < _capacity; ++index)
	{
		if (_size < SIZE) return false;

		decltype(_size) size = 0;
		if (!fetch(size, _data)) return false;

		if ((_size -= SIZE) < size) return false;

		using Element = _Container::value_type;
		Element element;
		if (!get(element, _data += SIZE, size))
			return false;

		push(_container, std::move(element));

		_size -= size;
		_data += size;
	}
	return true;
}

template <typename _Container>
bool Table::getBase(_Container& _container, \
	const char* _data, SizeType _size, \
	FIELD_TYPE _type, SizeType _capacity)
{
	auto size = sizeOfType(_type);
	if (size <= 0 || _size / size < _capacity)
		return false;

	reserve(_container, _capacity);
	for (decltype(_capacity) index = 0; \
		index < _capacity; ++index, _data += size)
	{
		using Element = _Container::value_type;
		Element element;
		if (!get(element, _data, \
			static_cast<decltype(size)>(_type)))
			return false;

		push(_container, std::move(element));
	}
	return true;
}

template <typename _Container>
bool Table::getVector(_Container& _container, \
	const char* _data, SizeType _size)
{
	constexpr auto TYPE_SIZE = sizeOfValue<FieldType>();
	constexpr auto FIELD_SIZE = sizeOfValue<decltype(_size)>();
	constexpr auto EXTRA_SIZE = TYPE_SIZE + FIELD_SIZE;
	static_assert(EXTRA_SIZE <= MAX_SIZE);

	if (_size < EXTRA_SIZE) return false;

	auto sourceType = FIELD_TYPE::INVALID;
	if (!fetch(sourceType, _data)) return false;

	using Element = _Container::value_type;
	auto& element = reinterpret_cast<const Element&>(_container);
	auto targetType = typeOfValue(element);
	if (!similar(targetType, sourceType))
		return false;

	decltype(_size) size = 0;
	return fetch(size, _data += TYPE_SIZE) \
		&& getContainer(_container, _data += FIELD_SIZE, \
			_size - EXTRA_SIZE, sourceType, size);
}

template <typename _Element>
bool Table::getElement(_Element& _element, \
	const char*& _data, SizeType& _size, FIELD_TYPE _type)
{
	if (_type <= FIELD_TYPE::BASE)
	{
		auto size = sizeOfType(_type);
		if (size <= 0 || _size < size) return false;

		if (!get(_element, _data, \
			static_cast<decltype(size)>(_type)))
			return false;

		_size -= size;
		_data += size;
		return true;
	}

	constexpr auto FIELD_SIZE = sizeOfValue<SizeType>();
	if (_size < FIELD_SIZE) return false;

	SizeType size = 0;
	if (!fetch(size, _data)) return false;

	if (_size - FIELD_SIZE < size) return false;

	if (!get(_element, _data + FIELD_SIZE, size))
		return false;

	size += FIELD_SIZE;
	_size -= size;
	_data += size;
	return true;
}

template <typename _Container>
bool Table::getMapping(_Container& _container, \
	const char* _data, SizeType _size)
{
	constexpr auto TYPE_SIZE = sizeOfValue<FieldType>();
	constexpr auto FIELD_SIZE = sizeOfValue<decltype(_size)>();
	constexpr auto EXTRA_SIZE = TYPE_SIZE * 2 + FIELD_SIZE;
	static_assert(EXTRA_SIZE <= MAX_SIZE);

	if (_size < EXTRA_SIZE) return false;

	auto keyType = FIELD_TYPE::INVALID;
	if (!fetch(keyType, _data)) return false;

	using KeyType = _Container::key_type;
	auto& key = reinterpret_cast<const KeyType&>(_container);
	if (!similar(typeOfValue(key), keyType))
		return false;

	auto valueType = FIELD_TYPE::INVALID;
	if (!fetch(valueType, _data += TYPE_SIZE))
		return false;

	using ValueType = _Container::mapped_type;
	auto& value = reinterpret_cast<const ValueType&>(_container);
	if (!similar(typeOfValue(value), valueType))
		return false;

	decltype(_size) capacity = 0;
	if (!fetch(capacity, _data += TYPE_SIZE))
		return false;

	_data += FIELD_SIZE;
	_size -= EXTRA_SIZE;

	for (decltype(capacity) index = 0; \
		index < capacity; ++index)
	{
		KeyType key;
		if (!getElement(key, _data, _size, keyType))
			return false;

		ValueType value;
		if (!getElement(value, _data, _size, valueType))
			return false;

		_container.emplace(std::move(key), std::move(value));
	}
	return true;
}

template <typename _Type>
static bool Table::get(_Type& _value, const char* _data, \
	SizeType _size)
{
	auto type = static_cast<FIELD_TYPE>(_size);
	switch (type)
	{
	case FIELD_TYPE::UINT8:
		return convert<std::uint8_t>(_value, _data);
	case FIELD_TYPE::UINT16:
		return convert<std::uint16_t>(_value, _data);
	case FIELD_TYPE::UINT32:
		return convert<std::uint32_t>(_value, _data);
	case FIELD_TYPE::UINT64:
		return convert<std::uint64_t>(_value, _data);
	case FIELD_TYPE::INT8:
		return convert<std::int8_t>(_value, _data);
	case FIELD_TYPE::INT16:
		return convert<std::int16_t>(_value, _data);
	case FIELD_TYPE::INT32:
		return convert<std::int32_t>(_value, _data);
	case FIELD_TYPE::INT64:
		return convert<std::int64_t>(_value, _data);
	case FIELD_TYPE::BOOLEAN:
		return convert<bool>(_value, _data);
	case FIELD_TYPE::FLOAT:
		return convert<float>(_value, _data);
	case FIELD_TYPE::DOUBLE:
		return convert<double>(_value, _data);
	default:
		break;
	}
	return false;
}

template <typename _Container>
Table& Table::setVector(const _Container& _container, SizePool* _pool)
{
	SizeType size = 0;
	bool found = false;
	if (_pool != nullptr)
		if (auto iterator = _pool->find(&_container); \
			iterator != _pool->end())
		{
			size = static_cast<decltype(size)>(iterator->second);
			found = true;
		}

	if (!found) size = sizeOfValue(_container, _pool);

	constexpr auto SIZE = sizeOfValue<decltype(size)>();
	set(size - SIZE);

	using Element = _Container::value_type;
	set(typeOfValue(reinterpret_cast<const Element&>(_container)));

	set(static_cast<decltype(size)>(_container.size()));

	for (const auto& element : _container)
		set(element, _pool);
	return *this;
}

template <typename _Container>
Table& Table::setMapping(const _Container& _container, SizePool* _pool)
{
	SizeType size = 0;
	bool found = false;
	if (_pool != nullptr)
		if (auto iterator = _pool->find(&_container); \
			iterator != _pool->end())
		{
			size = static_cast<decltype(size)>(iterator->second);
			found = true;
		}

	if (!found) size = sizeOfValue(_container, _pool);

	constexpr auto SIZE = sizeOfValue<decltype(size)>();
	set(size - SIZE);

	using KeyType = _Container::key_type;
	set(typeOfValue(reinterpret_cast<const KeyType&>(_container)));

	using ValueType = _Container::mapped_type;
	set(typeOfValue(reinterpret_cast<const ValueType&>(_container)));

	set(static_cast<decltype(size)>(_container.size()));

	for (auto& [key, value] : _container)
		set(key, _pool).set(value, _pool);
	return *this;
}

template <typename _Type>
Table& Table::set(_Type _value, SizePool* _pool)
{
	USING_BYTE_ORDER_SPACE;

	auto value = hton(_value);
	_stream.append(reinterpret_cast<const char*>(&value), sizeof value);
	return *this;
}

template <typename _Container>
bool Table::setContainer(NumberType _number, \
	const _Container& _container)
{
	SizePool pool;
	auto totalSize = size(_container, &pool);
	if (totalSize <= 0) return false;

	auto streamSize = _stream.size();
	if (streamSize > MAX_SIZE - totalSize)
		return false;

	_stream.reserve(streamSize + totalSize);
	set(_number).set(typeOfValue(_container));
	set(_container, &pool);
	return true;
}

template <typename _Container>
bool Table::getContainer(NumberType _number, \
	_Container& _container) const
{
	parse();

	auto data = get(_number, typeOfValue(_container));
	if (data == nullptr) return false;

	SizeType size = 0;
	constexpr auto SIZE = sizeOfValue<decltype(size)>();
	return fetch(size, data) ? \
		get(_container, data + SIZE, size) : false;
}

template <typename _Type>
bool Table::set(NumberType _number, _Type _value)
{
	constexpr auto SIZE = size<_Type>();
	constexpr auto RESIDUE_SIZE = MAX_SIZE - SIZE;

	auto size = _stream.size();
	if (size > RESIDUE_SIZE) return false;

	_stream.reserve(size + SIZE);
	set(_number).set(typeOfValue(_value)).set(_value);
	return true;
}

template <typename _Type>
bool Table::get(NumberType _number, _Type& _value) const
{
	parse();

	auto iterator = _mapping.find(_number);
	if (iterator == _mapping.end()) return false;

	constexpr auto SIZE = sizeOfValue<FieldType>();

	auto data = _stream.data() + iterator->second;
	auto type = FIELD_TYPE::INVALID;
	return fetch(type, data) && similar(typeOfValue(_value), type) ? \
		get(_value, data += SIZE, static_cast<SizeType>(type)) : false;
}

PROTOCOL_SPACE_END
