#pragma once

#include <type_traits>
#include <utility>
#include <cstddef>
#include <optional>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include "Protocol.hpp"

#define GET_CONTAINER(Container, get) \
template <> \
static bool getContainer(Container<std::uint8_t>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<std::uint16_t>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<std::uint32_t>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<std::uint64_t>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<std::int8_t>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<std::int16_t>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<std::int32_t>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<std::int64_t>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<bool>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<float>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
} \
template <> \
static bool getContainer(Container<double>& _container, \
	const char* _data, SizeType _size, FIELD_TYPE _type, SizeType _capacity) \
{ \
	return get(_container, _data, _size, _type, _capacity); \
}

PROTOCOL_SPACE_BEGIN

class Table;
inline constexpr auto typeOfValue(const Table&) noexcept
{
	return FIELD_TYPE::TABLE;
}

class Table
{
private:
	using MappingType = std::unordered_map<NumberType, SizeType>;

private:
	std::string _stream;
	mutable std::size_t _offset;
	mutable MappingType _mapping;

private:
	static std::size_t size(const char* _data, \
		std::size_t _size, SizePool* _pool = nullptr);

	template <typename _Element, typename _Container>
	static std::size_t size(const _Container& _container, \
		SizePool* _pool = nullptr);

public:
	template <typename _Type>
	static constexpr std::size_t size() noexcept;

	template <typename _Type>
	static std::size_t size(const _Type& _value, \
		SizePool* _pool = nullptr) noexcept
	{
		return size<_Type>();
	}

	static auto size(const char* _string, SizePool* _pool = nullptr)
	{
		return size(_string, std::strlen(_string), _pool);
	}

	template <>
	static std::size_t size(const std::string& _string, SizePool* _pool)
	{
		return size(_string.data(), _string.size(), _pool);
	}

	template <>
	static std::size_t size(const Table& _table, SizePool* _pool)
	{
		return size(_table.data(), _table.size(), _pool);
	}

	template <typename _Element>
	static auto size(const std::vector<_Element>& _vector, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_vector)>>;
		return size<_Element, Container>(_vector, _pool);
	}

	template <typename _Element>
	static auto size(const std::deque<_Element>& _deque, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_deque)>>;
		return size<_Element, Container>(_deque, _pool);
	}

	template <typename _Element>
	static auto size(const std::list<_Element>& _list, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_list)>>;
		return size<_Element, Container>(_list, _pool);
	}

	template <typename _Element>
	static auto size(const std::set<_Element>& _set, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_set)>>;
		return size<_Element, Container>(_set, _pool);
	}

	template <typename _Element>
	static auto size(const std::multiset<_Element>& _set, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_set)>>;
		return size<_Element, Container>(_set, _pool);
	}

	template <typename _Element>
	static auto size(const std::unordered_set<_Element>& _set, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_set)>>;
		return size<_Element, Container>(_set, _pool);
	}

	template <typename _Element>
	static auto size(const std::unordered_multiset<_Element>& _set, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_set)>>;
		return size<_Element, Container>(_set, _pool);
	}

	template <typename _KeyType, typename _ValueType>
	static auto size(const std::map<_KeyType, _ValueType>& _mapping, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_mapping)>>;
		using Element = Container::value_type;
		return size<Element, Container>(_mapping, _pool);
	}

	template <typename _KeyType, typename _ValueType>
	static auto size(const std::multimap<_KeyType, _ValueType>& _mapping, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_mapping)>>;
		using Element = Container::value_type;
		return size<Element, Container>(_mapping, _pool);
	}

	template <typename _KeyType, typename _ValueType>
	static auto size(const std::unordered_map<_KeyType, _ValueType>& _mapping, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_mapping)>>;
		using Element = Container::value_type;
		return size<Element, Container>(_mapping, _pool);
	}

	template <typename _KeyType, typename _ValueType>
	static auto size(const std::unordered_multimap<_KeyType, _ValueType>& _mapping, \
		SizePool* _pool = nullptr)
	{
		using Container = std::remove_const_t<std::remove_reference_t<decltype(_mapping)>>;
		using Element = Container::value_type;
		return size<Element, Container>(_mapping, _pool);
	}

private:
	static bool similar(FIELD_TYPE _left, FIELD_TYPE _right) noexcept;

	template <typename _Type>
	static std::optional<SizeType> next(SizeType _size, \
		SizeType _offset, const _Type& _value) noexcept;

	template <typename _Type>
	static bool fetch(_Type& _value, const char* _data);

	template <>
	static bool fetch(FIELD_TYPE& _type, const char* _data);

	template <typename _Source, typename _Target>
	static bool convert(_Target& _value, const char* _data);

private:
	template <typename _Container>
	static void reserve(_Container& _container, SizeType _size) noexcept {}

	template <typename _Element>
	static void reserve(std::vector<_Element>& _vector, SizeType _size)
	{
		_vector.reserve(_vector.size() + _size);
	}

	template <typename _Container, typename _Element>
	static void push(_Container& _container, _Element&& _element)
	{
		_container.insert(std::forward<_Element>(_element));
	}

	template <typename _Element>
	static void push(std::vector<_Element>& _vector, _Element&& _element)
	{
		_vector.push_back(std::forward<_Element>(_element));
	}

	template <typename _Element>
	static void push(std::deque<_Element>& _deque, _Element&& _element)
	{
		_deque.push_back(std::forward<_Element>(_element));
	}

	template <typename _Element>
	static void push(std::list<_Element>& _list, _Element&& _element)
	{
		_list.push_back(std::forward<_Element>(_element));
	}

	template <typename _Container>
	static bool getContainer(_Container& _container, const char* _data, \
		SizeType _size, FIELD_TYPE _type, SizeType _capacity);

	template <typename _Container>
	static bool getBase(_Container& _container, const char* _data, \
		SizeType _size, FIELD_TYPE _type, SizeType _capacity);

	GET_CONTAINER(std::vector, getBase);

	GET_CONTAINER(std::deque, getBase);

	GET_CONTAINER(std::list, getBase);

	GET_CONTAINER(std::set, getBase);

	GET_CONTAINER(std::multiset, getBase);

	GET_CONTAINER(std::unordered_set, getBase);

	GET_CONTAINER(std::unordered_multiset, getBase);

	template <typename _Container>
	static bool getVector(_Container& _container, \
		const char* _data, SizeType _size);

	template <typename _Element>
	static bool getElement(_Element& _element, \
		const char*& _data, SizeType& _size, FIELD_TYPE _type);

	template <typename _Container>
	static bool getMapping(_Container& _container, \
		const char* _data, SizeType _size);

public:
	template <typename _Type>
	static bool get(_Type& _value, \
		const char* _data, SizeType _size);

	template <>
	static bool get(std::string& _string, \
		const char* _data, SizeType _size)
	{
		_string.append(_data, _size);
		return true;
	}

	template <>
	static bool get(Table& _table, \
		const char* _data, SizeType _size)
	{
		_table.append(_data, _size);
		return true;
	}

	template <typename _Element>
	static bool get(std::vector<_Element>& _vector, \
		const char* _data, SizeType _size)
	{
		return getVector(_vector, _data, _size);
	}

	template <typename _Element>
	static bool get(std::deque<_Element>& _deque, \
		const char* _data, SizeType _size)
	{
		return getVector(_deque, _data, _size);
	}

	template <typename _Element>
	static bool get(std::list<_Element>& _list, \
		const char* _data, SizeType _size)
	{
		return getVector(_list, _data, _size);
	}

	template <typename _Element>
	static bool get(std::set<_Element>& _set, \
		const char* _data, SizeType _size)
	{
		return getVector(_set, _data, _size);
	}

	template <typename _Element>
	static bool get(std::multiset<_Element>& _set, \
		const char* _data, SizeType _size)
	{
		return getVector(_set, _data, _size);
	}

	template <typename _Element>
	static bool get(std::unordered_set<_Element>& _set, \
		const char* _data, SizeType _size)
	{
		return getVector(_set, _data, _size);
	}

	template <typename _Element>
	static bool get(std::unordered_multiset<_Element>& _set, \
		const char* _data, SizeType _size)
	{
		return getVector(_set, _data, _size);
	}

	template <typename _KeyType, typename _ValueType>
	static bool get(std::map<_KeyType, _ValueType>& _mapping, \
		const char* _data, SizeType _size)
	{
		return getMapping(_mapping, _data, _size);
	}

	template <typename _KeyType, typename _ValueType>
	static bool get(std::multimap<_KeyType, _ValueType>& _mapping, \
		const char* _data, SizeType _size)
	{
		return getMapping(_mapping, _data, _size);
	}

	template <typename _KeyType, typename _ValueType>
	static bool get(std::unordered_map<_KeyType, _ValueType>& _mapping, \
		const char* _data, SizeType _size)
	{
		return getMapping(_mapping, _data, _size);
	}

	template <typename _KeyType, typename _ValueType>
	static bool get(std::unordered_multimap<_KeyType, _ValueType>& _mapping, \
		const char* _data, SizeType _size)
	{
		return getMapping(_mapping, _data, _size);
	}

private:
	Table& set(const char* _data, std::size_t _size);

	template <typename _Container>
	Table& setVector(const _Container& _container, \
		SizePool* _pool = nullptr);

	template <typename _Container>
	Table& setMapping(const _Container& _container, \
		SizePool* _pool = nullptr);

	template <typename _Type>
	Table& set(_Type _value, SizePool* _pool = nullptr);

	template <>
	Table& set(FIELD_TYPE _type, SizePool* _pool)
	{
		return set(static_cast<FieldType>(_type), _pool);
	}

	Table& set(NumberType _value, SizePool* _pool = nullptr)
	{
		return set<NumberType>(_value, _pool);
	}

	Table& set(const char* _string, SizePool* _pool = nullptr);

	Table& set(const std::string& _string, SizePool* _pool = nullptr)
	{
		return set(_string.data(), _string.size());
	}

	Table& set(const Table& _table, SizePool* _pool = nullptr)
	{
		return set(_table.data(), _table.size());
	}

	template <typename _Element>
	Table& set(const std::vector<_Element>& _vector, \
		SizePool* _pool = nullptr)
	{
		return setVector(_vector, _pool);
	}

	template <typename _Element>
	Table& set(const std::deque<_Element>& _deque, \
		SizePool* _pool = nullptr)
	{
		return setVector(_deque, _pool);
	}

	template <typename _Element>
	Table& set(const std::list<_Element>& _list, \
		SizePool* _pool = nullptr)
	{
		return setVector(_list, _pool);
	}

	template <typename _Element>
	Table& set(const std::set<_Element>& _set, \
		SizePool* _pool = nullptr)
	{
		return setVector(_set, _pool);
	}

	template <typename _Element>
	Table& set(const std::multiset<_Element>& _set, \
		SizePool* _pool = nullptr)
	{
		return setVector(_set, _pool);
	}

	template <typename _Element>
	Table& set(const std::unordered_set<_Element>& _set, \
		SizePool* _pool = nullptr)
	{
		return setVector(_set, _pool);
	}

	template <typename _Element>
	Table& set(const std::unordered_multiset<_Element>& _set, \
		SizePool* _pool = nullptr)
	{
		return setVector(_set, _pool);
	}

	template <typename _KeyType, typename _ValueType>
	Table& set(const std::map<_KeyType, _ValueType>& _mapping, \
		SizePool* _pool = nullptr)
	{
		return setMapping(_mapping, _pool);
	}

	template <typename _KeyType, typename _ValueType>
	Table& set(const std::multimap<_KeyType, _ValueType>& _mapping, \
		SizePool* _pool = nullptr)
	{
		return setMapping(_mapping, _pool);
	}

	template <typename _KeyType, typename _ValueType>
	Table& set(const std::unordered_map<_KeyType, _ValueType>& _mapping, \
		SizePool* _pool = nullptr)
	{
		return setMapping(_mapping, _pool);
	}

	template <typename _KeyType, typename _ValueType>
	Table& set(const std::unordered_multimap<_KeyType, _ValueType>& _mapping, \
		SizePool* _pool = nullptr)
	{
		return setMapping(_mapping, _pool);
	}

	bool set(NumberType _number, FIELD_TYPE _type, \
		const char* _data, std::size_t _size);

	template <typename _Container>
	bool setContainer(NumberType _number, const _Container& _container);

private:
	std::optional<SizeType> next(SizeType _size, SizeType _offset) const;

	void parse() const;

private:
	const char* get(NumberType _number, FIELD_TYPE _type) const;

	template <typename _Container>
	bool getContainer(NumberType _number, _Container& _container) const;

public:
	Table() : _offset(0) {};

	explicit Table(const std::string& _stream)
		: _stream(_stream), _offset(0) {}

	explicit Table(std::string&& _stream)
		: _stream(std::move(_stream)), _offset(0) {}

	Table(const char* _data, std::size_t _size)
		: _stream(_data, _size), _offset(0) {}

	Table& operator=(const std::string& _stream)
	{
		return assign(_stream);
	}

	Table& operator=(std::string&& _stream) noexcept
	{
		return assign(std::move(_stream));
	}

	Table& assign(const std::string& _stream);

	Table& assign(std::string&& _stream) noexcept;

	Table& assign(const char* _data, std::size_t _size);

	Table& append(const std::string& _stream)
	{
		this->_stream.append(_stream);
		return *this;
	}

	Table& append(const char* _data, \
		std::size_t _size)
	{
		_stream.append(_data, _size);
		return *this;
	}

	void clear() noexcept;

	auto data() const noexcept \
		-> decltype(_stream.data())
	{
		return _stream.data();
	}

	auto size() const noexcept \
		-> decltype(_stream.size())
	{
		return _stream.size();
	}

	template <typename _Type>
	bool set(NumberType _number, _Type _value);

	bool set(NumberType _number, const char* _string)
	{
		return set(_number, typeOfValue(_string), \
			_string, std::strlen(_string));
	}

	bool set(NumberType _number, const std::string& _string)
	{
		return set(_number, typeOfValue(_string), \
			_string.data(), _string.size());
	}

	bool set(NumberType _number, const Table& _table)
	{
		return set(_number, typeOfValue(_table), \
			_table.data(), _table.size());
	}

	template <typename _Element>
	bool set(NumberType _number, \
		const std::vector<_Element>& _vector)
	{
		return setContainer(_number, _vector);
	}

	template <typename _Element>
	bool set(NumberType _number, \
		const std::deque<_Element>& _deque)
	{
		return setContainer(_number, _deque);
	}

	template <typename _Element>
	bool set(NumberType _number, \
		const std::list<_Element>& _list)
	{
		return setContainer(_number, _list);
	}

	template <typename _Element>
	bool set(NumberType _number, \
		const std::set<_Element>& _set)
	{
		return setContainer(_number, _set);
	}

	template <typename _Element>
	bool set(NumberType _number, \
		const std::multiset<_Element>& _set)
	{
		return setContainer(_number, _set);
	}

	template <typename _Element>
	bool set(NumberType _number, \
		const std::unordered_set<_Element>& _set)
	{
		return setContainer(_number, _set);
	}

	template <typename _Element>
	bool set(NumberType _number, \
		const std::unordered_multiset<_Element>& _set)
	{
		return setContainer(_number, _set);
	}

	template <typename _KeyType, typename _ValueType>
	bool set(NumberType _number, \
		const std::map<_KeyType, _ValueType>& _mapping)
	{
		return setContainer(_number, _mapping);
	}

	template <typename _KeyType, typename _ValueType>
	bool set(NumberType _number, \
		const std::multimap<_KeyType, _ValueType>& _mapping)
	{
		return setContainer(_number, _mapping);
	}

	template <typename _KeyType, typename _ValueType>
	bool set(NumberType _number, \
		const std::unordered_map<_KeyType, _ValueType>& _mapping)
	{
		return setContainer(_number, _mapping);
	}

	template <typename _KeyType, typename _ValueType>
	bool set(NumberType _number, \
		const std::unordered_multimap<_KeyType, _ValueType>& _mapping)
	{
		return setContainer(_number, _mapping);
	}

	template <typename _Type>
	bool get(NumberType _number, _Type& _value) const;

	template <>
	bool get(NumberType _number, std::string& _string) const;

	template <>
	bool get(NumberType _number, Table& _table) const;

	template <typename _Element>
	bool get(NumberType _number, \
		std::vector<_Element>& _vector) const
	{
		return getContainer(_number, _vector);
	}

	template <typename _Element>
	bool get(NumberType _number, \
		std::deque<_Element>& _deque) const
	{
		return getContainer(_number, _deque);
	}

	template <typename _Element>
	bool get(NumberType _number, \
		std::list<_Element>& _list) const
	{
		return getContainer(_number, _list);
	}

	template <typename _Element>
	bool get(NumberType _number, \
		std::set<_Element>& _set) const
	{
		return getContainer(_number, _set);
	}

	template <typename _Element>
	bool get(NumberType _number, \
		std::multiset<_Element>& _set) const
	{
		return getContainer(_number, _set);
	}

	template <typename _Element>
	bool get(NumberType _number, \
		std::unordered_set<_Element>& _set) const
	{
		return getContainer(_number, _set);
	}

	template <typename _Element>
	bool get(NumberType _number, \
		std::unordered_multiset<_Element>& _set) const
	{
		return getContainer(_number, _set);
	}

	template <typename _KeyType, typename _ValueType>
	bool get(NumberType _number, \
		std::map<_KeyType, _ValueType>& _mapping) const
	{
		return getContainer(_number, _mapping);
	}

	template <typename _KeyType, typename _ValueType>
	bool get(NumberType _number, \
		std::multimap<_KeyType, _ValueType>& _mapping) const
	{
		return getContainer(_number, _mapping);
	}

	template <typename _KeyType, typename _ValueType>
	bool get(NumberType _number, \
		std::unordered_map<_KeyType, _ValueType>& _mapping) const
	{
		return getContainer(_number, _mapping);
	}

	template <typename _KeyType, typename _ValueType>
	bool get(NumberType _number, \
		std::unordered_multimap<_KeyType, _ValueType>& _mapping) const
	{
		return getContainer(_number, _mapping);
	}
};

template <>
inline auto sizeOfValue(const Table& _table, SizePool* _pool)
{
	return sizeOfValue(_table.data(), _table.size(), _pool);
}

PROTOCOL_SPACE_END

#undef GET_CONTAINER
