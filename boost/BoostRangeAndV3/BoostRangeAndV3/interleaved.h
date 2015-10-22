#ifndef BOOST_RANGE_ADAPTOR_INTERLEAVED_HPP_INCLUDED
#define BOOST_RANGE_ADAPTOR_INTERLEAVED_HPP_INCLUDED

#include <boost/range/adaptor/argument_fwd.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/functional.hpp>
#include <iterator>
#include <vector>

namespace boost
{
    namespace range_detail
    {
        template < class Iter >
        class interleave_iterator;

        template < class Iter >
        struct interleave_iterator_base_gen
        {
            typedef typename Iter::value_type sub_range_t;
            typedef decltype( boost::begin( sub_range_t() ) ) sub_iter_t;

            typedef typename sub_iter_t::value_type value_type;

            typedef iterator_facade< interleave_iterator< Iter >,
                                     value_type,
                                     forward_traversal_tag,
                                     value_type > base_t;
        };

        // interleave_iterator for wrapping a forward traversal iterator
        template < class Iter >
        class interleave_iterator
            : public interleave_iterator_base_gen< Iter >::base_t
        {
            typedef
                typename interleave_iterator_base_gen< Iter >::base_t super_t;
            typedef typename interleave_iterator_base_gen< Iter >::sub_iter_t
                sub_iter_t;

            friend class ::boost::iterator_core_access;

        public:
            typedef Iter iter_t;

            interleave_iterator()
            : m_interleave_size( 0 ), m_it(), m_next(), m_last()
            {
            }

            interleave_iterator( iter_t it, iter_t last ) : m_current( 0 )
            {
                for ( const auto& sub_range : make_iterator_range( it, last ) )
                {
                    m_its.push_back( boost::begin( sub_range ) );
                    m_lasts.push_back( boost::end( sub_range ) );
                }
            }

        private:
            void increment()
            {
                if ( 0 == ( ( ++m_current ) %= m_its.size() ) )
                {
                    for ( auto& it : m_its )
                    {
                        ++it;
                    }

                    BOOST_ASSERT( is_end_safe() == ( m_its == m_lasts ) );
                    if ( m_its[ 0 ] == m_lasts[ 0 ] )
                    {
                        m_its.clear();
                        m_lasts.clear();
                    }
                }
            }

            typename super_t::reference dereference() const
            {
                return *m_its[ m_current ];
            }

            bool equal( const interleave_iterator& other ) const
            {
                BOOST_ASSERT( m_lasts == other.m_lasts || m_lasts.empty() ||
                              other.m_lasts.empty() );

                return m_its.empty() == other.m_its.empty() &&
                       ( m_its.empty() ||
                         m_its[ m_current ] == other.m_its[ m_current ] );
            }

            const bool is_end_safe() const
            {
                return std::make_pair( m_its.end(), m_lasts.end() ) !=
                       std::mismatch( m_its.begin(),
                                      m_its.end(),
                                      m_lasts.begin(),
                                      m_lasts.end(),
                                      std::not_equal_to< sub_iter_t >() );
            }

            std::vector< sub_iter_t > m_its;
            std::vector< sub_iter_t > m_lasts;
            std::size_t m_current;
        };

        template < class Range >
        struct interleave_range : iterator_range< interleave_iterator<
                                      typename range_iterator< Range >::type > >
        {
        public:
            // interleave_range() {}
            interleave_range( Range& r )
            : iterator_range( iterator( boost::begin( r ), boost::end( r ) ),
                              iterator( boost::end( r ), boost::end( r ) ) )
            {
            }
        };

        class interleave_forwarder
        {
        };

        template < class ForwardRng >
        inline interleave_range< ForwardRng > operator|(
            ForwardRng& rng,
            const interleave_forwarder& /*tag*/ )
        {
            BOOST_RANGE_CONCEPT_ASSERT( (ForwardRangeConcept< ForwardRng >));

            return interleave_range< ForwardRng >( rng );
        }

        template < class ForwardRng >
        inline interleave_range< const ForwardRng > operator|(
            const ForwardRng& rng,
            const interleave_forwarder& /*tag*/ )
        {
            BOOST_RANGE_CONCEPT_ASSERT(
                (ForwardRangeConcept< const ForwardRng >));

            return interleave_range< const ForwardRng >( rng );
        }

    } // namespace range_detail

    using range_detail::interleave_range;

    namespace adaptors
    {

        namespace
        {
            const range_detail::interleave_forwarder interleaved =
                range_detail::interleave_forwarder();
        }

        template < class ForwardRng >
        inline interleave_range< ForwardRng > interleave( ForwardRng& rng )
        {
            BOOST_RANGE_CONCEPT_ASSERT( (ForwardRangeConcept< ForwardRng >));

            return interleave_range< ForwardRng >( rng );
        }

        template < class ForwardRng >
        inline interleave_range< const ForwardRng > interleave(
            const ForwardRng& rng )
        {
            BOOST_RANGE_CONCEPT_ASSERT(
                (ForwardRangeConcept< const ForwardRng >));

            return interleave_range< const ForwardRng >( rng );
        }

    } // namespace 'adaptors'
} // namespace 'boost'

#endif
