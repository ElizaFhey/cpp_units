#pragma once
#include "units.hpp"
#include "compound_unit.hpp"
#include "exponent_unit.hpp"

namespace units
{
	template<class T>
	concept UnitSystem = requires()
	{
		typename T::length;
		typename T::time;
		typename T::mass;
		typename T::temperature;
		typename T::amount;
		typename T::current;
		typename T::luminosity;
	};

	template<UnitSystem BaseSystem>
	struct unit_system : BaseSystem
	{
		using length = typename BaseSystem::length;
		using time = typename BaseSystem::time;
		using mass = typename BaseSystem::mass;
		using temperature = typename BaseSystem::temperature;
		using amount = typename BaseSystem::amount;
		using current = typename BaseSystem::current;
		using luminosity = typename BaseSystem::luminosity;
		using frequency = inverse_unit<typename BaseSystem::time>;

		using velocity = make_compound_t<typename BaseSystem::length, frequency>;
		using acceleration = make_compound_t<velocity, frequency>;
		using force = make_compound_t<typename BaseSystem::mass, acceleration>;
		using energy = make_compound_t<force, typename BaseSystem::time>;
	};
}