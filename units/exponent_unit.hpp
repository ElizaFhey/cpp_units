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

	/*!
	 * Represents a unit raised to a power. For example,
	 * square meters could be represented as exponent_unit<meter, std::integral_constant<std::intmax_t, 2>>>.
	 * Inverse units can be represented as well by using a negative exponent.
	 */
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

	/*!
	 * Helper meta-function for creating an exponent_unit.
	 */
	template<Unit BaseUnit, std::intmax_t N>
	using make_exponent_t = exponent_unit<BaseUnit, std::integral_constant<std::intmax_t, N>>;

	/*!
	 * Helper meta-function for creating an inverse unit (ie, unit -> 1 / unit)
	 */
	template<Unit unit>
	using inverse_unit = make_exponent_t<unit, -1>;

	/*!
	 * Specialization of the exponent_of meta-function for exponent_unit.
	 */
	template<Unit BaseUnit, class Exponent>
	struct exponent_of<exponent_unit<BaseUnit, Exponent>>
	{
		using type = Exponent;
	};
}