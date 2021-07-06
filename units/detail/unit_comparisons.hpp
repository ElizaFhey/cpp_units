#pragma once
#include "../units.hpp"
#include "unit_list.hpp"

namespace units
{
	template<Unit UnitA, Unit UnitB>
	struct same_unit : std::false_type {};

	template<Unit unit>
	struct same_unit<unit, unit> : std::true_type {};

	/*template<Unit unit_a, Unit unit_b>
	struct same_exponent : std::true_type {};*/

	template<Unit UnitA, Unit UnitB>
	struct compare_exponent
	{
		using type = std::integral_constant<std::intmax_t, exponent_of_v<UnitA> - exponent_of_v<UnitB>>;
	};

	template<Unit UnitA, Unit UnitB>
	using compare_exponent_t = typename compare_exponent<UnitA, UnitB>::type;

	template<Unit UnitA, Unit UnitB>
	constexpr const std::intmax_t compare_exponent_v = compare_exponent_t<UnitA, UnitB>::value;

	template<Unit UnitA, Unit UnitB>
	struct compare_tag 
	{
		using type = std::is_same<tag_of_t<UnitA>, tag_of_t<UnitB>>;
	};

	template<Unit UnitA, Unit UnitB>
	using compare_tag_t = typename compare_tag<UnitA, UnitB>::type;

	template<Unit UnitA, Unit UnitB>
	constexpr const bool compare_tag_v = compare_tag_t<UnitA, UnitB>::value;

	template<Unit UnitA, Unit UnitB>
	struct similar_units 
	{
		using type = std::bool_constant<
			compare_exponent_t<UnitA, UnitB>::value == 0
			&& compare_tag_t<UnitA, UnitB>::value>;
	};

	template<Unit UnitA, Unit UnitB>
	using similar_units_t = typename similar_units<UnitA, UnitB>::type;

	template<Unit UnitA, Unit UnitB>
	constexpr const bool similar_units_v = similar_units_t<UnitA, UnitB>::value;

	template<class A, class B>
	concept SimilarUnits = Unit<A> && Unit<B> && similar_units_v<A, B>;

	/*template<Unit UnitA, Unit UnitB>
	constexpr auto is_similar(UnitA, UnitB)
	{
		return std::bool_constant<compare_exponent_t<UnitA, UnitB>::value == 0 && std::is_same<tag_of_t<UnitA>, tag_of_t<UnitB>>::value > {};
	}*/

	/*template<Unit unit_a, Unit unit_b>
	struct similar_unit :
		std::bool_constant<
			std::is_same<typename unit_a::unit_tag, typename unit_b::unit_tag>::value
			&& exponent_of_v<unit_a> == exponent_of_v<unit_b>>::value
		> {};*/
}
