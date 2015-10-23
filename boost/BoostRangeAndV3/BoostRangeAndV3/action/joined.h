#ifndef BOOST_RANGE_ADAPTOR_ACTION_JOINED_HPP_INCLUDED
#define BOOST_RANGE_ADAPTOR_ACTION_JOINED_HPP_INCLUDED

#include <boost/range/adaptor/argument_fwd.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <iterator>

namespace boost
{
    namespace range_detail
    {
        namespace action
        {
            //
            // This is a quick implementation.
            // It is not very generic and would need to be improved.
            //
            template < typename Range >
            struct join_with_push_back
            {
                typedef typename Range::value_type value_type;

                value_type operator()( Range rng )
                {
                    value_type result;
                    for ( auto&& val : rng )
                    {
                        boost::range::push_back( result, val );
                    }
                    return result;
                }
            };

            struct join_forwarder
            {
            };

            template < class ForwardRng >
            inline typename join_with_push_back< ForwardRng >::value_type
            operator|( ForwardRng& rng, const join_forwarder& /* tag */ )
            {
                BOOST_RANGE_CONCEPT_ASSERT(
                    (ForwardRangeConcept< ForwardRng >));

                return join_with_push_back< ForwardRng >()( rng );
            }

            template < class ForwardRng >
            inline typename join_with_push_back< ForwardRng >::value_type
            operator|( const ForwardRng& rng, const join_forwarder& /*tag*/ )
            {
                BOOST_RANGE_CONCEPT_ASSERT(
                    (ForwardRangeConcept< const ForwardRng >));

                return join_with_push_back< const ForwardRng >()( rng );
            }
        } // namespace action
    }     // namespace range_detail

    namespace adaptors
    {
        namespace action
        {
            namespace
            {
                const range_detail::action::join_forwarder joined =
                    range_detail::action::join_forwarder();
            }

            template < class ForwardRng >
            inline typename range_detail::action::join_with_push_back<
                ForwardRng >::value_type
            join( ForwardRng& rng )
            {
                BOOST_RANGE_CONCEPT_ASSERT(
                    (ForwardRangeConcept< ForwardRng >));

                return range_detail::action::join_with_push_back<
                    ForwardRng >()( rng );
            }

            template < class ForwardRng >
            inline typename range_detail::action::join_with_push_back<
                ForwardRng >::value_type
            join( const ForwardRng& rng )
            {
                BOOST_RANGE_CONCEPT_ASSERT(
                    (ForwardRangeConcept< const ForwardRng >));

                return range_detail::action::join_with_push_back<
                    const ForwardRng >()( rng );
            }
        }
    }
}
#endif
