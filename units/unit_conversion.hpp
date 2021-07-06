#pragma once
#include "units.hpp"
#include "detail/unit_comparisons.hpp"

namespace units
{
	template<Unit From, Unit To>
	requires SimilarUnits<From, To>
	struct unit_conversion 
	{
		using value_type = std::common_type_t<typename From::value_type, typename To::value_type>;

		constexpr static value_type convert(value_type value)
		{
			return To::from_fundamental(From::to_fundamental(value));
		}
	};
}