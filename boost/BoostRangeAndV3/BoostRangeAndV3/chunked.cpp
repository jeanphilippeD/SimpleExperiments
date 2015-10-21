#include "stdafx.h"
#include "chunked.h"
#include "boost\range\algorithm\copy.hpp"
#include <vector>
#include <iostream>

namespace
{
    void RunTest()
    {
        std::vector< int > v = {1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 7};
        const std::vector< int > cv = v;

        typedef boost::range_detail::chunk_iterator<
            std::vector< int >::const_iterator > Iter;

        Iter it1( 2, v.begin(), v.end() );
        Iter it2( 2, v.end(), v.end() );

        if ( it1 == it2 )
            std::cout << "equals" << std::endl;

        for ( auto it = it1; it != it2; ++it )
        {
            for ( auto val : *it )
            {
                std::cout << val;
            }
            std::cout << std::endl;
        }

        typedef boost::range_detail::chunk_range< std::vector< int > > Range;

        Range rng( 3, v );
        for ( auto sub : rng )
        {
            for ( auto val : sub )
            {
                std::cout << val;
            }
            std::cout << std::endl;
        }

        for ( auto sub : v | boost::adaptors::chunked( 5 ) )
        {
            for ( auto val : sub )
            {
                std::cout << val;
            }
            std::cout << std::endl;
        }

        for ( auto sub : cv | boost::adaptors::chunked( 5 ) )
        {
        }
        for ( auto sub : boost::adaptors::chunk( v, 5 ) )
        {
        }
        for ( auto sub : boost::adaptors::chunk( cv, 5 ) )
        {
        }
    }
}

void run_chunked_test()
{
    RunTest();
}
