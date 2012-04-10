/*  This file is part of the Vc library.

    Copyright (C) 2010 Matthias Kretz <kretz@kde.org>

    Vc is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Vc is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Vc.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef VC_COMMON_MACROS_H
#define VC_COMMON_MACROS_H
#undef VC_COMMON_UNDOMACROS_H

#include <Vc/global.h>

#ifdef VC_MSVC
# define ALIGN(n) __declspec(align(n))
# define STRUCT_ALIGN1(n) ALIGN(n)
# define STRUCT_ALIGN2(n)
# define ALIGNED_TYPEDEF(n, _type_, _newType_) typedef ALIGN(n) _type_ _newType_
#else
# define ALIGN(n) __attribute__((aligned(n)))
# define STRUCT_ALIGN1(n)
# define STRUCT_ALIGN2(n) ALIGN(n)
# define ALIGNED_TYPEDEF(n, _type_, _newType_) typedef _type_ _newType_ ALIGN(n)
#endif

#define FREE_STORE_OPERATORS_ALIGNED(alignment) \
        void *operator new(size_t size) { return _mm_malloc(size, alignment); } \
        void *operator new(size_t, void *p) { return p; } \
        void *operator new[](size_t size) { return _mm_malloc(size, alignment); } \
        void operator delete(void *ptr, size_t) { _mm_free(ptr); } \
        void operator delete[](void *ptr, size_t) { _mm_free(ptr); }

#ifdef VC_CLANG
#  define INTRINSIC __attribute__((always_inline))
#  define INTRINSIC_L
#  define INTRINSIC_R INTRINSIC
#  define FLATTEN
#  define CONST __attribute__((const))
#  define CONST_L
#  define CONST_R CONST
#  define PURE __attribute__((pure))
#  define MAY_ALIAS __attribute__((may_alias))
#  define ALWAYS_INLINE __attribute__((always_inline))
#  define ALWAYS_INLINE_L
#  define ALWAYS_INLINE_R ALWAYS_INLINE
#  define VC_IS_UNLIKELY(x) __builtin_expect(x, 0)
#  define VC_IS_LIKELY(x) __builtin_expect(x, 1)
#  define VC_RESTRICT __restrict__
#elif defined(__GNUC__)
#  define INTRINSIC __attribute__((__flatten__, __always_inline__, __artificial__))
#  define INTRINSIC_L
#  define INTRINSIC_R INTRINSIC
#  define FLATTEN __attribute__((__flatten__))
#  define CONST __attribute__((__const__))
#  define CONST_L
#  define CONST_R CONST
#  define PURE __attribute__((__pure__))
#  define MAY_ALIAS __attribute__((__may_alias__))
#  define ALWAYS_INLINE __attribute__((__always_inline__))
#  define ALWAYS_INLINE_L
#  define ALWAYS_INLINE_R ALWAYS_INLINE
#  define VC_IS_UNLIKELY(x) __builtin_expect(x, 0)
#  define VC_IS_LIKELY(x) __builtin_expect(x, 1)
#  define VC_RESTRICT __restrict__
#else
#  define FLATTEN
#  ifdef PURE
#    undef PURE
#  endif
#  define PURE
#  define MAY_ALIAS
#  ifdef VC_MSVC
#    define ALWAYS_INLINE __forceinline
#    define ALWAYS_INLINE_L ALWAYS_INLINE
#    define ALWAYS_INLINE_R
#    define CONST __declspec(noalias)
#    define CONST_L CONST
#    define CONST_R
#    define INTRINSIC __forceinline
#    define INTRINSIC_L INTRINSIC
#    define INTRINSIC_R
#  else
#    define ALWAYS_INLINE
#    define ALWAYS_INLINE_L
#    define ALWAYS_INLINE_R
#    define CONST
#    define CONST_L
#    define CONST_R
#    define INTRINSIC
#    define INTRINSIC_L
#    define INTRINSIC_R
#  endif
#  define VC_IS_UNLIKELY(x) x
#  define VC_IS_LIKELY(x) x
#  define VC_RESTRICT __restrict
#endif

#ifdef VC_GCC
# define VC_WARN_INLINE
# define VC_WARN(msg) __attribute__((warning("\n\t" msg)))
#else
# define VC_WARN_INLINE inline
# define VC_WARN(msg)
#endif

#define CAT_HELPER(a, b) a##b
#define CAT(a, b) CAT_HELPER(a, b)

#define CAT3_HELPER(a, b, c) a##b##c
#define CAT3(a, b, c) CAT3_HELPER(a, b, c)

#define unrolled_loop16(_it_, _start_, _end_, _code_) \
if (_start_ +  0 < _end_) { enum { _it_ = (_start_ +  0) < _end_ ? (_start_ +  0) : _start_ }; _code_ } \
if (_start_ +  1 < _end_) { enum { _it_ = (_start_ +  1) < _end_ ? (_start_ +  1) : _start_ }; _code_ } \
if (_start_ +  2 < _end_) { enum { _it_ = (_start_ +  2) < _end_ ? (_start_ +  2) : _start_ }; _code_ } \
if (_start_ +  3 < _end_) { enum { _it_ = (_start_ +  3) < _end_ ? (_start_ +  3) : _start_ }; _code_ } \
if (_start_ +  4 < _end_) { enum { _it_ = (_start_ +  4) < _end_ ? (_start_ +  4) : _start_ }; _code_ } \
if (_start_ +  5 < _end_) { enum { _it_ = (_start_ +  5) < _end_ ? (_start_ +  5) : _start_ }; _code_ } \
if (_start_ +  6 < _end_) { enum { _it_ = (_start_ +  6) < _end_ ? (_start_ +  6) : _start_ }; _code_ } \
if (_start_ +  7 < _end_) { enum { _it_ = (_start_ +  7) < _end_ ? (_start_ +  7) : _start_ }; _code_ } \
if (_start_ +  8 < _end_) { enum { _it_ = (_start_ +  8) < _end_ ? (_start_ +  8) : _start_ }; _code_ } \
if (_start_ +  9 < _end_) { enum { _it_ = (_start_ +  9) < _end_ ? (_start_ +  9) : _start_ }; _code_ } \
if (_start_ + 10 < _end_) { enum { _it_ = (_start_ + 10) < _end_ ? (_start_ + 10) : _start_ }; _code_ } \
if (_start_ + 11 < _end_) { enum { _it_ = (_start_ + 11) < _end_ ? (_start_ + 11) : _start_ }; _code_ } \
if (_start_ + 12 < _end_) { enum { _it_ = (_start_ + 12) < _end_ ? (_start_ + 12) : _start_ }; _code_ } \
if (_start_ + 13 < _end_) { enum { _it_ = (_start_ + 13) < _end_ ? (_start_ + 13) : _start_ }; _code_ } \
if (_start_ + 14 < _end_) { enum { _it_ = (_start_ + 14) < _end_ ? (_start_ + 14) : _start_ }; _code_ } \
if (_start_ + 15 < _end_) { enum { _it_ = (_start_ + 15) < _end_ ? (_start_ + 15) : _start_ }; _code_ } \
do {} while ( false )

#define for_all_vector_entries(_it_, _code_) \
  unrolled_loop16(_it_, 0, Size, _code_)

#ifndef _VC_STATIC_ASSERT_TYPES_H
#define _VC_STATIC_ASSERT_TYPES_H
namespace Vc {
    namespace {
        template<bool> class STATIC_ASSERT_FAILURE;
        template<> class STATIC_ASSERT_FAILURE<true> {};
}}
#endif // _VC_STATIC_ASSERT_TYPES_H

#define VC_STATIC_ASSERT_NC(cond, msg) \
    typedef STATIC_ASSERT_FAILURE<cond> CAT(_STATIC_ASSERTION_FAILED_##msg, __LINE__); \
    CAT(_STATIC_ASSERTION_FAILED_##msg, __LINE__) CAT3(Error_,__LINE__,msg)
#define VC_STATIC_ASSERT(cond, msg) VC_STATIC_ASSERT_NC(cond, msg); (void) CAT3(Error_,__LINE__,msg)

#ifdef NDEBUG
#define VC_ASSERT(x)
#else
#include <assert.h>
#define VC_ASSERT(x) assert(x);
#endif

#ifdef VC_CLANG
#define VC_HAS_BUILTIN(x) __has_builtin(x)
#else
#define VC_HAS_BUILTIN(x) 0
#endif

#ifndef VC_COMMON_MACROS_H_ONCE
#define VC_COMMON_MACROS_H_ONCE
    template<int e, int center> struct exponentToMultiplier { enum {
        X = exponentToMultiplier<e - 1, center>::X * ((e - center < 31) ? 2 : 1),
        Value = (X == 0 ? 1 : X)
    }; };
    template<int center> struct exponentToMultiplier<center,center> { enum { X = 1, Value = X }; };
    template<int center> struct exponentToMultiplier<   -1, center> { enum { X = 0 }; };
    template<int center> struct exponentToMultiplier< -512, center> { enum { X = 0 }; };
    template<int center> struct exponentToMultiplier<-1024, center> { enum { X = 0 }; };

    template<int e> struct exponentToDivisor { enum {
        X = exponentToDivisor<e + 1>::X * 2,
      Value = (X == 0 ? 1 : X)
    }; };
    template<> struct exponentToDivisor<0> { enum { X = 1, Value = X }; };
    template<> struct exponentToDivisor<512> { enum { X = 0 }; };
    template<> struct exponentToDivisor<1024> { enum { X = 0 }; };
#endif // VC_COMMON_MACROS_H_ONCE
#define Vc_buildDouble(sign, mantissa, exponent) \
    ((static_cast<double>((mantissa & 0x000fffffffffffffull) | 0x0010000000000000ull) / 0x0010000000000000ull) \
    * exponentToMultiplier<exponent, 0>::Value \
    * exponentToMultiplier<exponent, 30>::Value \
    * exponentToMultiplier<exponent, 60>::Value \
    * exponentToMultiplier<exponent, 90>::Value \
    / exponentToDivisor<exponent>::Value \
    * static_cast<double>(sign))
#define Vc_buildFloat(sign, mantissa, exponent) \
    ((static_cast<float>((mantissa & 0x007fffffu) | 0x00800000) / 0x00800000) \
    * exponentToMultiplier<exponent, 0>::Value \
    * exponentToMultiplier<exponent, 30>::Value \
    * exponentToMultiplier<exponent, 60>::Value \
    * exponentToMultiplier<exponent, 90>::Value \
    / exponentToDivisor<exponent>::Value \
    * static_cast<float>(sign))

#endif // VC_COMMON_MACROS_H
