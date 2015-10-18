// BoostRangeAndV3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "grouped_by.h"
#include "action\joined.h"
#include "boost\date_time\gregorian\gregorian.hpp"
#include "boost\format.hpp"
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

namespace
{
    // return by value, not the most efficient but safer.
    template < typename RangeT >
    typename RangeT::value_type front( const RangeT& range )
    {
        assert( boost::begin( range ) != boost::end( range ) );
        return *boost::begin( range );
    }
}

namespace action
{
    using namespace boost::adaptors::action;
}
namespace view
{
    using namespace boost::adaptors;


    template < class Range1 >
    Range1 concat(const Range1& r1)
    {
        return r1;
    }

    template < class Range1, class Range2, class... RangeN >
    auto concat( Range1& r1 , Range2& r2, RangeN&... rn )
    {
        return boost::range::join(r1, concat( r2, rn...));
    }
}

auto dates( unsigned short start, unsigned short stop )
{
    return boost::counting_range( date{start, greg::Jan, 1}, // view::iota
                                  date{stop, greg::Jan, 1} );
}


auto by_month() {
    return view::grouped_by(
        []( date a, date b ) { return a.month() == b.month(); } );
}


auto by_week() {
    // Mon-Sun
    return view::grouped_by(
        []( date a, date b ) { return a.week_number() == b.week_number(); } );
}

auto month_by_week()
{
    return view::transformed(
        [](auto month){ return month | by_week(); }
        );
}

std::string format_day(date d) {
    return boost::str(boost::format("%|3|") % d.day());
}

int position_day_in_week(date d)
{
    typedef greg::gregorian_calendar::day_of_week_type day_t;

    day_t monday = boost::date_time::Monday;
    day_t current = d.day_of_week();
    
    // adjust the day so monday is in position 0.
    // days are in [0,7) using modulo on positive numbers
    return ( (int)current - (int)monday + 7 ) % 7;
}

// In:  Range<Range<date>>: month grouped by weeks.
// Out: Range<std::string>: month with formatted weeks.
auto format_weeks()
{
    return view::transformed( []( /*Range<date>*/ auto week ) {
        return boost::str(
            boost::format( "%1%%2%%|22t|" ) %
            std::string(position_day_in_week( front( week ) ) * 3, ' ' ) %
            ( week | view::transformed( format_day ) | action::joined ) );
    } );
}

// Return a formatted string with the title of the month
// corresponding to a date.
std::string month_title( date d )
{
    return boost::str( boost::format( "%|=22|" ) % d.month().as_long_string() );
}

// In:  Range<Range<date>>: year of months of days
// Out: Range<Range<std::string>>: year of months of formatted wks
auto layout_months()
{
    return view::transformed( []( /*Range<date>*/ auto month ) {
        return month | by_week() | format_weeks();
    } );
}

//auto layout_months()
//{
//    return view::transform( []( /*Range<date>*/ auto month ) {
//        int week_count = distance( month | by_week() );
//        return view::concat(
//            view::single( month_title( front( month ) ) ),
//            month | by_week() | format_weeks(),
//            view::repeat_n( std::string( 22, ' ' ), 6 - week_count ) );
//    } );
//}

//#define RUN_TEST 1
#ifndef RUN_TEST
int main()
{
    std::vector< std::string > str1;
    auto str2 = (str1 | action::joined);

    auto year = dates(2015, 2016) | by_month() | layout_months();
    for (auto month : year )
    {
        for (auto week : month)
        {
            std::cout << week << "\n";
        }
        std::cout << "---------------------------\n";
    }

    //for (auto month : year | by_month())
    //{
    //    auto cc = view::concat(
    //        view::single(month_title(front(month))),
    //        month | by_week() | format_weeks()
    //        );
    //}
    //for ( auto month : year |  )
    //{

    //    //auto weektr = month | format_weeks();
    //    //auto rng = (month | view::transformed([]( /*Range<date>*/ auto week) {
    //    //    return *boost::begin( week ); }));
    //    //date a = *boost::begin(rng);

    //    for ( auto week : month )
    //    {
    //        //auto weekStr2 = boost::str(
    //        //    boost::format("%1%%2%%|22t|") %
    //        //    std::string((int)front(week).day_of_week() * 3, ' ') %
    //        //    (week | view::transformed(format_day) | action::joined));

    //        //auto weekStr = (week | view::transformed(format_day) | action::joined);
    //        //auto weektr = week | format_weeks();
    //        std::cout << weekStr << "\n";
    //    }
    //    std::cout << "---------------------------\n";
    //}

    std::cout << std::endl;
    return 0;
}
#endif
