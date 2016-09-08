# Using Mapbox Variant insteads of Boost.Variant

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
