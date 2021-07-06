#pragma once
#include "units.hpp"
#include "detail/unit_comparisons.hpp"

namespace units
{
	/*!
	 * Helper function for converting from one unit type to another
	 * 
	 * @tparam From The unit type to convert from
	 * @tparam To The unit type to convert to
	 * 
	 * Note: The From and To parameters must satisfy SimilarUnits<From, To>
	 */
	template<Unit From, Unit To>
	requires SimilarUnits<From, To>
	struct unit_conversion 
	{
		using value_type = std::common_type_t<typename From::value_type, typename To::value_type>;

		/*!
		 * Convert a value of unit type From to unit type To and return the result.
		 */
		constexpr static value_type convert(value_type value)
		{
			return To::from_fundamental(From::to_fundamental(value));
		}
	};
}