#pragma once
#include <ratio>
#include <cstdint>
#include "units.hpp"
#include "difference_unit.hpp"

namespace units
{
	/*!
	 * Same as std::ratio, but adds the apply and apply_inverse functions to the interface.
	 */
	template<std::intmax_t Num, std::intmax_t Den = 1>
	struct ratio : std::ratio<Num, Den>
	{
		template<class value_type>
		constexpr static value_type apply(value_type value)
		{
			return Num * value / Den;
		}

		template<class value_type>
		constexpr static value_type apply_inverse(value_type value)
		{
			return Den * value / Num;
		}
	};

	/*!
	 * scaled_unit represents a unit that is in a fixed ratio
	 * to another unit type.
	 *
	 * @tparam BaseUnit The unit this unit is scaled from. This does not need to be a fundamental_unit; it is perfectly fine to chain unit definitions.
	 * @tparam Ratio The ratio between this unit and the base unit. The Numerator is the number of this unit to a Denominator number of the BaseUnit.
	 */
	template<Unit BaseUnit, class Ratio>
	struct scaled_unit
	{
		using ratio_type = Ratio;
		using base_unit = BaseUnit;
		using value_type = typename BaseUnit::value_type;
		using unit_tag = typename BaseUnit::unit_tag;

		constexpr static value_type to_fundamental(value_type v)
		{
			return Ratio::apply_inverse(v);
		}

		constexpr static value_type from_fundamental(value_type v)
		{
			return Ratio::apply(v);
		}
	};

	/*!
	 * offset_unit represents a unit that is a fixed offset from another unit. For example, celsius could be defined as 
	 * @code
	 * struct celsius_offset_type { constexpr static const double value = 273.15;};
	 * using celsius = offset_unit<kelvin, celsius_offset_type>;
	 * @endcode
	 */
	template<Unit BaseUnit, class Offset>
	struct offset_unit
	{
		using offset_type = Offset;
		using base_unit = BaseUnit;
		using value_type = typename BaseUnit::value_type;
		using unit_tag = typename BaseUnit::unit_tag;

		constexpr static value_type to_fundamental(value_type v)
		{
			return v - Offset::value;
		}

		constexpr static value_type from_fundamental(value_type v)
		{
			return v + Offset::value;
		}
	};

	/*!
	 * Specialization of difference_unit for offset_unit
	 * Suppose offset_type = base_type + offset.
	 * Then given offset_type values A and B,
	 * @code
	 * A - B = (k + offset) - (j + offset) for some values j, k
	 * = k + offset - j - offset
	 * = k - j
	 * @endcode
	 * As you can see, the offsets cancel. As a result, the 
	 * difference_unit type is therefore just the base unit itself
	 */
	template<Unit BaseUnit, class Offset>
	struct difference_unit<offset_unit<BaseUnit, Offset>>
	{
		using type = BaseUnit;
	};

	/*!
	 * linear_unit combines scaled_unit and offset_unit into
	 * a single unit type.
	 */
	template<Unit BaseUnit, class Ratio, class Offset>
	struct linear_unit
	{
		using base_unit = BaseUnit;
		using value_type = typename BaseUnit::value_type;
		using unit_tag = typename BaseUnit::unit_tag;
		using scaled = scaled_unit<BaseUnit, Ratio>;
		using offset = offset_unit<BaseUnit, Offset>;

		constexpr static value_type to_fundamental(value_type v)
		{
			return scaled::to_fundamental(offset::to_fundamental(v));
		}

		constexpr static value_type from_fundamental(value_type v)
		{
			return offset::from_fundamental(scaled::from_fundamental(v));
		}
	};

	template<Unit BaseUnit, class Ratio, class Offset>
	struct difference_unit<linear_unit<BaseUnit, Ratio, Offset>>
	{
		using type = scaled_unit<BaseUnit, Ratio>;
	};

	/*!
	 * Predefined list of metric prefixes.
	 * These provided a convenient way to create
	 * new unit types scaled from a base type
	 * For example, using millimeter = prefixes::milli<meter>;
	 */
	namespace prefixes
	{
		template<Unit unit>
		using nano = scaled_unit<unit, ratio<1000000000>>;

		template<Unit unit>
		using micro = scaled_unit<unit, ratio<1000000>>;

		template<Unit unit>
		using milli = scaled_unit<unit, ratio<1000>>;

		template<Unit unit>
		using centi = scaled_unit<unit, ratio<100>>;

		template<Unit unit>
		using deci = scaled_unit<unit, ratio<10>>;

		template<Unit unit>
		using deca = scaled_unit<unit, ratio<1, 10>>;

		template<Unit unit>
		using centa = scaled_unit<unit, ratio<1, 100>>;

		template<Unit unit>
		using kilo = scaled_unit<unit, ratio<1, 1000>>;

		template<Unit unit>
		using mega = scaled_unit<unit, ratio<1, 1000000>>;

		template<Unit unit>
		using giga = scaled_unit<unit, ratio<1, 1000000000>>;
	}

	using namespace prefixes;
}