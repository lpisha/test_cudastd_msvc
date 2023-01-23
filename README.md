# Test `cuda::std` MSVC

Reproducer for a bug in the CUDA Standard Library (`libcudacxx`) where C++ (not CUDA) compilation fails on MSVC.

### What is the bug

This is in the context of projects which contain both C++ and CUDA source files, where the C++ files are compiled with MSVC and the CUDA files with nvcc. The project uses the CUDA Standard Library across host and device code, so common headers which get included by both .cpp and .cu files include parts of the CUDA Standard Library.

During C++ compilation, some headers in the CUDA Standard Library expect that MSVC will have declared / defined certain symbols, but in certain MSVC / Visual Studio versions it has not, causing compilation errors. Depending on the MSVC version, many of the headers in the CUDA Standard Library include these problematic headers, making much of the library unusable.

This bug does not arise in the compilation of .cu files with nvcc, only in the compilation of .cpp files with MSVC.

### How did this happen: `<cuda/std/limits>`

Apparently, Microsoft moved the relevant symbols from `<ymath.h>` to `<stl/xmath.hpp>` between Visual Studio 16 2019 (toolset v142) and Visual Studio 17 2022 (toolset v143). On my machine:
- The symbols are in the old version at `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\include\ymath.h`
- The symbols are missing from the new version at `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.34.31933\include\ymath.h`
- The symbols are in the new version at `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.34.31933\crt\src\stl\xmath.hpp`

So, when building with Visual Studio 15 2017 (toolset v141) or Visual Studio 16 2019 (toolset v142), these symbols are defined and building is successful. On Visual Studio 17 2022 (toolset v143), these symbols are no longer defined and the compile fails. Adding `#include <../crt/src/stl/xmath.hpp>` allows the compile to succeed, though it's not clear if this should go into production exactly like this.

### How did this happen: `<cuda/std/atomic>`

I'm not able to find any mention of `__ATOMIC_RELEASE` in any of the Microsoft / Windows headers on my machine, so it appears that this code should never have worked.

Edit: Fixed on top-of-tree `libcudacxx`.

### How did this happen: `<cuda/std/functional>`

This issue only arose after fixing the `<limits>` issue. VS 2022 gives multiple syntax errors around line 3026 of `<cuda/std/functional>` (implementation of `__search`). The code looks fine by eye and works with VS 2017 and VS 2019. Perhaps now the symbols `_D1` and `_D2` are already defined elsewhere?

### How did this happen: `<cuda/std/type_traits>`

This fails only in VS 2017: `<cuda/std/detail/libcxx/include/__type_traits/disjunction.h>`: line 47, "error C2210: '_First': pack expansions cannot be used as arguments to non-packed parameters in alias templates". Not exactly sure what the issue is here.

### Version information

Windows 10 x64, CUDA 12.0.76, driver 527.41.

Changing Windows SDK versions without changing toolset versions did not appear to change behavior. (10.0.20348.0, 10.0.18362.0, 10.0.17763.0)

All of the tests compile and work correctly on Linux.

### How to build / use

By default, compilation will succeed on both Windows and Linux. To test the different sets of headers, set any of the following defines to 1 in `test.hpp`:
- `TEST_TYPETRAITS` - fails on VS 2017, OK on VS 2019 and 2022
- `TEST_LIMITS` - OK on VS 2017 and 2019, fails on VS 2022
- `TEST_FIX_LIMITS` - fixes limits on VS 2022
- `TEST_ATOMIC` - with top-of-tree, works on all versions now
- `TEST_FUNCTIONAL` - fails on VS 2022

Build with CMake as usual.

If desired, once the Visual Studio solution / projects have been generated, right-click on the test_cudastd_msvc project, click Properties, and change the toolset from Visual Studio 17 2022 to an earlier version you have installed.
