#include <iostream>

#include <mapbox/variant.hpp>
#include <mapbox/boost_spirit_karma.hpp>
#include <boost/spirit/include/karma.hpp>

typedef mapbox::util::variant<int, bool, std::string, float, long, uint64_t> frame;

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
