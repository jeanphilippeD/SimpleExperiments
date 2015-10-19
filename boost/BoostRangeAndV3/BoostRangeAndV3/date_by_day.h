#ifndef DATE_BY_DAY_H_
#define DATE_BY_DAY_H_

#include "boost\date_time\gregorian\gregorian.hpp"
#include "boost\iterator\iterator_categories.hpp"

namespace date_ext
{
    class date_by_day : public boost::gregorian::date
    {
        typedef boost::gregorian::date super_t;

    public:
        typedef boost::gregorian::date_duration day;
        typedef day::duration_rep_type integral_duration_day;

    public:
        using super_t::super_t;
        // explicit date_by_day(date d) : date(d) {}

        date_by_day& operator+=( day dd )
        {
            super_t::operator+=( dd );
            return *this;
        }
        date_by_day& operator+=( integral_duration_day dd )
        {
            return operator+=( day( dd ) );
        }

        date_by_day& operator-=( day dd )
        {
            super_t::operator-=( dd );
            return *this;
        }
        date_by_day& operator-=( integral_duration_day dd )
        {
            return operator-=( day( dd ) );
        }

        date_by_day operator+( day dd ) { return date_by_day( *this ) += dd; }
        date_by_day operator+( integral_duration_day dd )
        {
            return operator+( day( dd ) );
        }

        date_by_day operator-( day dd ) { return date_by_day( *this ) -= dd; }
        date_by_day operator-( integral_duration_day dd )
        {
            return operator-( day( dd ) );
        }

        date_by_day& operator++() { return operator+=( 1 ); }
        date_by_day operator++( int ) { return operator++() - 1; }
        date_by_day& operator--() { return operator-=( 1 ); }
        date_by_day operator--( int ) { return operator--() + 1; }
    };

    inline date_ext::date_by_day::integral_duration_day operator-(
        const date_by_day& d1,
        const date_by_day& d2 )
    {
        const boost::gregorian::date& s1 = d1;
        const boost::gregorian::date& s2 = d2;

        date_by_day::day difference = s1 - s2;
        return difference.days();
    }
}

namespace boost
{
    namespace iterators
    {
        template <>
        struct iterator_traversal< date_ext::date_by_day >
        {
            typedef boost::iterators::random_access_traversal_tag type;
        };

        template <>
        struct iterator_difference< date_ext::date_by_day >
        {
            typedef date_ext::date_by_day::integral_duration_day type;
        };
    }
}

#endif
