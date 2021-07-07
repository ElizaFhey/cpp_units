#include "../units/systems/si.hpp"
#include "../units/quantity.hpp"

namespace tests
{
	using units::si;
	using units::quantity;
	using units::delta;

	constexpr quantity<si::length> position(delta<si::time> elapsed, quantity<si::acceleration> a, quantity<si::velocity> v, quantity<si::length> initial = quantity<si::length>{0})
	{
		//return <si::length>{a* elapsed* elapsed + v * elapsed};
		return initial + a * elapsed * elapsed + v * elapsed;
	}

	using namespace units::literals;

	constexpr quantity<si::length> freefall = position(10._sec, quantity<si::acceleration>{-9.8}, quantity<si::velocity>{}, 5000_meter);
}