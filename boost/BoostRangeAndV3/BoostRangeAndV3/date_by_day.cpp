#include "stdafx.h"
#include "date_by_day.h"
#include "boost\range\counting_range.hpp"

namespace greg = boost::gregorian;
using date = greg::date;
using day = greg::date_duration;
using namespace date_ext;

namespace
{
    template < typename ForwardRng >
    void test( ForwardRng rng )
    {
        BOOST_RANGE_CONCEPT_ASSERT(
            (boost::SinglePassRangeConcept< ForwardRng >));

        BOOST_RANGE_CONCEPT_ASSERT( (boost::ForwardRangeConcept< ForwardRng >));

        BOOST_RANGE_CONCEPT_ASSERT(
            (boost::BidirectionalRangeConcept< ForwardRng >));

        BOOST_RANGE_CONCEPT_ASSERT(
            (boost::RandomAccessRangeConcept< ForwardRng >));
    }

    void RunTest()
    {
        {
            auto range = boost::counting_range( 1, 10 );

            // Random access result in a warning in VS2015 because
            // operator-(int,int) -> __int64 and using it to increment the
            // integer result in possible loss of precision.
            BOOST_RANGE_CONCEPT_ASSERT(
                (boost::BidirectionalRangeConcept< decltype( range ) >));
            // test( range )

            for ( auto val : range )
            {
                std::cout << val << ", ";
            }
            std::cout << std::endl;
        }

        {
            auto range =
                boost::counting_range( date_by_day{2015, greg::Jan, 1},
                                       date_by_day{2016, greg::Jan, 1} );

            test( range );
            for ( auto val : range )
            {
                std::cout << val << ", ";
            }
            auto dist = boost::distance( range );

            std::cout << std::endl;
        }
    }
}

void run_date_by_day_test()
{
    RunTest();
}