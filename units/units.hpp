#pragma once
#include <type_traits>

namespace units
{
	/*!
	 * Unit concept. This is used extensively in meta-functions to check that the type constraints are satisfied.
	 */
	template<class T>
	concept Unit = requires()
	{
		//! The value_type of the unit. This is the underlying numeric representation used to store values of this unit.
		typename T::value_type;
		/*!
		 * The unit_tag.This is used to track the base type of the unit.
		 * For example, meters and millimeters would have the same unit_tag, since they are both lengths, but meters and kilograms
		 * would not since they are length and mass respectively.
		 */ 
		typename T::unit_tag;
		/*!
		 * Each unit_tag group has a common type called the 'fundamental' type, which is defined by the unit system being used.
		 * Each Unit type must provide static functions 'to_fundamental' and 'from_fundamental' which transforms values to and from
		 * the fundamental unit type.
		 */
		T::to_fundamental(std::declval<typename T::value_type>());
		T::from_fundamental(std::declval<typename T::value_type>());
	};

	/*!
	 * Meta-function, returns the unit_tag of a Unit.
	 */
	template<Unit UnitType>
	struct tag_of
	{
		using type = typename UnitType::unit_tag;
	};

	template<Unit UnitType>
	using tag_of_t = typename tag_of<UnitType>::type;

	template<Unit UnitType>
	constexpr auto tag(UnitType) { return tag_of_t<UnitType>{}; }

	/*!
	 * Meta-function, returns the exponent of a Unit type. For example, square meters would have an exponent of 2,
	 * while meters would have an exponent of 1.
	 */
	template<Unit unit>
	struct exponent_of
	{
		using type = std::integral_constant<std::intmax_t, 1>;
	};

	template<Unit unit>
	using exponent_of_t = typename exponent_of<unit>::type;

	template<Unit unit>
	constexpr const std::intmax_t exponent_of_v = exponent_of_t<unit>::value;

	/*template<Unit UnitType>
	constexpr auto exponent(UnitType) { return exponent_of_v<UnitType>{}; }*/

	template<Unit unit>
	class quantity;

	template<Unit unit>
	class delta;
}