#pragma once
#include "units.hpp"

namespace units
{
	/*!
	 * difference_unit is used to represent the difference
	 * between two units instead of an absolute amount of a unit.
	 * In many cases this will be identitcal to the base unit,
	 * but for some cases (temperature for example) the types will
	 * be different.
	 */
	template<Unit unit>
	struct difference_unit
	{
		using type = unit;
	};

	template<Unit unit>
	using difference_unit_t = typename difference_unit<unit>::type;
}