#include "../units/units.hpp"
#include "../units/fundamental_unit.hpp"
#include "../units/linear_unit.hpp"
#include "../units/exponent_unit.hpp"
#include "../units/compound_unit.hpp"
#include "../units/quantity.hpp"

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
	struct second : units::fundamental_unit<second, double> {};

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

	using velocity = units::make_compound_t<meter, units::inverse_unit<second>>;
	using acceleration = units::make_compound_t<velocity, units::inverse_unit<second>>;
	static_assert(std::is_same<acceleration, units::compound_unit<meter, units::inverse_unit<second>, units::inverse_unit<second>>>::value, "Incorrect acceleration type");

	using velo1 = units::make_compound_t<second, acceleration>;
	using velo2 = units::make_compound_t<acceleration, second>;
	static_assert(units::similar_units_v<velo1, velocity>, "Incorrect velocity type");
	static_assert(units::similar_units_v<velo2, velocity>, "Incorrect velocity type");

	constexpr auto p1 = delta<second>{ 1 } *delta<second>{1} * quantity<acceleration>{ 1 };
	constexpr auto p2 = delta<second>{ 1 } *quantity<velocity>{1};
	using p1_t = decltype(p1);
	using p2_t = decltype(p2);
	constexpr std::intmax_t diff = units::compare_exponent_v<typename p1_t::unit_type, meter>;
	constexpr bool cmp = units::compare_tag_v<typename p1_t::unit_type, meter>;
	constexpr bool cmp1 = units::detail::has_unit_tag_v<meter, typename p1_t::unit_type::units>;
	static_assert(units::similar_units_v<typename p1_t::unit_type, typename p2_t::unit_type>, "Incorrect position type");
	static_assert(units::similar_units_v<meter, units::difference_unit_t<meter>>, "Incorrect difference type");
	using p1_sum = units::detail::sum_exponent<second, units::detail::pop_list_t<units::detail::find_unit_tag_t<second, typename p1_t::unit_type::units>>>;
	static_assert(p1_sum::value == 0, "Incorrect exponent sum");
	static_assert(units::similar_units_v<typename p1_t::unit_type, meter>, "Incorrect position type");
	static_assert(units::similar_units_v<typename p2_t::unit_type, meter>, "Incorrect position type");
}