#include <iostream>
#include <mapbox/variant.hpp>

//namespace boost {
//  using mapbox::util::get;
//}

#include <boost/spirit/include/karma.hpp>

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


namespace boost { namespace spirit { namespace traits
{
    template <typename Domain>
    struct not_is_variant<frame, Domain>
      : mpl::false_
    {};
}
}}

//

namespace boost { namespace spirit { namespace traits {

    ///////////////////////////////////////////////////////////////////////////
    // The compute_compatible_component_variant for mapbox::util::variant
    ///////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(first_type)
    }

    template <typename Variant, typename Expected, typename Domain>
    struct compute_compatible_component_varianta
    {
        typedef typename traits::variant_type<Variant>::type variant_type;
        using type = is_in_type_list<Expected, Variant>;
        enum { value = type::value };

        // return the type in the variant the attribute is compatible with
        typedef typename
            mpl::eval_if<type, mpl::identity<Expected>, mpl::identity<unused_type> >::type
        compatible_type;

        // return whether the given type is compatible with the Expected type
        static bool is_compatible(int which)
        {

            std::cout << "OOK : " << v.which() << std::endl;
            return which == type::index;
        }
    };

    template <typename Expected, typename Attribute, typename Domain>
    struct compute_compatible_component_a;

    template <typename Expected,  typename Domain>
    struct compute_compatible_component_a<Expected, frame, Domain>
      : compute_compatible_component_varianta<frame, Expected, Domain> {};


    ///////////////////////////////////////////////////////////////////////////
    // return the type currently stored in the given variant
    template <class... Types>
    struct variant_which<mapbox::util::variant<Types...> >
    {
        static int call(mapbox::util::variant<Types...> const& v)
        {
            std::cout << "OOK : " << v.which() << std::endl;
            return v.which();
        }
    };

}}}

//
//
//
//

namespace boost { namespace spirit { namespace karma { namespace detail
{


   template <typename Component, typename Expected>
    struct alternative_generate<Component, frame, Expected
      , typename std::enable_if< not traits::compute_compatible_component_varianta<frame, Expected, domain>::value > >
            /*traits::compute_compatible_component<Expected, Attribute, karma::domain> >::type*/
    {

        typedef frame Attribute;

        template <typename OutputIterator, typename Context, typename Delimiter>
        static bool
        call(Component const& component, OutputIterator& sink
          , Context& ctx, Delimiter const& d, Attribute const& attr, bool&)
        {
          std::cout << "oops" << std::endl;
          return false;
        }

    };


    //  this gets instantiated if the Attribute type is compatible to the
    //  generator
    template <typename Component, typename Expected>
    struct alternative_generate<Component, frame, Expected
      , typename enable_if< typename traits::compute_compatible_component_varianta<frame, Expected, domain>::type >::type*

      //, typename enable_if< is_same<frame, Attribute> >::type
            /*traits::compute_compatible_component<Expected, Attribute, karma::domain> >::type*/>
    {
        typedef frame Attribute;

        template <typename OutputIterator, typename Context, typename Delimiter>
        static bool
        call(Component const& component, OutputIterator& sink
          , Context& ctx, Delimiter const& d, Attribute const& attr, bool&)
        {
          std::cout << "Got called in specialization !" << std::endl;
            return call(component, sink, ctx, d, attr
              , spirit::traits::not_is_variant<Attribute, karma::domain>());
        }

        template <typename OutputIterator, typename Context, typename Delimiter>
        static bool
        call(Component const& component, OutputIterator& sink
          , Context& ctx, Delimiter const& d, Attribute const& attr, mpl::true_)
        {
          std::cout << "Got called in specialization 1 !" << std::endl;
            return component.generate(sink, ctx, d, attr);
        }

        template <typename OutputIterator, typename Context, typename Delimiter>
        static bool
        call(Component const& component, OutputIterator& sink
          , Context& ctx, Delimiter const& d, Attribute const& attr, mpl::false_)
        {
          std::cout << "Got called in specialization 2 !" << std::endl;
            typedef
                traits::compute_compatible_component_varianta<Attribute, Expected, domain>
            component_type;

            // if we got passed an empty optional, just fail generation
            if (!traits::has_optional_value(attr))
                return false;

            // make sure, the content of the passed variant matches our
            // expectations
            typename traits::optional_attribute<Attribute>::type attr_ = 
                traits::optional_value(attr);
            if (!component_type::is_compatible(spirit::traits::which(attr_)))
                return false;

            // returns true if any of the generators succeed
            typedef typename component_type::compatible_type compatible_type;
            return component.generate(sink, ctx, d
              , mapbox::util::get<compatible_type>(attr_));
        }

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
  typedef std::back_insert_iterator<std::string> Iterator;

  using namespace boost::spirit::karma;

  rule<Iterator, frame()> r = 
      int_
    | bool_
    | *char_
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
    frame frm = std::string{"Ma grosse banana"};
    std::string buf;
    Iterator iter(buf);
    auto s = generate(iter, r, frm); 

    std::cout << " success : " << s << " - " << mapbox::util::get<std::string>(frm) << " - " << buf << std::endl;
  }

  return 0;
}
