Trying to get Eric Niebler CppCon calendar example building on boost library range and adaptors.

The goal was to try and see whether we could levergage technique provided in Eric's talk with currently available compiler and (slightly extended) library. it was developped using VS2015 using the boost range library.

Note:
- The core algorithm that were redevelopped view group_by, chunk, join, interleave, and action join were developped as if they were part of the boost range library(https://github.com/boostorg/range), they invade the existing boost range and range detail namespace. They are implemented following the example from boost range adaptors. However, the implementation are failrly sub optimal in that a few of them do some work at construction and contain extra members. The code is also not tested.

- The code in the BoostRangeAndV3.cpp is mostly taken from Eric's CppCon talk and from the companion source code https://github.com/ericniebler/range-v3/blob/master/example/calendar.cpp with the exception of the lambda expressions which are replaced by equivalent functors here.
