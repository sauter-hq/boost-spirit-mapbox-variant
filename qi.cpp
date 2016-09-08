#include <iostream>
#include <mapbox/variant.hpp>

#include <boost/spirit/include/qi.hpp>

typedef mapbox::util::variant<int, bool, std::string> frame;

namespace boost { namespace spirit { namespace traits
{
    template <typename Domain, class... Types>
    struct not_is_variant<mapbox::util::variant<Types...>, Domain>
      : mpl::false_
    {};
}
}}


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




struct visitor {
    template <typename T>
    void operator()(T const& val) const
    {
      std::cout << typeid(T).name() << " " << val << std::endl;
    }

};

int main(int argc, char** argv) {
  typedef std::string::const_iterator Iterator;

  using namespace boost::spirit::qi;

  rule<Iterator, frame()> r = 
      int_
    | bool_
    | as_string[*char_]
  ;

  frame frm{};
  auto text = std::string{argv[1]};
  auto s = parse(text.cbegin(), text.cend(), r, frm); 

  if (s) mapbox::util::apply_visitor(visitor(), frm);


  return 0;
}
