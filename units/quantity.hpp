#pragma once

#include "units.hpp"
#include "detail/unit_comparisons.hpp"
#include "unit_conversion.hpp"
#include "difference_unit.hpp"
#include "compound_unit.hpp"
#include "exponent_unit.hpp"

namespace units
{
	/*!
	 * A quantity represents an absolute amount of a unit of the specified type.
	 * The numeric value is stored in type UnitType::value_type.
	 * Common arithmetic operators are defined for quantity.
	 */
	template<Unit UnitType>
	class quantity
	{
	public:

		using value_type = typename UnitType::value_type;
		using unit_type = UnitType;

		/*!
		 * Default constructor. Takes an optional argument of value_type
		 * to initialize the quantity to a specific value. Otherwise,
		 * it is value initialized.
		 * 
		 * @param value The value to initialize the quantity to.
		 */
		constexpr explicit quantity(value_type value = {})
			: value_{ value }
		{}

		/*!
		 * Copy/conversion constructor. This will automatically convert
		 * quantities of SimilarUnits to this unit type and store the value.
		 * 
		 * @tparam unit The Unit type of the other quantity. Must satisfy SimilarUnit<UnitType, unit>.
		 * @param other The other quantity. The value will be converted to this unit type automatically
		 */
		template<Unit unit>
		constexpr quantity(quantity<unit> other) requires SimilarUnits<UnitType, unit>
			: value_{ unit_conversion<unit, UnitType>::convert(other.value()) }
		{}

		template<Unit unit>
		constexpr quantity& operator=(quantity<unit> const& other) requires SimilarUnits<UnitType, unit>
		{
			value_ = unit_conversion<unit, UnitType>::convert(other.value());
			return *this;
		}

		/*!
		 * Returns the actual value of this quantity.
		 */
		constexpr value_type value() const { return value_; }

	private:

		value_type value_;
	};

	/*!
	 * delta represents a relative quantity of units. Internally, this uses
	 * difference_unit<UnitType> as its unit type. delta provides
	 * common arithmetic operators, both with other deltas and
	 * with quantity.
	 */
	template<Unit UnitType>
	class delta
	{
	public:

		using unit_type = difference_unit_t<UnitType>;
		using base_unit = UnitType;
		using value_type = typename UnitType::value_type;

		/*!
		 * Default constructor for delta. Takes an optional
		 * value_type argument for initializing the value.
		 * Otherwise, it is value initialized.
		 */
		constexpr explicit delta(value_type value = {})
			: value_{ value }
		{}

		/*!
		 * Copy/conversion constructor. This will automatically convert
		 * deltas of SimilarUnits to this unit type and store the value.
		 *
		 * @tparam unit The Unit type of the other delta. Must satisfy SimilarUnit<UnitType, unit>.
		 * @param other The other quantity. The value will be converted to this unit type automatically
		 */
		template<Unit Other>
		requires SimilarUnits<UnitType, Other>
			constexpr delta(delta<Other> other)
			: value_{ unit_conversion<typename delta<Other>::unit_type, unit_type>::convert(other.value()) }
		{}

		/*!
		 * Returns the current value of this delta
		 */
		constexpr value_type value() const { return value_; }

		/*!
		 * Negation operator, returns a delta with the sign of the value flipped.
		 */
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