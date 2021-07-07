#pragma once
#include "../units.hpp"
#include "../fundamental_unit.hpp"
#include "../exponent_unit.hpp"
#include "../quantity.hpp"
#include "../compound_unit.hpp"
#include "../linear_unit.hpp"
#include "../unit_system.hpp"
#include "../detail/literal_helper.hpp"

namespace units
{
	namespace si_system
	{
		template<class ValueType>
		struct celsius_offset_type
		{
			constexpr static const ValueType value = ValueType{ 273.15 };
		};

		template<class ValueType>
		struct si_unit_system
		{
			struct meter : fundamental_unit<meter, ValueType> {};
			struct kilogram : fundamental_unit<kilogram, ValueType> {};
			struct second : fundamental_unit<second, ValueType> {};
			struct ampere : fundamental_unit<ampere, ValueType> {};
			struct kelvin : fundamental_unit<kelvin, ValueType> {};
			struct mole : fundamental_unit<mole, ValueType> {};
			struct candela : fundamental_unit<candela, ValueType> {};

			using length = meter;
			using mass = kilogram;
			using time = second;
			using current = ampere;
			using temperature = kelvin;
			using amount = mole;
			using luminosity = candela;

			using celsius = offset_unit<kelvin, celsius_offset_type<ValueType>>;
		};
	}

	template<class ValueType>
	using si_system_t = unit_system<si_system::si_unit_system<ValueType>>;

	using si = si_system_t<double>;

	namespace literals
	{
#define CPP_UNITS_MAKE_LITERAL(name, unit) \
		constexpr inline detail::quantity_or_delta< unit > operator""_##name (long double dval) { return detail::quantity_or_delta< unit > { static_cast<double>(dval)};} \
		constexpr inline detail::quantity_or_delta< unit > operator""_##name (unsigned long long int ival) { return detail::quantity_or_delta< unit > { static_cast<double>(ival)};}

		CPP_UNITS_MAKE_LITERAL(meter, si::meter);
		CPP_UNITS_MAKE_LITERAL(sec, si::second);

		/*constexpr inline detail::quantity_or_delta<si::meter> operator""_meter(long double dval) { return detail::quantity_or_delta<si::meter>{static_cast<double>(dval)}; }
		constexpr inline detail::quantity_or_delta<si::meter> operator""_meter(unsigned long long int ival) { return detail::quantity_or_delta<si::meter>{static_cast<double>(ival)}; }*/
	}
}