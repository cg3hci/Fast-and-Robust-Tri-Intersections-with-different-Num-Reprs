# A Robust Approach to Detect Intersections between Triangles with Different Numerical Representations

Official code accompanying the paper:

**A Robust Approach to Detect Intersections between Triangles with Different Numerical Representations**  
Luca Garau and Gianmarco Cherchi  
WSCG 2025 — Full Papers Proceedings

**[Paper / DOI](https://doi.org/10.24132/CSRN.2025-7)** · **[Open-access publication](https://dspace.zcu.cz/items/09cc6e94-23cc-4f9f-ab20-8432eb64264d)**

## Overview

This repository implements the triangle–triangle intersection detection and
classification method described in the paper.

The approach supports floating-point coordinates, rational coordinates, and
implicit point representations through a templated C++ interface. It identifies
the features involved in an intersection and records their connectivity,
including coplanar configurations.

The repository provides:
- the intersection implementation and geometric predicate wrappers;
- an example application with three numerical representation modes;
- an interactive viewer for inspecting the example triangles and their intersections.

Read the paper for the algorithm, its numerical foundations, and the experimental evaluation.

## Requirements

The supplied example requires:

- a C++20 compiler;
- CMake 3.27 or newer;
- [CinoLib](https://github.com/mlivesu/cinolib);
- [CGAL](https://www.cgal.org/), with GMP and MPFR;
- the graphics dependencies required by CinoLib's OpenGL/GLFW/ImGui viewer.

The supplied CMake configuration enables CinoLib support for Shewchuk predicates,
Indirect Predicates, and CGAL/GMP/MPFR.

## Build

Clone this repository:

```sh
git clone https://github.com/cg3hci/Fast-and-Robust-Tri-Intersections-with-different-Num-Reprs.git
cd Fast-and-Robust-Tri-Intersections-with-different-Num-Reprs
```

Install the dependencies above. In `CMakeLists.txt`, replace the placeholder in:

```cmake
set(cinolib_DIR <YOUR CINOLIB PATH HERE>)
```

with the path to your local CinoLib checkout, for example:

```cmake
set(cinolib_DIR "/absolute/path/to/cinolib")
```

Configure and build:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

If CMake cannot locate CGAL, specify `CGAL_DIR` when configuring, pointing to
the directory containing `CGALConfig.cmake`.

## Run the example

The executable requires exactly one argument selecting the numerical representation:

| Argument | Representation |
| --- | --- |
| `FP` | Floating-point coordinates (`double`) |
| `RATIONAL` | Rational coordinates (`CGAL::Gmpq`) |
| `IMPLICIT` | Indirect Predicates point interface (`genericPoint`) |

For a single-configuration build:

```sh
./build/triangle_intersections FP
./build/triangle_intersections RATIONAL
./build/triangle_intersections IMPLICIT
```

For a Windows build using a multi-configuration generator, the executable
is typically located in `build/Release`:

```powershell
.\build\Release\triangle_intersections.exe FP
```

The application prints intersection information to the terminal and opens an
interactive viewer.

**The example uses triangles defined in `main.cpp`; it does not load mesh files.**
To try other inputs, edit the corresponding point definitions and rebuild.
The `IMPLICIT` example initializes its input vertices as `explicitPoint3D`
objects accessed through the `genericPoint` interface.

Rational and implicit coordinates are converted to floating point for display.
The visualization therefore shows approximate coordinates.

## Use in your own project

The main entry point is `tri_intersect_tri`, declared in `tri_tri_intersect.h`.
It takes the three vertices of the first triangle followed by the three
vertices of the second triangle.

For example, after configuring the same dependencies as the supplied application:

```cpp
#include <iostream>
#include "tri_tri_intersect.h"

int main()
{
    initFPU();

    double a0[3] = {0.0, 0.0, 0.0};
    double a1[3] = {2.0, 0.0, 0.0};
    double a2[3] = {0.0, 2.0, 0.0};

    double b0[3] = {0.5, 0.5, -1.0};
    double b1[3] = {0.5, 0.5,  1.0};
    double b2[3] = {1.5, 0.5,  0.0};

    intersectionResult result =
        tri_intersect_tri(a0, a1, a2, b0, b1, b2);

    std::cout << "Intersect: " << result.intersect << '\n';
    result.printIntersectionPoints();
    result.printIntersectionEdges();
}
```

Use a consistent representation for all six input vertices in each call.
For rational coordinates, provide arrays of `CGAL::Gmpq`; for the implicit
point interface, follow the `genericPoint` pointer example in `main.cpp`.

The headers include their corresponding implementation files. Keep
`tri_tri_intersect.cpp` and `orientTemplated.cpp` alongside the headers
when incorporating the implementation into your project.

### Understanding the result

`intersectionResult` contains:

| Member | Meaning |
| --- | --- |
| `intersect` | Whether an intersection was detected |
| `coplanar` | Whether the triangles are coplanar |
| `intersectionsPoints` | Flat sequence of `(type, index0, index1)` records |
| `intersectionsEdges` | Flat sequence of pairs referencing intersection point records |

Point records classify the input features involved; they do not store XYZ
coordinates. In the type names, `A` and `B` denote the first and second
triangles, while `V`, `E`, and `T` denote a vertex, edge, and triangle.

Vertex indices are local to each triangle (`0`, `1`, `2`). Edge `i` connects
vertex `i` to vertex `(i + 1) % 3`. Unused feature indices are stored as `-1`.

Use `getIntersectionPointsSize()` and `getIntersectionEdgesSize()` to obtain
record counts, or the print methods to inspect them.

## Numerical representations

The orientation wrappers in `orientTemplated.h` and `orientTemplated.cpp`
select the predicate implementation according to the input type:

- `double`: CinoLib's interface to Shewchuk predicates;
- `genericPoint`: Indirect Predicates;
- other compatible numeric types: determinant evaluation using that type's arithmetic.

Supporting an additional type requires compatible arithmetic, comparisons,
and point operations. The numerical guarantees depend on the chosen
representation and predicates; templating alone does not make an arbitrary
numeric type exact.

## Citation

If you use this implementation in research, please cite the accompanying paper:

```bibtex
@inproceedings{garau2025robust,
  author    = {Garau, Luca and Cherchi, Gianmarco},
  title     = {A Robust Approach to Detect Intersections between
               Triangles with Different Numerical Representations},
  booktitle = {WSCG 2025: Full Papers Proceedings},
  year      = {2025},
  doi       = {10.24132/CSRN.2025-7},
  url       = {https://doi.org/10.24132/CSRN.2025-7}
}
```

## License

This repository is distributed under the [MIT License](LICENSE).
Third-party dependencies retain their respective licenses.
