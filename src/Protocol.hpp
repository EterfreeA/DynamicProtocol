#pragma once

#include <utility>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>

#include "Protocol.h"
#include "ByteOrder.h"

PROTOCOL_SPACE_BEGIN

template <typename _Element>
constexpr auto typeOfValue(const std::vector<_Element>&) noexcept
{
	return FIELD_TYPE::VECTOR;
}

template <typename _Element>
constexpr auto typeOfValue(const std::deque<_Element>&) noexcept
{
	return FIELD_TYPE::VECTOR;
}

template <typename _Element>
constexpr auto typeOfValue(const std::list<_Element>&) noexcept
{
	return FIELD_TYPE::VECTOR;
}

template <typename _Element>
constexpr auto typeOfValue(const std::set<_Element>&) noexcept
{
	return FIELD_TYPE::VECTOR;
}

template <typename _Element>
constexpr auto typeOfValue(const std::multiset<_Element>&) noexcept
{
	return FIELD_TYPE::VECTOR;
}

template <typename _Element>
constexpr auto typeOfValue(const std::unordered_set<_Element>&) noexcept
{
	return FIELD_TYPE::VECTOR;
}

template <typename _Element>
constexpr auto typeOfValue(const std::unordered_multiset<_Element>&) noexcept
{
	return FIELD_TYPE::VECTOR;
}

template <typename _KeyType, typename _ValueType>
constexpr auto typeOfValue(const std::map<_KeyType, _ValueType>&) noexcept
{
	return FIELD_TYPE::MAPPING;
}

template <typename _KeyType, typename _ValueType>
constexpr auto typeOfValue(const std::multimap<_KeyType, _ValueType>&) noexcept
{
	return FIELD_TYPE::MAPPING;
}

template <typename _KeyType, typename _ValueType>
constexpr auto typeOfValue(const std::unordered_map<_KeyType, _ValueType>&) noexcept
{
	return FIELD_TYPE::MAPPING;
}

template <typename _KeyType, typename _ValueType>
constexpr auto typeOfValue(const std::unordered_multimap<_KeyType, _ValueType>&) noexcept
{
	return FIELD_TYPE::MAPPING;
}

template <typename _Type>
constexpr auto sizeOfValue() noexcept
{
	USING_BYTE_ORDER_SPACE;

	constexpr auto SIZE = sizeof hton(static_cast<_Type>(0));
	static_assert(SIZE <= MAX_SIZE);
	return static_cast<SizeType>(SIZE);
}

template <typename _Type>
auto sizeOfValue(const _Type& _value, \
	SizePool* _pool = nullptr) noexcept
{
	return sizeOfValue<_Type>();
}

template <>
inline auto sizeOfValue(const std::string& _string, SizePool* _pool)
{
	return sizeOfValue(_string.data(), _string.size(), _pool);
}

template <typename _Element>
auto sizeOfElement(const _Element& _element, \
	std::size_t _size, SizePool* _pool = nullptr);

template <typename _Container>
SizeType sizeOfVector(const _Container& _container, \
	SizePool* _pool = nullptr)
{
	auto vectorSize = _container.size();
	if (vectorSize > MAX_SIZE) return 0;

	constexpr auto EXTRA_SIZE = sizeOfValue<SizeType>() \
		+ sizeOfValue<FieldType>() + sizeOfValue<SizeType>();

	static_assert(EXTRA_SIZE <= MAX_SIZE);
	constexpr auto RESIDUE_SIZE = MAX_SIZE - EXTRA_SIZE;

	auto residueSize = RESIDUE_SIZE;
	auto totalSize = EXTRA_SIZE;

	for (const auto& element : _container)
	{
		auto [size, all] = sizeOfElement(element, \
			vectorSize, _pool);
		if (size <= 0 || residueSize < size)
			return 0;

		residueSize -= size;
		totalSize += size;

		if (all) break;
	}

	if (_pool != nullptr)
		_pool->emplace(&_container, totalSize);
	return totalSize;
}

template <typename _Element>
auto sizeOfValue(const std::vector<_Element>& _vector, \
	SizePool* _pool = nullptr)
{
	return sizeOfVector(_vector, _pool);
}

template <typename _Element>
auto sizeOfValue(const std::deque<_Element>& _deque, \
	SizePool* _pool = nullptr)
{
	return sizeOfVector(_deque, _pool);
}

template <typename _Element>
auto sizeOfValue(const std::list<_Element>& _list, \
	SizePool* _pool = nullptr)
{
	return sizeOfVector(_list, _pool);
}

template <typename _Element>
auto sizeOfValue(const std::set<_Element>& _set, \
	SizePool* _pool = nullptr)
{
	return sizeOfVector(_set, _pool);
}

template <typename _Element>
auto sizeOfValue(const std::multiset<_Element>& _set, \
	SizePool* _pool = nullptr)
{
	return sizeOfVector(_set, _pool);
}

template <typename _Element>
auto sizeOfValue(const std::unordered_set<_Element>& _set, \
	SizePool* _pool = nullptr)
{
	return sizeOfVector(_set, _pool);
}

template <typename _Element>
auto sizeOfValue(const std::unordered_multiset<_Element>& _set, \
	SizePool* _pool = nullptr)
{
	return sizeOfVector(_set, _pool);
}

template <typename _Container>
SizeType sizeOfMapping(const _Container& _container, \
	SizePool* _pool = nullptr)
{
	auto mappingSize = _container.size();
	if (mappingSize > MAX_SIZE) return 0;

	constexpr auto EXTRA_SIZE = sizeOfValue<SizeType>() \
		+ sizeOfValue<FieldType>() * 2 + sizeOfValue<SizeType>();

	static_assert(EXTRA_SIZE <= MAX_SIZE);
	constexpr auto RESIDUE_SIZE = MAX_SIZE - EXTRA_SIZE;

	auto residueSize = RESIDUE_SIZE;
	auto totalSize = EXTRA_SIZE;

	for (auto& [key, _] : _container)
	{
		auto [size, all] = sizeOfElement(key, \
			mappingSize, _pool);
		if (size <= 0 || residueSize < size)
			return 0;

		residueSize -= size;
		totalSize += size;

		if (all) break;
	}

	for (auto& [_, value] : _container)
	{
		auto [size, all] = sizeOfElement(value, \
			mappingSize, _pool);
		if (size <= 0 || residueSize < size)
			return 0;

		residueSize -= size;
		totalSize += size;

		if (all) break;
	}

	if (_pool != nullptr)
		_pool->emplace(&_container, totalSize);
	return totalSize;
}

template <typename _KeyType, typename _ValueType>
auto sizeOfValue(const std::map<_KeyType, _ValueType>& _mapping, \
	SizePool* _pool = nullptr)
{
	return sizeOfMapping(_mapping, _pool);
}

template <typename _KeyType, typename _ValueType>
auto sizeOfValue(const std::multimap<_KeyType, _ValueType>& _mapping, \
	SizePool* _pool = nullptr)
{
	return sizeOfMapping(_mapping, _pool);
}

template <typename _KeyType, typename _ValueType>
auto sizeOfValue(const std::unordered_map<_KeyType, _ValueType>& _mapping, \
	SizePool* _pool = nullptr)
{
	return sizeOfMapping(_mapping, _pool);
}

template <typename _KeyType, typename _ValueType>
auto sizeOfValue(const std::unordered_multimap<_KeyType, _ValueType>& _mapping, \
	SizePool* _pool = nullptr)
{
	return sizeOfMapping(_mapping, _pool);
}

template <typename _Element>
auto sizeOfElement(const _Element& _element, \
	std::size_t _size, SizePool* _pool)
{
	auto size = sizeOfValue(_element, _pool);
	if (typeOfValue(_element) > FIELD_TYPE::BASE)
		return std::make_pair(size, false);

	if (_size > MAX_SIZE / size) size = 0;
	return std::make_pair(static_cast<decltype(size)>(size * _size), true);
}

PROTOCOL_SPACE_END
