#ifndef MAPBOX_BOOST_SPIRIT_QI_HPP
#define MAPBOX_BOOST_SPIRIT_QI_HPP

#include <mapbox/detail/boost_spirit_attributes.hpp>

#include <boost/spirit/include/qi_alternative.hpp>

namespace boost { namespace spirit { namespace qi { namespace detail
{
    template <typename Expected, class... Types>
    struct find_substitute<mapbox::util::variant<Types...>, Expected>
    {
        // Get the typr from the variant that can be a substitute for Expected.
        // If none is found, just return Expected

        typedef Expected type;
    };
}}}}


#endif
