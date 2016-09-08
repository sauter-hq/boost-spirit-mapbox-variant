#include <iostream>
#include <mapbox/variant.hpp>

namespace boost {
  using mapbox::util::get;
}


#include <boost/spirit/include/karma.hpp>

typedef mapbox::util::variant<int, bool, std::string, float, long, uint64_t> frame;

template <class... Types>
struct is_mapbox_variant {
  using type = std::false_type;
};

template <class... Types>
struct is_mapbox_variant<mapbox::util::variant<Types...>> {
  using type = std::true_type;
};

template <class T, class... Types>
struct is_in_type_list;

template <class T, class... Types>
struct is_in_type_list<T, mapbox::util::variant<Types...>> {

  using variant = mapbox::util::variant<Types...>;
  using direct_type = mapbox::util::detail::direct_type<T, Types...>;

  using type = boost::mpl::bool_<(direct_type::index !=  mapbox::util::detail::invalid_value)>;
  enum { value = type::value };

  enum { index = direct_type::index };
  enum { size = sizeof...(Types) };
};


namespace boost { namespace spirit { namespace traits
{
    template <typename Domain, class... Types>
    struct not_is_variant<mapbox::util::variant<Types...>, Domain>
      : mpl::false_
    {};
}
}}

namespace boost { namespace spirit { namespace traits {

    template <class... Types>
    struct variant_which< mapbox::util::variant<Types...> >
    {
        static int call(mapbox::util::variant<Types...> const& v)
        {
            return v.which();
        }
    };

}}}

namespace boost { namespace spirit { namespace traits {

    template <typename Variant, typename Expected>
    struct compute_compatible_component_variant<Variant, Expected, mpl::false_
      , typename enable_if< typename is_mapbox_variant<Variant>::type >::type>
    {
        typedef typename traits::variant_type<Variant>::type variant_type;

        // true_ if the attribute matches one of the types in the variant
        typedef is_in_type_list<Expected, Variant> type;
        enum { value = type::value };

        // return the type in the variant the attribute is compatible with
        typedef typename
            mpl::eval_if<type, mpl::identity<Expected>, mpl::identity<unused_type> >::type
        compatible_type;

        // return whether the given type is compatible with the Expected type
        static bool is_compatible(int which)
        {
          auto idx = (type::size - 1 - type::index); // Typelist is inverted and 0-based
          return which == idx;
        }
    };


}}}


struct visitor {
    template <typename T>
    void operator()(T const& val) const
    {
      std::cout << typeid(T).name() << " " << val << std::endl;
    }

};

int main(int argc, char** argv) {
  typedef std::back_insert_iterator<std::string> Iterator;

  using namespace boost::spirit::karma;

  rule<Iterator, std::string()> mystring = *char_;

  rule<Iterator, frame()> r = 
      int_
    | bool_
    | mystring
  ;

  {
    frame frm = 12;
    std::string buf;
    Iterator iter(buf);
    auto s = generate(iter, r, frm); 

    std::cout << " success : " << s << " - " << mapbox::util::get<int>(frm) << " - " << buf << std::endl;
  }

  {
    frame frm = true;
    std::string buf;
    Iterator iter(buf);
    auto s = generate(iter, r, frm); 

    std::cout << " success : " << s << " - " << mapbox::util::get<bool>(frm) << " - " << buf << std::endl;
  }

  {
    frame frm = std::string{"this is a string"};
    std::string buf;
    Iterator iter(buf);
    auto s = generate(iter, r, frm); 

    std::cout << " success : " << s << " - " << mapbox::util::get<std::string>(frm) << " - " << buf << std::endl;
  }

  return 0;
}
