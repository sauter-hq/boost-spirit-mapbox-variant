#ifndef MAPBOX_TRAITS_IS_TYPE_IN_VARIANT_HPP
#define MAPBOX_TRAITS_IS_TYPE_IN_VARIANT_HPP

#include <mapbox/variant.hpp>

namespace mapbox { namespace traits { 

  template <class T, class... Types>
  struct is_type_in_variant;

  template <class T, class... Types>
  struct is_type_in_variant<T, mapbox::util::variant<Types...>> {

    using variant = mapbox::util::variant<Types...>;
    using direct_type = mapbox::util::detail::direct_type<T, Types...>;

    using type = std::integral_constant<bool, (direct_type::index !=  mapbox::util::detail::invalid_value)>;
    enum { value = type::value };

    enum { index = direct_type::index };
    enum { size = sizeof...(Types) };
  };

}}

#endif
