#ifndef IOTA_H_
#define IOTA_H_

#include "boost\range.hpp"
#include "boost\range\counting_range.hpp"
#include "boost\range\concepts.hpp"

namespace view
{
    template < typename Value >
    auto iota( Value start, Value stop )
    {
        auto range = boost::counting_range( start, stop );
        BOOST_RANGE_CONCEPT_ASSERT(
            (boost::ForwardRangeConcept< decltype( range ) >));

        return range;
    }
}

#endif
