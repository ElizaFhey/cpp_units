#pragma once
#include "../units/quantity.hpp"

namespace units
{
	namespace detail
	{
		template<Unit unit>
		struct quantity_or_delta
		{
			using value_type = typename unit::value_type;
			const value_type value;

			constexpr explicit quantity_or_delta(value_type value)
				: value{ value }
			{}

			quantity_or_delta(quantity_or_delta const&) = delete;
			quantity_or_delta& operator=(quantity_or_delta const&) = delete;

			constexpr operator quantity<unit>() const&& { return quantity<unit>{value}; }
			constexpr operator delta<unit>() const&& { return delta<unit>{value}; }
		};
	}
}