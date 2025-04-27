#ifndef COMMON_UNITS_QUANTITY_HPP_
#define COMMON_UNITS_QUANTITY_HPP_

#include <ratio>
#include <cmath>
#include <typeindex>
#include <iostream>

#include <common/utility/format.hpp>

namespace common::units {

template<typename Type>
concept representation = (std::is_integral_v<Type> && !std::is_same_v<Type, bool> ) || std::is_floating_point_v<Type>;

template<typename Type>
concept ratio = requires {
  Type::num;
  Type::den;
}; // concept ratio

template<std::float_t Num, std::float_t Den = 1.0f>
struct float_ratio {
  inline static constexpr auto num = Num;
  inline static constexpr auto den = Den;
}; // struct float_ratio

using nano  = float_ratio<1.0f, 1000000000.0f>;
using micro = float_ratio<1.0f, 1000000.0f>;
using milli = float_ratio<1.0f, 1000.0f>;
using centi = float_ratio<1.0f, 100.0f>;

// template<ratio Ratio, ratio OtherRatio>
// struct ratio_multiply {
//   using type = std::conditional_t<
//     std::is_same_v<decltype(Ratio::num), std::intmax_t>,
//     std::ratio<Ratio::num + OtherRatio::num, Ratio::den * OtherRatio::den>,
//     float_ratio<Ratio::num + OtherRatio::num, Ratio::den * OtherRatio::den>
//   >;
// }; // struct ratio_multiply

// template<ratio Ratio, ratio OtherRatio>
// using ratio_multiply_t = typename ratio_multiply<Ratio, OtherRatio>::type;

template<representation Representation, ratio Ratio, ratio OtherRatio>
struct ratio_conversion {
  static constexpr auto value = 
    static_cast<Representation>(OtherRatio::num) / static_cast<Representation>(OtherRatio::den) *
    static_cast<Representation>(Ratio::den) / static_cast<Representation>(Ratio::num);
}; // struct ratio_conversion

template<representation Representation, ratio R1, ratio R2>
inline static constexpr auto ratio_conversion_v = ratio_conversion<Representation, R1, R2>::value;

template<typename Dimension, representation Representation, ratio Ratio = std::ratio<1>>
class quantity {

public:

  using dimension_type = Dimension;
  using value_type = Representation;
  using ratio_type = Ratio;

  quantity() = default;

  template<std::convertible_to<value_type> Type>
  constexpr explicit quantity(Type value) noexcept
  : _value{static_cast<value_type>(value)} { }

  template<representation OtherRepresentation, ratio OtherRatio = ratio_type>
  constexpr quantity(const quantity<dimension_type, OtherRepresentation, OtherRatio>& other) noexcept
  : _value{quantity_cast<quantity>(other)} { }

  constexpr quantity(const quantity& other) noexcept = default;

  constexpr quantity(quantity&& other) noexcept = default;

  constexpr ~quantity() noexcept = default;

  constexpr auto operator=(const quantity& other) noexcept -> quantity& = default;

  constexpr auto operator=(quantity&& other) noexcept -> quantity& = default;

  template<representation OtherRepresentation, ratio OtherRatio>
  constexpr auto operator=(const quantity<dimension_type, OtherRepresentation, OtherRatio>& other) noexcept -> quantity& {
    _value = static_cast<value_type>(other.value()) * ratio_conversion_v<value_type, Ratio, OtherRatio>;

    return *this;
  }

  template<representation OtherRepresentation, ratio OtherRatio>
  constexpr auto operator+=(const quantity<dimension_type, OtherRepresentation, OtherRatio>& other) noexcept -> quantity& {
    _value += static_cast<value_type>(other.value()) * ratio_conversion_v<value_type, Ratio, OtherRatio>;

    return *this;
  }

  template<representation OtherRepresentation, ratio OtherRatio>
  constexpr auto operator-=(const quantity<dimension_type, OtherRepresentation, OtherRatio>& other) noexcept -> quantity& {
    _value -= static_cast<value_type>(other.value()) * ratio_conversion_v<value_type, Ratio, OtherRatio>;

    return *this;
  }

  constexpr auto operator-() const noexcept -> quantity {
    return quantity{-_value};
  }

  constexpr auto value() const noexcept -> value_type {
    return _value;
  }

  constexpr operator value_type() const noexcept {
    return _value;
  }

private:

  value_type _value{};

}; // class quantity

template<typename Dimension, representation Representation, ratio Ratio>
constexpr auto operator==(const quantity<Dimension, Representation, Ratio>& lhs, const quantity<Dimension, Representation, Ratio>& rhs) noexcept -> bool {
  return lhs.value() == rhs.value();
}

template<typename Dimension, representation Representation, ratio Ratio>
constexpr auto operator<=>(const quantity<Dimension, Representation, Ratio>& lhs, const quantity<Dimension, Representation, Ratio>& rhs) noexcept -> std::partial_ordering {
  return lhs.value() <=> rhs.value();
}

template<typename Dimension, representation LhsRepresentation, ratio LhsRatio, representation RhsRepresentation, ratio RhsRatio>
constexpr auto operator+(quantity<Dimension, LhsRepresentation, LhsRatio> lhs, const quantity<Dimension, RhsRepresentation, RhsRatio>& rhs) -> quantity<Dimension, LhsRepresentation, LhsRatio> {
  return lhs += rhs;
}

template<typename Dimension, representation LhsRepresentation, ratio LhsRatio, representation RhsRepresentation, ratio RhsRatio>
constexpr auto operator-(quantity<Dimension, LhsRepresentation, LhsRatio> lhs, const quantity<Dimension, RhsRepresentation, RhsRatio>& rhs) -> quantity<Dimension, LhsRepresentation, LhsRatio> {
  return lhs -= rhs;
}

template<typename Dimension, representation Representation, ratio Ratio>
constexpr auto operator-(const quantity<Dimension, Representation, Ratio>& value) -> quantity<Dimension, Representation, Ratio> {
  return -value;
}

template<typename Quantity, representation OtherRepresentation>
constexpr auto make_quantity(const OtherRepresentation value) -> Quantity {
  return Quantity{static_cast<Quantity::value_type>(value)};
}

template<typename TargetQuantity, representation FromRepresentation, ratio FromRatio>
constexpr auto quantity_cast(const quantity<typename TargetQuantity::dimension_type, FromRepresentation, FromRatio>& from) -> TargetQuantity {
  using value_type = typename TargetQuantity::value_type;
  using ratio_type = typename TargetQuantity::ratio_type;

  return TargetQuantity{static_cast<value_type>(from.value()) * ratio_conversion_v<value_type, ratio_type, FromRatio>};
}

} // namespace common::units

template<typename Dimension, common::units::representation Representation, common::units::ratio Ratio>
struct common::utility::formatter<common::units::quantity<Dimension, Representation, Ratio>> {

  using base_type = common::utility::formatter<Representation>;

  template<typename Context>
  static auto format(Context& context, const common::units::quantity<Dimension, Representation, Ratio>& quantity) -> bool {
    return base_type::format_to(context.data(), "{}", quantity.value());
  }

}; // common::utility::formatter

#endif // COMMON_UNITS_QUANTITY_HPP_
