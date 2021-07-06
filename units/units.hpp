#pragma once
#include <type_traits>

namespace units
{
	template<class T>
	concept Unit = requires()
	{
		typename T::value_type;
		typename T::unit_tag;
		T::to_fundamental(std::declval<typename T::value_type>());
		T::from_fundamental(std::declval<typename T::value_type>());
	};

	template<Unit UnitType>
	struct tag_of
	{
		using type = typename UnitType::unit_tag;
	};

	template<Unit UnitType>
	using tag_of_t = typename tag_of<UnitType>::type;

	template<Unit UnitType>
	constexpr auto tag(UnitType) { return tag_of_t<UnitType>{}; }

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