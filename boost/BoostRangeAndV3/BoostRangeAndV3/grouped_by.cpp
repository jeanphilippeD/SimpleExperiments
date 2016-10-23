#include "stdafx.h"
#include "grouped_by.h"
#include "boost\range\algorithm\copy.hpp"
#include <vector>
#include <iostream>

namespace
{
    template< typename Pred >
    void RunTest( Pred pred )
    {
        std::vector< int > v = {1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 7};
        const std::vector< int > cv = v;

        typedef boost::range_detail::view::
            group_by_iterator< Pred, std::vector< int >::iterator >
                Iter;

        Iter it1( pred, v.begin(), v.end() );
        Iter it2( pred, v.end(), v.end() );

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

        typedef boost::view::group_by_range< Pred, std::vector< int > > Range;

        Range rng( pred, v );
        for ( auto sub : rng )
        {
            for ( auto val : sub )
            {
                std::cout << val;
            }
            std::cout << std::endl;
        }

        for ( auto sub : v | boost::adaptors::view::grouped_by( pred ) )
        {
            for ( auto val : sub )
            {
                std::cout << val;
            }
            std::cout << std::endl;
        }

        for ( auto sub : cv | boost::adaptors::view::grouped_by( pred ) )
        {
        }
        for ( auto sub : boost::adaptors::view::group_by( v, pred ) )
        {
        }
        for ( auto sub : boost::adaptors::view::group_by( cv, pred ) )
        {
        }

        using namespace boost::adaptors;

        std::vector< int > input;

        std::cout << "Not eq " << std::endl;
        boost::copy( v | adjacent_filtered( std::not_equal_to< int >() ),
                     std::ostream_iterator< int >( std::cout, "," ) );

        std::cout << "eq " << std::endl;
        boost::copy( v | adjacent_filtered( pred ),
                     std::ostream_iterator< int >( std::cout, "," ) );
    }
}

void run_grouped_by_test()
{
    RunTest( std::equal_to< int >() );

    //std::unique_ptr< 
    RunTest([](int v1, int v2) { return v1 == v2; });
}
