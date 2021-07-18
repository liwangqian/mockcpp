/***
   mockcpp is a C/C++ mock framework.
   Copyright [2021] [Darwin Yuan <darwin.yuan@gmail.com>]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
***/

#ifndef __MOCKCPP_INVOKABLE_TRAITS_H
#define __MOCKCPP_INVOKABLE_TRAITS_H

#if __cplusplus >= 201103L

#include <mockcpp/mockcpp.h>
#include <type_traits>

MOCKCPP_NS_START
namespace detail {

template <typename F>
struct InvokableTraitsHelper;

template <typename R, typename ...Args>
struct InvokableTraitsHelper<R(Args...)>
{
    using InvokableType = R(*)(Args...);
};

template <typename R, typename ...Args>
struct InvokableTraitsHelper<R(*)(Args...)> : InvokableTraitsHelper<R(Args...)> {};

template <typename C, typename R, typename ...Args>
struct InvokableTraitsHelper<R(C::*)(Args...)> : InvokableTraitsHelper<R(Args...)> {};

template <typename C, typename R, typename ...Args>
struct InvokableTraitsHelper<R(C::*)(Args...) const> : InvokableTraitsHelper<R(Args...)> {};

template <typename F>
struct IsFunctor
{
    template <class U>
    static auto _(U*) -> decltype(&U::operator(), std::true_type());

    template <class U>
    static auto _(...) -> std::false_type;

    using type = decltype(_<F>(nullptr));
    static constexpr bool value = type::value;
};

template <typename T, bool Functor = IsFunctor<T>::value>
struct SelectInvokableTrait
{
    using Trait = void;
};

template <typename T>
struct SelectInvokableTrait<T, true>
{
    /* deduce for lambda */
    using Trait = InvokableTraitsHelper<decltype(&T::operator())>;
};

} // namespace detail

template <typename F>
struct InvokableTrait
{
    using Trait_ = typename detail::SelectInvokableTrait<F>::Trait;
    using InvokableType = typename Trait_::InvokableType;
};

MOCKCPP_NS_END

#endif // #if __cplusplus >= 201103L
#endif
