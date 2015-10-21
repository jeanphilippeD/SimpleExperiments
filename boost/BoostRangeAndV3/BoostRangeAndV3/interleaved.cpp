#include "stdafx.h"
#include "interleaved.h"
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

        typedef boost::range_detail::interleave_iterator<
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

        typedef boost::range_detail::interleave_range< InitialRange > Range;

        Range rng( v );
        for ( auto val : rng )
        {

            std::cout << val;
        }
        std::cout << std::endl;

        for ( auto val : v | boost::adaptors::interleaved )
        {
            std::cout << val;
        }
        std::cout << std::endl;

        for ( auto sub : cv | boost::adaptors::interleaved )
        {
        }
        for ( auto sub : boost::adaptors::interleave( v) )
        {
        }
        for ( auto sub : boost::adaptors::interleave( cv ) )
        {
        }
    }
}

void run_interleaved_test()
{
    RunTest();
}
