#pragma once
#include <ratio>
#include <cstdint>
#include "units.hpp"
#include "difference_unit.hpp"

namespace units
{
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

	template<Unit BaseUnit, class Offset>
	struct difference_unit<offset_unit<BaseUnit, Offset>>
	{
		using type = BaseUnit;
	};

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