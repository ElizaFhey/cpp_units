#pragma once
#include "units.hpp"

namespace units
{
	template<Unit unit>
	struct difference_unit
	{
		using type = unit;
	};

	template<Unit unit>
	using difference_unit_t = typename difference_unit<unit>::type;
}