#pragma once
#include "../units.hpp"

namespace units
{
	namespace detail
	{
		struct empty_list { using unit = void; using next = void; };

		template<Unit UnitType, class Rest = empty_list>
		struct unit_list
		{
			using unit = UnitType;
			using next = Rest;
		};

		template<class T>
		struct is_unit_list : std::false_type {};

		template<Unit UnitType, class Rest>
		struct is_unit_list<unit_list<UnitType, Rest>> : std::true_type {};

		template<>
		struct is_unit_list<empty_list> : std::true_type {};

		template<class T>
		concept UnitList = is_unit_list<T>::value;

		template<Unit unit, UnitList list>
		struct push_list
		{
			using type = unit_list<unit, list>;
		};

		template<Unit unit, UnitList list>
		using push_list_t = typename push_list<unit, list>::type;

		template<UnitList list>
		struct pop_list
		{
			using type = typename list::next;
		};

		template<UnitList list>
		using pop_list_t = typename pop_list<list>::type;

		template<UnitList list>
		struct top_list
		{
			using type = typename list::unit;
		};

		template<UnitList list>
		using top_list_t = typename top_list<list>::type;

		template<UnitList list>
		struct size_list : std::integral_constant<int, 1 + size_list<pop_list_t<list>>::value> {};

		template<>
		struct size_list<empty_list> : std::integral_constant<int, 0> {};

		template<UnitList list>
		using size_list_t = typename size_list<list>::value_type;

		template<UnitList list>
		constexpr int size_list_v = size_list_t<list>::value;

		template<Unit unit, Unit... rest>
		struct make_list
		{
			using type = push_list_t<unit, typename make_list<rest...>::type>;
		};

		template<Unit unit>
		struct make_list<unit>
		{
			using type = unit_list<unit>;
		};

		template<Unit... units>
		using make_list_t = typename make_list<units...>::type;

		template<Unit unit, UnitList list>
		struct find_unit_exact
		{
			using type = std::conditional_t<std::is_same<unit, typename list::unit>::value,
				list,
				typename find_unit_exact<unit, pop_list_t<list>>::type
			>;
		};

		template<Unit unit>
		struct find_unit_exact<unit, empty_list>
		{
			using type = empty_list;
		};

		template<Unit unit, UnitList list>
		using find_unit_exact_t = typename find_unit_exact<unit, list>::type;

		template<Unit unit, UnitList list>
		struct find_unit_tag
		{
			using type = std::conditional_t<std::is_same<typename unit::unit_tag, typename list::unit::unit_tag>::value,
				list,
				typename find_unit_tag<unit, pop_list_t<list>>::type
			>;
		};

		template<Unit unit>
		struct find_unit_tag<unit, empty_list>
		{
			using type = empty_list;
		};

		template<Unit unit, UnitList list>
		using find_unit_tag_t = typename find_unit_tag<unit, list>::type;

		template<template <class, class> class Compare, Unit unit, UnitList list>
		struct remove_item
		{
			using top = top_list_t<list>;
			using rest = typename remove_item<Compare, unit, pop_list_t<list>>::type;
			using type = std::conditional_t<
				Compare<unit, top>::value,
				rest,
				push_list_t<top, rest>
			>;
		};

		template<template<class, class> class Compare, Unit unit>
		struct remove_item<Compare, unit, empty_list>
		{
			using type = empty_list;
		};

		template<template <class, class> class Compare, Unit unit, UnitList list>
		using remove_item_t = typename remove_item<Compare, unit, list>::type;

		template<template<class, class> class Compare, UnitList List>
		struct unique_items
		{
			using top = top_list_t<List>;
			using type = push_list_t<top, typename unique_items<Compare, remove_item_t<Compare, top, List>>::type>;
		};

		template<template<class, class> class Compare>
		struct unique_items<Compare, empty_list>
		{
			using type = empty_list;
		};
	}
}