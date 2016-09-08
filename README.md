# mapbox::variant for Boost.Spirit Qi and Boost.Spirit Karma
This defines the traits you need to use mapbox::variant in place of Boost.Variant with Boost.Spirit

## HowTo: Qi
```cpp
#include <mapbox/variant.hpp>
#include <mapbox/boost_spirit_qi.hpp>
#include <boost/spirit/include/qi.hpp>

...

using namespace boost::spirit::qi;
rule<Iterator, mapbox::util::variant<int, bool>()> r = int_ | bool_;
```

## HowTo: Karma
```cpp
#include <mapbox/variant.hpp>
#include <mapbox/boost_spirit_karma.hpp>
#include <boost/spirit/include/karma.hpp>

...

using namespace boost::spirit::karma;
rule<Iterator, mapbox::util::variant<int, bool>()> r = int_ | bool_;
```


### Benchmark

The same program with Boost.Spirit and a MapBox.Variant instead of a Boost.Variant results in good improvements in terms of Binary Sizes.

Compile times improvements are not really measurable, but as it reduces binary sizes it should have an impact on link time, but this wasn't measured.

## Binary sizes
|                    | Boost.Variant | MapBox.Variant |
|--------------------|---------------|----------------|
| Boost.Spirit Qi    | 75 Kb         | 63 Kb          |
| Boost.Spirit Karma | 119 Kb        | 83 Kb          |

## Compile Times
Benchmark PC: Dell Precision 5510, 8 Cores Intel(R) Core(TM) i7-6820HQ CPU @ 2.70GHz + SSD Hard Disk + 32 GB RAM

|                    | Boost.Variant | MapBox.Variant |
|--------------------|---------------|----------------|
| Boost.Spirit Qi    | 3.624s        | 3.693s         |
| Boost.Spirit Karma | 3.940s        | 3.821s         |
