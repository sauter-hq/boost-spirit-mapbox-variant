#include <iostream>
#include <mapbox/variant.hpp>
#include <boost/mpl/bool.hpp>

typedef mapbox::util::variant<int, bool, std::string> frame;

template <class T, class... Types>
struct is_in_type_list;

template <class T, class... Types>
struct is_in_type_list<T, mapbox::util::variant<Types...>> {

  using variant = mapbox::util::variant<Types...>;
  using direct_type = mapbox::util::detail::direct_type<T, Types...>;

  using type = boost::mpl::bool_<(direct_type::index !=  mapbox::util::detail::invalid_value)>;
  enum { value = type::value };

  enum { index = direct_type::index };
};




int main() {
  {
    is_in_type_list<bool, frame> o;
    std::cout << " bool : " << o.value << std::endl;
  }

  {
    is_in_type_list<std::string, frame> o;
    std::cout << " string : " << o.value << std::endl;
  }

  {
    is_in_type_list<float, frame> o;
    std::cout << " float : " << o.value << std::endl;
  }
}
