#include <iostream>
#include <boost/variant.hpp>

#include <boost/spirit/include/qi.hpp>

typedef boost::variant<int, bool, std::string> frame;

struct visitor : public boost::static_visitor<> {
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

  if (s) boost::apply_visitor(visitor(), frm);


  return 0;
}
