#pragma once
#include <type_traits>
#include "units.hpp"
#include "detail/unit_list.hpp"
#include "detail/unit_comparisons.hpp"
#include "exponent_unit.hpp"
#include "difference_unit.hpp"

namespace units
{
	/*!
	 * A compound unit is made up of one or more child units. An example of this
	 * would be meters/second or kg/cubic meters.
	 */
	template<Unit... Units>
	struct compound_unit
	{
		using units = detail::make_list_t<Units...>;
		using value_type = std::common_type_t<typename Units::value_type...>;
		using unit_tag = detail::make_list_t<typename Units::unit_tag...>;

		constexpr static value_type to_fundamental(value_type value)
		{
			return value * (Units::to_fundamental(1) * ...);
		}

		constexpr static value_type from_fundamental(value_type value)
		{
			return value * (Units::from_fundamental(1) * ...);
		}
	};

	namespace detail
	{
		template<Unit UnitType, UnitList List>
		struct has_unit_tag
		{
			using position = find_unit_tag_t<UnitType, List>;
			using type = std::bool_constant<!std::is_same_v<position, empty_list>>;
		};

		template<Unit UnitType, UnitList List>
		using has_unit_tag_t = typename has_unit_tag<UnitType, List>::type;

		template<Unit UnitType, UnitList List>
		constexpr const bool has_unit_tag_v = has_unit_tag_t<UnitType, List>::value;
	}

	/*!
	 * Specialization of the compare_tag meta-function. Comparing a non-compound unit
	 * to a compound one is done by making a compound_unit with a single type (of UnitA)
	 * and calling the compare_tag specialization for multiple compound units.
	 */
	template<Unit UnitA, Unit... Compound>
	struct compare_tag<UnitA, compound_unit<Compound...>>
		: compare_tag<compound_unit<UnitA>, compound_unit<Compound...>> {};

	template<Unit UnitType, Unit... Compound>
	struct compare_tag<compound_unit<Compound...>, UnitType>
		: compare_tag<compound_unit<UnitType>, compound_unit<Compound...>> {};

	/*!
	 * Specialization of the compare_tag meta-function for compound_units.
	 * This checks that all of the unit_tags in CompA are present in the unit_tags of CompB
	 * and vice-versa
	 */
	template<Unit... CompA, Unit... CompB>
	struct compare_tag<compound_unit<CompA...>, compound_unit<CompB...>>
	{
		using a = compound_unit<CompA...>;
		using b = compound_unit<CompB...>;

		constexpr static bool check_tags()
		{
			return (detail::has_unit_tag_v<CompA, typename b::units> && ...) && (detail::has_unit_tag_v<CompB, typename a::units> && ...);
		}

		//using same_size = std::bool_constant<detail::size_list_v<typename a::unit_tag> == detail::size_list_v<typename b::unit_tag>>;
		using type = std::bool_constant<check_tags()>;
	};

	/*!
	 * Specialization of the compare_exponent meta-function. To compare the exponents of a single unit A with
	 * the compound_unit C, we promote A into a compound_unit with a single type then call the specialization
	 * of compare_exponent on multiple compound_units.
	 */
	template<Unit A, Unit... C>
	struct compare_exponent<A, compound_unit<C...>> : compare_exponent<compound_unit<A>, compound_unit<C...>>{};

	template<Unit A, Unit... C>
	struct compare_exponent<compound_unit<C...>, A> : compare_exponent<compound_unit<A>, compound_unit<C...>>{};

	namespace detail
	{
		template<Unit unit, UnitList units>
		struct sum_exponent
		{
			using pos = detail::find_unit_tag_t<unit, units>;
			constexpr static const std::intmax_t value = std::conditional_t<
				std::is_same<pos, empty_list>::value,
				std::integral_constant<std::intmax_t, 0>,
				std::integral_constant<std::intmax_t, exponent_of_v<typename pos::unit> +sum_exponent<unit, pop_list_t<pos>>::value>
			>::value;
			/*if constexpr (std::is_same_v<pos, detail::empty_list>)
				return 0;
			else
				return exponent_of_v<typename pos::unit> +sum_exponent<unit, detail::pop_list_t<pos>>();*/
		};

		template<Unit unit>
		struct sum_exponent<unit, empty_list>
		{
			using pos = empty_list;
			constexpr static const std::intmax_t value = 0;
		};

		template<UnitList units, UnitList unit_a, UnitList unit_b>
		constexpr std::intmax_t compare_exponents()
		{
			if constexpr (!std::is_same<units, empty_list>::value)
			{
				using current = detail::top_list_t<units>;
				constexpr std::intmax_t val = detail::sum_exponent<current, unit_a>::value - detail::sum_exponent<current, unit_b>::value;
				return val != 0 ? val : detail::compare_exponents<detail::pop_list_t<units>, unit_a, unit_b>();
			}
			else
				return 0;
		}
	}

	/*!
	 * Specialization of compare_exponent for compound_units.
	 * Essentially this works by making a list of unique unit_tags found in both
	 * A and B, then doing a lexicographic comparison of the total sum of the exponents
	 * for each unique unit_tag.
	 */
	template<Unit ...A, Unit ...B>
	struct compare_exponent<compound_unit<A...>, compound_unit<B...>>
	{
		using unit_a = compound_unit<A...>;
		using unit_b = compound_unit<B...>;

		using all_units = detail::make_list_t<A..., B...>;
		using all_unique = typename detail::unique_items<compare_tag_t, all_units>::type;

		/*using unique_a = detail::unique_items<compare_tag_t, typename unit_a::units>;
		using unique_b = detail::unique_items<compare_tag_t, typename unit_b::units>;*/

		using type = std::integral_constant<std::intmax_t, detail::compare_exponents<all_unique, typename unit_a::units, typename unit_b::units>()>;
	};

	/*!
	 * Meta-function for making a compound unit with Units A and B.
	 * Although any unit can be represented using compound_unit<...> directly,
	 * it is sometimes convenient to use a meta-function instead. In addition,
	 * this allows for specializations; for example, calling make_compound with
	 * two exponent units with the same base unit
	 * just adds the exponents instead of creating a compound_unit.
	 */
	template<Unit A, Unit B>
	struct make_compound
	{
		using type = compound_unit<A, B>;
	};

	template<Unit A, Unit B>
	using make_compound_t = typename make_compound<A, B>::type;

	template<Unit ...As, Unit ...Bs>
	struct make_compound<compound_unit<As...>, compound_unit<Bs...>>
	{
		using type = compound_unit<As..., Bs...>;
	};

	template<Unit T, class ExponentA, class ExponentB>
	struct make_compound<exponent_unit<T, ExponentA>, exponent_unit<T, ExponentB>>
	{
		using type = make_exponent_t<T, ExponentA::value + ExponentB::value>;
	};

	/*!
	 * Specialization for difference_unit. The difference unit of a compound unit
	 * is compound_unit of the difference_unit of its types.
	 */
	template<Unit ...units>
	struct difference_unit<compound_unit<units...>>
	{
		using type = compound_unit<difference_unit_t<units>...>;
	};
}