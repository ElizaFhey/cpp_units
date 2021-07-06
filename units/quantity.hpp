#pragma once

#include "units.hpp"
#include "detail/unit_comparisons.hpp"
#include "unit_conversion.hpp"
#include "difference_unit.hpp"
#include "compound_unit.hpp"
#include "exponent_unit.hpp"

namespace units
{
	template<Unit UnitType>
	class quantity
	{
	public:

		using value_type = typename UnitType::value_type;
		using unit_type = UnitType;

		constexpr explicit quantity(value_type value = {})
			: value_{ value }
		{}

		template<Unit unit>
		constexpr quantity(quantity<unit> other) requires SimilarUnits<UnitType, unit>
			: value_{ unit_conversion<unit, UnitType>::convert(other.value()) }
		{}

		constexpr value_type value() const { return value_; }

	private:

		value_type value_;
	};

	template<Unit UnitType>
	class delta
	{
	public:

		using unit_type = difference_unit_t<UnitType>;
		using base_unit = UnitType;
		using value_type = typename UnitType::value_type;

		constexpr explicit delta(value_type value = {})
			: value_{ value }
		{}

		template<Unit Other>
		requires SimilarUnits<UnitType, Other>
			constexpr delta(delta<Other> other)
			: value_{ unit_conversion<typename delta<Other>::unit_type, unit_type>::convert(other.value()) }
		{}

		constexpr value_type value() const { return value_; }

		constexpr delta<UnitType> operator-() const { return delta<UnitType>{-value_}; }

	private:

		value_type value_;
	};

	template<Unit A, Unit B>
	constexpr inline delta<A> operator+(delta<A> a, delta<B> b) requires SimilarUnits<A, B>
	{
		return delta<A>{a.value() + delta<A>{b}.value()};
	}

	template<Unit A, Unit B>
	constexpr inline delta<A> operator-(delta<A> a, delta<B> b) requires SimilarUnits<A, B>
	{
		return delta<A>{a.value() - delta<A>{b}.value()};
	}

	template<Unit A, Unit B>
	constexpr inline quantity<A> operator+(quantity<A> a, delta<B> b) requires SimilarUnits<A, B>
	{
		return quantity<A>{a.value() + delta<A>{b}.value()};
	}

	template<Unit A, Unit B>
	constexpr inline quantity<B> operator+(delta<A> a, quantity<B> b) requires SimilarUnits<A, B>
	{
		return quantity<B>{b.value() + delta<B>{a}.value()};
	}

	template<Unit A, Unit B>
	constexpr inline quantity<A> operator-(quantity<A> a, delta<B> b) requires SimilarUnits<A, B>
	{
		return quantity<A>{a.value() - delta<A>{b}.value()};
	}

	template<Unit A, Unit B>
	constexpr inline delta<A> operator-(quantity<A> a, quantity<B> b) requires SimilarUnits<A, B>
	{
		return delta<A>{a.value() - quantity<A>{b}.value()};
	}

	template<Unit A, Unit B>
	constexpr inline quantity<make_compound_t<A, B>> operator*(quantity<A> a, quantity<B> b)
	{
		return quantity<make_compound_t<A, B>>{a.value() * b.value()};
	}

	template<Unit A, Unit B>
	constexpr inline delta<make_compound_t<A, B>> operator*(delta<A> a, delta<B> b)
	{
		return delta<make_compound_t<A, B>>{a.value() * b.value()};
	}

	template<Unit A, Unit B>
	constexpr inline auto operator/(quantity<A> a, quantity<B> b)
		-> decltype(a * std::declval<quantity<inverse_unit<B>>>())
	{
		return a * quantity<inverse_unit<B>>{typename B::value_type{ 1 } / b.value()};
	}
}