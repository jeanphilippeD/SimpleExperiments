// AdlTagForAggregate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace unit_test
{
    template < typename T >
    void AssertEqual( const T& expect, const T& actual )
    {
        if ( expect == actual )
        {
            std::cout << "OK" << std::endl;
        }
        else
        {
            std::cout << "Fail:\nActual: " << actual << "\nExpected: " << expect
                      << std::endl;
        }
    }
}

namespace testvectorstream
{
    template < typename T >
    std::ostream& operator<<( std::ostream& os, const std::vector< T >& v )
    {
        os << "(";

        bool first = true;
        for ( auto& val : v )
        {
            if ( !first )
            {
                os << ", ";
            }
            os << val;
        }

        return os << ")";
    }
}

namespace tag_utils
{
    //
    // A tagged type that can be used to initialize an aggregate as a base class
    //
    template < typename T, class... TagT >
    struct Tagged : T
    {
        // ensure no implicit conversion.
        template < class Arg1, class Arg2, class... ArgsT >
        Tagged( Arg1&& arg1, Arg2&& arg2, ArgsT&&... args )
        : T{arg1, arg2, args...}
        {
        }
        template < class Arg1 >
        explicit Tagged( Arg1&& arg1 ) : T{arg1}
        {
        }
    };
}

namespace reflect_tuple
{
    template < class... T >
    using Tagged = tag_utils::Tagged< T... >;

    namespace streamable
    {
        struct Tag
        {
        };

        template < int idx, typename NamesT, typename ValuesT >
        std::ostream& StreamOne( std::ostream& os,
                                 NamesT& names,
                                 ValuesT& values )
        {
            return os << std::get< idx >( names ) << "="
                      << std::get< idx >( values );
        }

        template < int stopIdx >
        struct StreamUntil
        {
            template < typename NamesT, typename ValuesT >
            std::ostream& operator()( std::ostream& os,
                                      NamesT& names,
                                      ValuesT& values )
            {
                StreamUntil< stopIdx - 1 >()( os, names, values ) << ", ";
                return StreamOne< stopIdx - 1 >( os, names, values );
            }
        };

        template <>
        struct StreamUntil< 1 >
        {
            template < typename NamesT, typename ValuesT >
            std::ostream& operator()( std::ostream& os,
                                      NamesT& names,
                                      ValuesT& values )
            {
                return StreamOne< 0 >( os, names, values );
            }
        };

        template < typename T, class... TagsT >
        std::ostream& operator<<( std::ostream& os,
                                  const Tagged< T, TagsT... >& v )
        {
            auto names = v.MemberVariableNames();
            auto values = v.MemberVariableValues();

            os << "(";
            StreamUntil< std::tuple_size< decltype( names ) >::value >()(
                os, names, values );
            return os << ")";
        }
    }

    namespace equal_comparable
    {
        struct Tag
        {
        };

        template < typename T, class... TagsT >
        bool operator==( const Tagged< T, TagsT... >& v1,
                         const Tagged< T, TagsT... >& v2 )
        {
            return v1.MemberVariableValues() == v2.MemberVariableValues();
        }
    }
}

//
// A type that can be used for asserting
//
template < typename T >
using CanAssertEqual =
    reflect_tuple::Tagged< T,
                           reflect_tuple::streamable::Tag,
                           reflect_tuple::equal_comparable::Tag >;
using unit_test::AssertEqual;
using namespace std::literals::string_literals;

//
// An aggregate type that can be initialized without constructor.
//
struct TestStructData
{
    int member1;
    std::string member2;

    auto MemberVariableValues() const
    {
        return std::make_tuple( member1, member2 );
    }
    auto MemberVariableNames() const
    {
        return std::make_tuple( "member1"s, "member2"s );
    }
};
typedef CanAssertEqual< TestStructData > TestStruct;

using testvectorstream::operator<<;
struct GlobalTag
{
};

int main()
{
    TestStruct t1{1, "tt1"};
    TestStruct t2{1, "tt2"};
    std::cout << t1 << std::endl;

    std::vector< TestStruct > vt1{{1, "2"}, {3, "4"}, {5, "6"}};
    std::vector< TestStruct > vt2{{1, "12"}, {3, "4"}, {5, "6"}};
    std::cout << vt1 << std::endl;

    std::cout << ( t1 == t1 ) << std::endl;
    std::cout << ( t1 == t2 ) << std::endl;
    std::cout << ( vt1 == vt1 ) << std::endl;
    std::cout << ( vt1 == vt2 ) << std::endl;

    AssertEqual( t1, t1 );
    AssertEqual( t1, t2 ); // Fail

    AssertEqual( vt1, vt1 );
    AssertEqual( vt1, vt2 ); // Fail

    TestStructData td1 = t1;
    TestStruct t3 = TestStruct( td1 );
    // TestStruct t4 = td1; // OK should not compile.

    reflect_tuple::Tagged< TestStructData, GlobalTag > tg1( t1 );
    // std::cout << tg1 << std::endl; // OK should not compile.
    // std::cout << (tg1 == tg1) << std::endl; // OK should not compile.

    return 0;
}
