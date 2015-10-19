#include "stdafx.h"
#include "iota.h"
#include "date_by_day.h"
#include <iostream>

namespace
{
    void RunTest()
    {
        {
            auto range = view::iota( 3, 10 );

            for ( auto val : range )
            {
                std::cout << val << ", ";
            }
            std::cout << std::endl;
        }

        {
            auto range = view::iota(
                date_ext::date_by_day{2015, boost::gregorian::Jan, 1},
                date_ext::date_by_day{2016, boost::gregorian::Jan, 1} );

            for ( auto val : range )
            {
                std::cout << val << ", ";
            }
            std::cout << std::endl;
        }
    }
}

void run_iota_test()
{
    RunTest();
}