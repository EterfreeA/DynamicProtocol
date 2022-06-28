#include "Core.hpp"
#include "Table.hpp"

#include <cstdlib>
#include <utility>
#include <cstdint>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#define PACK_SINGLE_CONTAINER(Container, Element, table, number) \
table.set(number++, \
	Container<Element>{ 11, 12, 13, 14, 15, 16, 17, 18, 19 });

#define PACK_DOUBLE_CONTAINER(Container, Element, table, number) \
table.set(number++, Container<Container<Element>>{ \
	{ 11, 12, 13 }, { 14, 15, 16 }, { 17, 18, 19 } });

#define UNPACK_SINGLE_CONTAINER(Container, Element, table, number) \
std::cout << STRING(Container<Element>) << std::endl; \
if (Container<Element> container; table.get(number++, container)) \
	print(container);

#define UNPACK_DOUBLE_CONTAINER(Container, Element, table, number) \
std::cout << STRING(Container<Container<Element>>) << std::endl; \
if (Container<Container<Element>> container; \
	table.get(number++, container)) \
{ \
	for (const auto& container : container) \
		for (const auto& element : container) \
			std::cout << element << ' '; \
	std::cout << "\b\n" << std::endl; \
}

using Introduction = std::map<std::uint16_t, std::multimap<std::string, std::string>>;

template <typename _Table, typename _Number>
static void packBase(_Table& _table, _Number& _number)
{
	_Table table;
	_Number number = 0;
	table.set(number++, static_cast<std::uint8_t>(1));
	table.set(number++, static_cast<std::uint16_t>(2));
	table.set(number++, static_cast<std::uint32_t>(3));
	table.set(number++, static_cast<std::uint64_t>(4));
	table.set(number++, static_cast<std::int8_t>(5));
	table.set(number++, static_cast<std::int16_t>(6));
	table.set(number++, static_cast<std::int32_t>(7));
	table.set(number++, static_cast<std::int64_t>(8));
	table.set(number++, false);
	table.set(number++, 9.9F);
	table.set(number++, 9.9);
	_table.set(_number++, table);
}

template <typename _Table, typename _Number>
static void packMartialArts(_Table& _table, _Number& _number)
{
	_Table table;
	_Number number = 0;
	table.set(number++, \
		std::unordered_multimap<std::uint8_t, const char*>{ \
		{ 1, "相思无用，" }, { 1, "唯别而已；" }, \
		{ 1, "别期若有定，" }, { 1, "千般煎熬又何如；" }, \
		{ 1, "莫道黯然销魂，" }, { 1, "何处柳暗花明。" } });

	table.set(number++, \
		std::unordered_multimap<std::uint8_t, const char*>{ \
		{ 1, "六神不安" }, { 1, "杞人忧天" }, { 1, "无中生有" }, \
		{ 1, "魂不守舍" }, { 1, "徘徊空谷" }, { 1, "力不从心" }, \
		{ 1, "行尸走肉" }, { 1, "拖泥带水" }, { 1, "倒行逆施" }, \
		{ 1, "废寝忘食" }, { 1, "孤形只影" }, { 1, "饮恨吞声" }, \
		{ 1, "心惊肉跳" }, { 1, "穷途末路" }, { 1, "面无人色" }, \
		{ 1, "想入非非" }, { 1, "呆若木鸡" } });

	std::unordered_map<std::string, _Table> martialArts;
	martialArts.emplace("黯然销魂掌", std::move(table));
	_table.set(_number++, martialArts);
}

static std::string pack()
{
	USING_PROTOCOL_SPACE;

	Table table;
	NumberType number = 0;
	table.set(number++, "DynamicProtocol");

	packBase(table, number);

	table.set(number++, std::vector<bool>{ false, true });
	PACK_DOUBLE_CONTAINER(std::vector, std::int8_t, table, number);
	PACK_DOUBLE_CONTAINER(std::vector, std::uint8_t, table, number);

	PACK_DOUBLE_CONTAINER(std::deque, std::int16_t, table, number);
	PACK_DOUBLE_CONTAINER(std::deque, std::uint16_t, table, number);

	PACK_DOUBLE_CONTAINER(std::list, std::int32_t, table, number);
	PACK_DOUBLE_CONTAINER(std::list, std::uint32_t, table, number);

	PACK_DOUBLE_CONTAINER(std::set, std::int64_t, table, number);
	table.set(number++, \
		std::multiset<std::int64_t>{ 11, 13, 15, 17, 19, 17, 15, 13, 11 });

	PACK_SINGLE_CONTAINER(std::unordered_set, std::uint64_t, table, number);
	table.set(number++, \
		std::unordered_multiset<std::uint64_t>{ 11, 13, 15, 17, 19, 17, 15, 13, 11 });

	table.set(number++, \
		Introduction{ { 1, { { "Organization", "Eterfree" } } }, \
		{ 2, { { "Author", "solifree" }, { "Author", "mystery" } } } });

	packMartialArts(table, number);
	return std::string(table.data(), table.size());
}

template <typename _Table, typename _Number>
static void unpackBase(const _Table& _table, _Number& _number)
{
	_Table table;
	if (!_table.get(_number++, table)) return;

	_Number number = 0;
	if (std::uint16_t value = 0; table.get(number++, value))
		std::cout << value << ' ';
	if (std::uint16_t value = 0; table.get(number++, value))
		std::cout << value << ' ';
	if (std::uint32_t value = 0; table.get(number++, value))
		std::cout << value << ' ';
	if (std::uint64_t value = 0; table.get(number++, value))
		std::cout << value << ' ';

	if (std::int16_t value = 0; table.get(number++, value))
		std::cout << value << ' ';
	if (std::int16_t value = 0; table.get(number++, value))
		std::cout << value << ' ';
	if (std::int32_t value = 0; table.get(number++, value))
		std::cout << value << ' ';
	if (std::int64_t value = 0; table.get(number++, value))
		std::cout << value << ' ';

	if (bool value = 0; table.get(number++, value))
		std::cout << std::boolalpha << value << ' ';
	if (float value = 0; table.get(number++, value))
		std::cout << value << ' ';
	if (double value = 0; table.get(number++, value))
		std::cout << value << ' ';
	std::cout << "\b\n" << std::endl;
}

template <typename _Container>
static void print(const _Container& _container) noexcept
{
	for (const auto& element : _container)
		std::cout << element << ' ';
	std::cout << "\b\n" << std::endl;
}

static void unpack(const std::string& _stream)
{
	USING_PROTOCOL_SPACE;

	Table table(_stream);
	NumberType number = 0;

	std::cout << STRING(std::string) << std::endl;
	if (std::string string; table.get(number++, string))
		std::cout << string << '\n' << std::endl;

	std::cout << STRING(Table) << std::endl;
	unpackBase(table, number);

	std::cout << STRING(std::vector<bool>) << std::endl;
	if (std::vector<bool> vector; table.get(number++, vector))
	{
		for (const auto& element : vector)
			std::cout << std::boolalpha << element << ' ';
		std::cout << "\b\n" << std::endl;
	}

	UNPACK_DOUBLE_CONTAINER(std::vector, std::int16_t, table, number);
	UNPACK_DOUBLE_CONTAINER(std::vector, std::uint16_t, table, number);

	UNPACK_DOUBLE_CONTAINER(std::deque, std::int16_t, table, number);
	UNPACK_DOUBLE_CONTAINER(std::deque, std::uint16_t, table, number);

	UNPACK_DOUBLE_CONTAINER(std::list, std::int32_t, table, number);
	UNPACK_DOUBLE_CONTAINER(std::list, std::uint32_t, table, number);

	UNPACK_DOUBLE_CONTAINER(std::set, std::int64_t, table, number);
	UNPACK_SINGLE_CONTAINER(std::multiset, std::int64_t, table, number);

	UNPACK_SINGLE_CONTAINER(std::unordered_set, std::uint64_t, table, number);
	UNPACK_SINGLE_CONTAINER(std::unordered_multiset, std::uint64_t, table, number);

	std::cout << STRING(Introduction) << std::endl;
	if (Introduction introduction; table.get(number++, introduction))
	{
		for (const auto& [_, pair] : introduction)
			for (const auto& [key, value] : pair)
				std::cout << '[' << key << ": " << value << "] ";
		std::cout << "\b\n" << std::endl;
	}

	if (std::unordered_map<std::string, Table> martialArts; \
		table.get(number++, martialArts))
	{
		for (const auto& [name, content] : martialArts)
		{
			std::cout << name << std::endl;

			NumberType number = 0;
			if (std::unordered_multimap<std::uint8_t, std::string> method; \
				content.get(number++, method))
			{
				for (const auto& [_, sentence] : method)
					std::cout << sentence;
				std::cout << std::endl;
			}

			if (std::unordered_multimap<std::uint8_t, std::string> moves; \
				content.get(number++, moves))
			{
				for (const auto& [_, name] : moves)
					std::cout << name << ' ';
				std::cout << "\b\n" << std::endl;
			}
		}
	}
}

int main()
{
	unpack(pack());
	return EXIT_SUCCESS;
}
