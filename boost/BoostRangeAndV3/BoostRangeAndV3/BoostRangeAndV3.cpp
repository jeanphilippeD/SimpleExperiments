// BoostRangeAndV3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "boost\range.hpp"
#include "boost\range\counting_range.hpp"
#include "boost\range\adaptors.hpp"
#include "boost\date_time\gregorian\gregorian.hpp"
#include <vector>
#include <iostream>

namespace greg = boost::gregorian;
using date = greg::date;
using day = greg::date_duration;

namespace boost
{
    namespace gregorian
    {
        date& operator++( date& d ) { return d = d + day( 1 ); }
        date operator++( date& d, int ) { return ++d - day( 1 ); }
        date& operator--( date& d ) { return d = d - day( 1 ); }
        date operator--( date& d, int ) { return --d + day( 1 ); }
    }

    namespace iterators
    {
        template <>
        struct iterator_traversal< date >
        {
            typedef boost::iterators::bidirectional_traversal_tag type;
        };

        template <>
        struct iterator_difference< date >
        {
            typedef day type;
        };
    }
}

auto dates( unsigned short start, unsigned short stop )
{
    return boost::counting_range( date{start, greg::Jan, 1}, // view::iota
                                  date{stop, greg::Jan, 1} );
}

int main()
{
    for ( auto val : dates( 2015, 2016 ) )
    {
        std::cout << val << std::endl;
    }

    return 0;
}
