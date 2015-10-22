// BoostRangeAndV3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "chunked.h"
#include "action\joined.h"
#include "date_by_day.h"
#include "grouped_by.h"
#include "interleaved.h"
#include "iota.h"
#include "boost\date_time\gregorian\gregorian.hpp"
#include "boost\format.hpp"
#include "boost\range.hpp"
#include "boost\range\adaptors.hpp"
#include "boost\range\algorithm\copy.hpp"
#include "boost\range\join.hpp"
#include <vector>
#include <iostream>

#define RUN_TEST 1
void run_grouped_by_test();
void run_date_by_day_test();
void run_iota_test();
void run_chunked_test();
void run_interleaved_test();
void run_joined_test();

namespace greg = boost::gregorian;
using date = greg::date;
using day = greg::date_duration;
using namespace date_ext;

namespace
{
    void run_tests()
    {
        //run_grouped_by_test();
        //run_date_by_day_test();
        //run_iota_test();
        //run_chunked_test();
        //run_interleaved_test();
        run_joined_test();
    }

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
namespace details
{
    template < typename Value >
    struct Repeat
    {
    public:
        explicit Repeat( const Value& v ) : m_v( v ) {}
        const Value& operator()( int ) const { return m_v; }

    private:
        Value m_v;
    };
}
namespace view
{
    using namespace boost::adaptors;

    template < class Range1 >
    Range1 concat( const Range1& r1 )
    {
        return r1;
    }

    template < class Range1, class Range2, class... RangeN >
    auto concat( Range1& r1, Range2& r2, RangeN&... rn )
    {
        return boost::range::join( r1, concat( r2, rn... ) );
    }

    template < typename Value >
    auto repeat_n( const Value& v, std::ptrdiff_t n )
    {
        return boost::counting_range( std::ptrdiff_t(), n ) |
               transformed( details::Repeat< Value >( v ) );
    }

    template < typename Value >
    auto single( const Value& v )
    {
        return repeat_n( v, 1 );
    }
}

auto dates( unsigned short start, unsigned short stop )
{
    return view::iota( date_by_day{start, greg::Jan, 1},
                       date_by_day{stop, greg::Jan, 1} );
}

struct by_month_op
{
    bool operator()( date a, date b ) const { return a.month() == b.month(); }
};
auto by_month()
{
    return view::grouped_by( by_month_op() );
}

struct by_week_op
{
    bool operator()( date a, date b ) const
    {
        // Mon-Sun
        return a.week_number() == b.week_number();
    }
};
auto by_week()
{
    return view::grouped_by( by_week_op() );
}

struct month_by_week_op
{
    template < typename Month >
    auto operator()( Month month ) const
    {
        return month | by_week();
    }
};
auto month_by_week()
{
    return view::transformed( month_by_week_op() );
}

int position_day_in_week( date d )
{
    typedef greg::gregorian_calendar::day_of_week_type day_t;

    day_t monday = boost::date_time::Monday;
    day_t current = d.day_of_week();

    // adjust the day so monday is in position 0.
    // days are in [0,7) using modulo on positive numbers
    return ( (int)current - (int)monday + 7 ) % 7;
}

std::string format_day( date d )
{
    return boost::str( boost::format( "%|3|" ) % d.day() );
}

struct format_weeks_op
{
    template < typename Week >
    auto operator()( Week week ) const
    {
        return boost::str(
            boost::format( "%1%%2%%|22t|" ) %
            std::string( position_day_in_week( front( week ) ) * 3, ' ' ) %
            ( week | view::transformed( format_day ) | action::joined ) );
    }
};
// In:  Range<Range<date>>: month grouped by weeks.
// Out: Range<std::string>: month with formatted weeks.
auto format_weeks()
{
    return view::transformed( format_weeks_op() );
}

// Return a formatted string with the title of the month
// corresponding to a date.
std::string month_title( date d )
{
    return boost::str( boost::format( "%|=22|" ) % d.month().as_long_string() );
}

struct layout_months_op
{
    template < typename Month >
    auto operator()( Month month ) const
    {
        int week_count = boost::distance( month | by_week() );
        return view::concat(
            view::single( month_title( front( month ) ) ),
            month | by_week() | format_weeks(),
            view::repeat_n( std::string( 22, ' ' ), 6 - week_count ) );
    }
};
// In:  Range<Range<date>>: year of months of days
// Out: Range<Range<std::string>>: year of months of formatted wks
auto layout_months()
{
    return view::transformed( layout_months_op() );
}

struct transpose_op
{
    // In:  Range<Range<T>>
    // Out: Range<Range<T>>, transposing the rows and columns.
    template < typename ForwardRng >
    auto operator()( ForwardRng&& rngs ) const
    {
        BOOST_RANGE_CONCEPT_ASSERT( (boost::ForwardRangeConcept< ForwardRng >));
        BOOST_RANGE_CONCEPT_ASSERT(
            (boost::ForwardRangeConcept< typename ForwardRng::value_type >));

        return std::forward< ForwardRng >( rngs ) | interleave() |
               chunk( distance( rngs ) );
    }
};
const transpose_op transpose = transpose_op();

template < class ForwardRng >
inline auto operator|( ForwardRng& rng, const transpose_op& transpose_object )
{
    return transpose_object( rng );
}


struct transpose_months_op
{
    template < typename ForwardRng /*Range<Range<string>>*/ >
    auto operator()(ForwardRng rng) const
    {
        return rng | transpose();
    }
};
// In:  Range<Range<Range<string>>>
// Out: Range<Range<Range<string>>>, transposing months.
auto transpose_months() {
    return view::transformed( transpose_months_op() );
}

struct join_months_op
{
    template < typename ForwardRng /*Range<string>*/ >
    auto operator()(ForwardRng rng) const
    {
        return action::join(rng);
    }
};
// In:  Range<Range<string>>
// Out: Range<string>, joining the strings of the inner ranges
auto join_months() {
    return view::transformed( join_months_op() );
}

void print_calendar()
{
    auto year = dates( 2015, 2016 ) | by_month() | layout_months();
    for ( auto month : year )
    {
        for ( auto& week : month )
        {
            std::cout << week << "\n";
        }
        std::cout << "---------------------------\n";
    }

    std::cout << std::endl;
}

int main()
{
#ifdef RUN_TEST
    run_tests();
#else
    print_calendar();
#endif
    return 0;
}
