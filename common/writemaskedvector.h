/*  This file is part of the Vc library. {{{
Copyright © 2014 Matthias Kretz <kretz@kde.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the names of contributing organizations nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

}}}*/

#ifndef VC_COMMON_WRITEMASKEDVECTOR_H
#define VC_COMMON_WRITEMASKEDVECTOR_H

#include <utility>
#include "macros.h"

namespace Vc_VERSIONED_NAMESPACE
{
namespace Common
{

template <typename V, typename M = typename V::Mask> class WriteMaskedVector
{
    static_assert(
        V::Size == M::Size,
        "incorrect use of Vc::Common::WriteMaskedVector<V, M>. V and M must have the same «Size».");

public:
    typedef M Mask;
    static constexpr size_t Size = V::Size;

    FREE_STORE_OPERATORS_ALIGNED(alignof(Mask))

    // implicit (allows {vec, mask} in places where WriteMaskedVector is expected)
    Vc_INTRINSIC WriteMaskedVector(V *v, const Mask &k) : mask(k), vec(v)
    {
    }

    // prefix
    Vc_INTRINSIC V &operator++()
    {
        V one = V::One();
        one.setZeroInverted(mask);
        return *vec += one;
    }
    Vc_INTRINSIC V &operator--()
    {
        V one = V::One();
        one.setZeroInverted(mask);
        return *vec -= one;
    }

    // postfix
    Vc_INTRINSIC V operator++(int)
    {
        V ret(*vec);
        operator++();
        return ret;
    }
    Vc_INTRINSIC V operator--(int)
    {
        V ret(*vec);
        operator--();
        return ret;
    }

#define VC_OPERATOR__(op)                                                                          \
    template <typename U> Vc_ALWAYS_INLINE V &operator op##=(U &&x)                                \
    {                                                                                              \
        return operator=(static_cast<V>(*vec op std::forward<U>(x)));                              \
    }
    VC_ALL_BINARY(VC_OPERATOR__)
    VC_ALL_ARITHMETICS(VC_OPERATOR__)
    VC_ALL_SHIFTS(VC_OPERATOR__)
#undef VC_OPERATOR__

    Vc_ALWAYS_INLINE V &operator=(const V &x)
    {
        vec->assign(x, mask);
        return *vec;
    }

    template<typename T, typename IndexVector>
    Vc_ALWAYS_INLINE V &operator=(const GatherArguments<T, IndexVector> &x)
    {
        vec->gather(x, mask);
        return *vec;
    }

    template <typename T, typename = enable_if<Traits::is_subscript_operation<T>::value>>
    Vc_ALWAYS_INLINE V &operator=(T &&x)
    {
        vec->gather(std::forward<T>(x).gatherArguments(), mask);
        return *vec;
    }

    template <typename F> Vc_INTRINSIC void call(const F &f) const
    {
        return vec->call(f, mask);
    }
    template <typename F> Vc_INTRINSIC V apply(const F &f) const
    {
        return vec->apply(f, mask);
    }
    template <typename F> Vc_INTRINSIC void call(F &&f) const
    {
        return vec->call(std::forward<F>(f), mask);
    }
    template <typename F> Vc_INTRINSIC V apply(F &&f) const
    {
        return vec->apply(std::forward<F>(f), mask);
    }

private:
    Mask mask;
    V *const vec;
};
}
}

#include "undomacros.h"

#endif // VC_COMMON_WRITEMASKEDVECTOR_H
