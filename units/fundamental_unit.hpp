#pragma once

namespace units
{
	/*!
	 * fundamental_unit represents the common base unit of a unit_tag group.
	 * By definition, the to_fundamental and from_fundamental functions return
	 * the value passed to them unchaged.
	 */
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