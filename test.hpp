#include <cstdio>
#include <cstdlib>

// Set to 1 to make the test fail.
#define TEST_FAIL_LIMITS 0
#define TEST_FIX_LIMITS 0
#define TEST_FAIL_ATOMIC 0
#define TEST_FAIL_FUNCTIONAL 0

// These includes work correctly on MSVC.
#include <cuda/std/cassert>
#include <cuda/std/cfloat>
#include <cuda/std/climits>
#include <cuda/std/cstddef>
#include <cuda/std/cstdint>
#include <cuda/std/ctime>
#include <cuda/std/initializer_list>
#include <cuda/std/iterator>
#include <cuda/std/ratio>
#include <cuda/std/type_traits>
#include <cuda/std/utility>
#include <cuda/std/version>

#if TEST_FAIL_LIMITS

#if TEST_FIX_LIMITS
#if defined(_MSC_VER) && _MSC_VER >= 1930
#include <../crt/src/stl/xmath.hpp>
#endif
#endif

// cuda/std/limits uses three variables, _LInf, _LNan, and _LSnan,
// which it expects are declared by MSVC, but are not.

#include <cuda/std/limits>

// All of these headers include <limits>, so using any of them also
// causes compilation to fail.
#include <cuda/std/array>
#include <cuda/std/bit>
#include <cuda/std/ccomplex>
#include <cuda/std/chrono>
#include <cuda/std/cmath>
#include <cuda/std/complex>

#endif

#if TEST_FAIL_ATOMIC

// cuda/std/atomic uses a range of defines, e.g. `__ATOMIC_RELEASE`,
// `ATOMIC_INT_LOCK_FREE`, which it expects are declared by MSVC, but
// are not. It also includes <limits>, causing compilation to fail for
// that reason as well depending on the version.

#include <cuda/std/atomic>

// All of these headers include atomic, so using any of them also
// causes compilation to fail.
#include <cuda/std/barrier>
#include <cuda/std/latch>
#include <cuda/std/semaphore>

#endif

#if TEST_FAIL_FUNCTIONAL

// This header fails with syntax errors on some templated code;
// the code looks correct offhand. It also includes limits.
#include <cuda/std/functional>

// This header includes <functional>.
#include <cuda/std/tuple>

#endif

extern void foo();
