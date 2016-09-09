#ifndef MAPBOX_TRAITS_IS_MAPBOX_VARIANT_HPP
#define MAPBOX_TRAITS_IS_MAPBOX_VARIANT_HPP

#include <mapbox/variant.hpp>

namespace mapbox { namespace traits { 

  template <class... Types>
  struct is_mapbox_variant {
    using type = std::false_type;
    enum { value = false };
  };

  template <class... Types>
  struct is_mapbox_variant<mapbox::util::variant<Types...>> {
    using type = std::true_type;
    enum { value = true };
  };

  template <class... Types>
  struct is_mapbox_variant<const mapbox::util::variant<Types...>> {
    using type = std::true_type;
    enum { value = true };
  };


}}

#endif
