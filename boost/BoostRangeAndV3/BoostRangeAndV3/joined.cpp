#include "stdafx.h"
#include "joined.h"
#include "boost\range\algorithm\copy.hpp"
#include <vector>
#include <iostream>

namespace
{
    void RunTest()
    {
        typedef std::vector< std::vector< int > > InitialRange;
        InitialRange v = { {1, 4, 7 }, { 2, 5, 8}, {3,6,9} };
        const InitialRange cv = v;

        typedef boost::range_detail::join_iterator<
            InitialRange::const_iterator > Iter;

        Iter it1( v.begin(), v.end() );
        Iter it2( v.end(), v.end() );

        if ( it1 == it2 )
            std::cout << "equals" << std::endl;

        for ( auto it = it1; it != it2; ++it )
        {
            std::cout << *it;
        }
        std::cout << std::endl;

        typedef boost::range_detail::join_range< InitialRange > Range;

        Range rng( v );
        for ( auto val : rng )
        {

            std::cout << val;
        }
        std::cout << std::endl;

        for ( auto val : v | boost::adaptors::joined )
        {
            std::cout << val;
        }
        std::cout << std::endl;

        for ( auto sub : cv | boost::adaptors::joined )
        {
        }
        for ( auto sub : boost::adaptors::join( v) )
        {
        }
        for ( auto sub : boost::adaptors::join( cv ) )
        {
        }
    }
}

void run_joined_test()
{
    RunTest();
}
