#pragma once
#include "units.hpp"
#include <cstdint>
#include "detail/unit_comparisons.hpp"

namespace units
{
	namespace impl
	{
		template<class T>
		constexpr T power(T value, int N)
		{
			if (N == 0)
				return 1;
			else if (N < 0)
				return T{ 1 } / power(value, -N);
			else
				return value * power(value, N - 1);
		}
	}

	template<Unit BaseUnit, class Exponent>
	struct exponent_unit
	{
		using base_unit = BaseUnit;
		using value_type = typename BaseUnit::value_type;
		using unit_tag = typename BaseUnit::unit_tag;
		using exponent = Exponent;

		constexpr static value_type to_fundamental(value_type value)
		{
			return value * impl::power(base_unit::to_fundamental(1), exponent::value);
		}

		constexpr static value_type from_fundamental(value_type value)
		{
			return value * impl::power(base_unit::from_fundamental(1), exponent::value);
		}
	};

	template<Unit BaseUnit, std::intmax_t N>
	using make_exponent_t = exponent_unit<BaseUnit, std::integral_constant<std::intmax_t, N>>;

	template<Unit unit>
	using inverse_unit = make_exponent_t<unit, -1>;

	template<Unit BaseUnit, class Exponent>
	struct exponent_of<exponent_unit<BaseUnit, Exponent>>
	{
		using type = Exponent;
	};
}