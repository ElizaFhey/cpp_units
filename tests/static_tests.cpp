#include "units/units.hpp"
#include "units/fundamental_unit.hpp"
#include "units/linear_unit.hpp"
#include "units/exponent_unit.hpp"
#include "units/compound_unit.hpp"
#include "units/quantity.hpp"
#include "units/delta.hpp"

template<class Unit> requires units::Unit<Unit>
constexpr auto to_fundamental(auto val) { return Unit::to_fundamental(val); }

namespace static_tests
{
	template<class T>
	constexpr T static_abs(T value)
	{
		return value < 0 ? -value : value;
	}

	struct meter : units::fundamental_unit<meter, double> {};
	struct celsius : units::fundamental_unit<celsius, double> {};

	using millimeter = units::scaled_unit<meter, units::ratio<1000>>;
	using fahrenheit = units::linear_unit<celsius, units::ratio<9, 5>, std::integral_constant<int, 32>>;

	using sq_meter = units::make_exponent_t<meter, 2>;
	using sq_millimeter = units::make_exponent_t<millimeter, 2>;

	using meters2 = units::compound_unit<meter, meter>;
	using millimeters2 = units::compound_unit<millimeter, millimeter>;

	constexpr double m1 = to_fundamental<meter>(10.0);
	constexpr double m2 = millimeters2::to_fundamental(1);
	constexpr double m3 = millimeters2::from_fundamental(1);

	/*constexpr double mm = millimeter::from_fundamental(1);
	constexpr double m*/
	static_assert(millimeter::from_fundamental(1) == 1000, "Incorrect value for millimeter");
	static_assert(millimeter::to_fundamental(1) == 0.001, "Incorrect value for millimeter");
	static_assert(fahrenheit::to_fundamental(32) == 0, "Incorrect value for fahrenheit");
	static_assert(fahrenheit::from_fundamental(0) == 32, "Incorrect value for fahrenheit");
	static_assert(fahrenheit::to_fundamental(-40) == fahrenheit::from_fundamental(-40), "Incorrect value for fahrenheit");
	static_assert(sq_millimeter::from_fundamental(1) == 1000000, "Incorrect value for square millimeter");
	static_assert(static_abs(sq_millimeter::to_fundamental(100) - 0.0001) < 0.0000001, "Incorrect value for square millimeter");
	static_assert(units::compare_tag_v<meter, millimeter>, "Incorrect tag comparison");
	static_assert(units::compare_tag_v<meters2, millimeters2>, "Incorrect tag comparison");
	static_assert(units::compare_tag_v<meters2, millimeter>, "Incorrect tag comparison");
	static_assert(units::compare_tag_v<millimeter, meters2>, "Incorrect tag comparison");
	static_assert(units::compare_tag_v<meters2, sq_meter>, "Incorrect tag comparison");
	static_assert(units::compare_exponent_v<meter, millimeter> == 0, "Incorrect exponent comparison");
	static_assert(units::compare_exponent_v<meters2, millimeters2> == 0, "Incorrect exponent comparison");
	static_assert(units::compare_exponent_v<sq_meter, meters2> == 0, "Incorrect exponent comparison");
	static_assert(units::compare_exponent_v<sq_meter, millimeter> == 1, "Incorrect exponent comparison");
	static_assert(units::compare_exponent_v<meters2, sq_meter> == 0, "Incorrect exponent comparison");

	using units::quantity;
	constexpr quantity<meter> m4{ 1 };
	static_assert(m4.value() == 1, "Incorrect unit value");
	constexpr quantity<meter> m5 = quantity<millimeter>{ 500 };
	static_assert(m5.value() == 0.5, "Incorrect unit value");
	constexpr quantity<sq_millimeter> m6 = quantity<sq_meter>{ 3.5 };
	static_assert(m6.value() == 3500000, "Incorrect unit value");

	// the following line should not compile
	// constexpr quantity<sq_millimeter> m7 = quantity<meter>{ 5 };

	using units::delta;
	constexpr delta<meter> m8 = delta<millimeter>{ 50 };
	static_assert(m8.value() == 0.05, "Incorrect delta value");

	constexpr quantity<fahrenheit> f1 = quantity<celsius>{ 10 };
	static_assert(f1.value() == 50, "Incorrect quantity value");

	constexpr delta<fahrenheit> f2 = delta<celsius>{ 10 };
	static_assert(f2.value() == 18, "Incorrect delta value");

	constexpr quantity<sq_meter> m9 = quantity<millimeter>{ 100 } * quantity<meter>{5};
	static_assert(m9.value() == 0.5, "Incorrect square value");

	using m10_t = units::make_compound_t<sq_meter, units::inverse_unit<millimeter>>;
	static_assert(units::similar_units_v<m10_t, meter>, "Incorrect similar units");
	constexpr quantity<meter> m10 = m9 / quantity<millimeter>{100};
	static_assert(m10.value() == 5, "Incorrect div value");
}