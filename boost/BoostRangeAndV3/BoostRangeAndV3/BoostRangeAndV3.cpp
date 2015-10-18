// BoostRangeAndV3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "grouped_by.h"
#include "boost\date_time\gregorian\gregorian.hpp"
#include "boost\function.hpp"
#include "boost\range.hpp"
#include "boost\range\adaptors.hpp"
#include "boost\range\counting_range.hpp"
#include "boost\range\algorithm\copy.hpp"
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


auto by_month() {
    return boost::adaptors::grouped_by(
        []( date a, date b ) { return a.month() == b.month(); } );
}


auto by_week() {
    // Mon-Sun
    return boost::adaptors::grouped_by(
        []( date a, date b ) { return a.week_number() == b.week_number(); } );
}

auto month_by_week()
{
    return boost::adaptors::transformed(
        [](auto month){ return month | by_week(); }
        );
}

//#define RUN_TEST 1
#ifndef RUN_TEST
int main()
{
 
    for (auto months : dates(2015, 2016) | by_month() | month_by_week())
    {
        for (auto week : months )
        {
            for (auto day : week)
            {
                std::cout << day.day() << " ";
            }
            std::cout << "\n";
        }
        std::cout << "---------------------------\n";
    }

    std::cout << std::endl;
    return 0;
}
#endif
