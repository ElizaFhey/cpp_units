#pragma once

namespace units
{
	template<class Unit, class ValueType>
	struct fundamental_unit
	{
		using unit_tag = Unit;
		using base_type = Unit;
		using value_type = ValueType;

		constexpr static value_type to_fundamental(value_type value) { return value; }
		constexpr static value_type from_fundamental(value_type value) { return value; }
	};


}