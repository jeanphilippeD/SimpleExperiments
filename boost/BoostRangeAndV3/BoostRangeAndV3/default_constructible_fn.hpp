// Boost.Range library
//
//  Copyright Neil Groves 2014. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jean-Philippe DUFRAIGNE
// Original default_constructible_unary_fn_wrapper:
// Rename function to default_constructible_fn_wrapper and allow
// binary operator
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef BOOST_RANGE_DETAIL_DEFAULT_CONSTRUCTIBLE_FN_HPP_INCLUDED
#define BOOST_RANGE_DETAIL_DEFAULT_CONSTRUCTIBLE_FN_HPP_INCLUDED

#include <boost/optional/optional.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>

namespace boost
{
    namespace range_detail
    {

template<typename F, typename R>
class default_constructible_fn_wrapper
{
public:
    typedef R result_type;

    default_constructible_fn_wrapper()
    {
    }
    default_constructible_fn_wrapper(const F& source)
        : m_impl(source)
    {
    }
    template<typename Arg>
    R operator()(const Arg& arg) const
    {
        BOOST_ASSERT(m_impl);
        return (*m_impl)(arg);
    }
    template<typename Arg>
    R operator()(Arg& arg) const
    {
        BOOST_ASSERT(m_impl);
        return (*m_impl)(arg);
    }

    template<typename Arg1, typename Arg2>
    R operator()(const Arg1& arg1, const Arg2& arg2) const
    {
        BOOST_ASSERT(m_impl);
        return (*m_impl)(arg1, arg2);
    }
    template<typename Arg1, typename Arg2>
    R operator()(Arg1& arg1, Arg2& arg2) const
    {
        BOOST_ASSERT(m_impl);
        return (*m_impl)(arg1, arg2);
    }
private:
    boost::optional<F> m_impl;
};

template<typename F, typename R>
struct default_constructible_fn_gen
{
    typedef typename boost::mpl::if_<
        boost::has_trivial_default_constructor<F>,
        F,
        default_constructible_fn_wrapper<F,R>
    >::type type;
};


#ifndef BOOST_RANGE_DETAIL_DEFAULT_CONSTRUCTIBLE_UNARY_FN_HPP_INCLUDED
#define BOOST_RANGE_DETAIL_DEFAULT_CONSTRUCTIBLE_UNARY_FN_HPP_INCLUDED
// Hack: Patch boost locally Replace in all algoritm: need to be included first

template<typename F, typename R>
class default_constructible_unary_fn_wrapper :
    public default_constructible_fn_wrapper<F, R>
{
public:
    default_constructible_unary_fn_wrapper()
    {
    }

    default_constructible_unary_fn_wrapper(const F& source)
        : default_constructible_fn_wrapper(source)
    {
    }
};


template<typename F, typename R>
struct default_constructible_unary_fn_gen :
    default_constructible_fn_gen< F, R >
{
};
#endif
    } // namespace range_detail
} // namespace boost

#endif // include guard
