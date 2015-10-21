#ifndef BOOST_RANGE_ADAPTOR_CHUNKED_HPP_INCLUDED
#define BOOST_RANGE_ADAPTOR_CHUNKED_HPP_INCLUDED

#include <boost/range/adaptor/adjacent_filtered.hpp>
#include <boost/range/adaptor/argument_fwd.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/functional.hpp>
#include <iterator>

namespace boost
{
    namespace range_detail
    {
        template < class Iter >
        void checked_advance( Iter& it, std::size_t n, const Iter end )
        {
            // Non optimized version of std::advance that check for its bound.
            for ( ; it != end && n > 0; --n )
            {
                ++it;
            }
        }

        template < class Iter >
        class chunk_iterator;

        template < class Iter >
        struct chunk_iterator_base_gen
        {
            typedef typename iterator_range< Iter >::type value_type;

            typedef iterator_facade< chunk_iterator< Iter >,
                                     value_type,
                                     forward_traversal_tag,
                                     value_type > base_t;
        };

        // chunk_iterator for wrapping a forward traversal iterator
        template < class Iter >
        class chunk_iterator : public chunk_iterator_base_gen< Iter >::base_t
        {
            typedef typename chunk_iterator_base_gen< Iter >::base_t super_t;

            friend class ::boost::iterator_core_access;

        public:
            typedef Iter iter_t;

            chunk_iterator() : m_chunk_size( 0 ), m_it(), m_next(), m_last() {}

            chunk_iterator( std::size_t chunk_size, iter_t it, iter_t last )
            : m_chunk_size( chunk_size ),
              m_it( it ),
              m_next( it ),
              m_last( last )
            {
                increment();
            }

        private:
            void increment()
            {
                m_it = m_next;
                checked_advance( m_next, m_chunk_size, m_last );
            }

            typename super_t::reference dereference() const
            {
                return typename super_t::reference( m_it, m_next );
            }

            bool equal( const chunk_iterator& other ) const
            {
                BOOST_ASSERT( m_chunk_size == other.m_chunk_size );
                BOOST_ASSERT( !( m_it == other.m_it ) ||
                              ( m_next == other.m_next ) );
                BOOST_ASSERT( m_last == other.m_last );

                return m_it == other.m_it;
            }

            std::size_t m_chunk_size;
            iter_t m_it;
            iter_t m_next;
            iter_t m_last;
        };

        template < class Range >
        struct chunk_range
            : iterator_range<
                  chunk_iterator< typename range_iterator< Range >::type > >
        {
        public:
            // chunk_range() {}
            chunk_range( std::size_t chunk_size, Range& r )
            : iterator_range(
                  iterator( chunk_size, boost::begin( r ), boost::end( r ) ),
                  iterator( chunk_size, boost::end( r ), boost::end( r ) ) )
            {
            }
        };

        template < class ChunkSize >
        class chunk_holder : public holder< ChunkSize >
        {
        public:
            explicit chunk_holder( ChunkSize value )
            : holder< ChunkSize >( value )
            {
            }
        };

        template < class ForwardRng, class ChunkSize >
        inline chunk_range< ForwardRng > operator|(
            ForwardRng& rng,
            const chunk_holder< ChunkSize >& chunk_size )
        {
            BOOST_RANGE_CONCEPT_ASSERT( (ForwardRangeConcept< ForwardRng >));

            return chunk_range< ForwardRng >( chunk_size.val, rng );
        }

        template < class ForwardRng, class ChunkSize >
        inline chunk_range< const ForwardRng > operator|(
            const ForwardRng& rng,
            const chunk_holder< ChunkSize >& chunk_size )
        {
            BOOST_RANGE_CONCEPT_ASSERT(
                (ForwardRangeConcept< const ForwardRng >));

            return chunk_range< const ForwardRng >( chunk_size.val, rng );
        }

    } // namespace range_detail

    using range_detail::chunk_range;

    namespace adaptors
    {

        namespace
        {
            const range_detail::forwarder< range_detail::chunk_holder >
                chunked =
                    range_detail::forwarder< range_detail::chunk_holder >();
        }

        template < class ForwardRng >
        inline chunk_range< ForwardRng > chunk( ForwardRng& rng,
                                                std::size_t chunk_size )
        {
            BOOST_RANGE_CONCEPT_ASSERT( (ForwardRangeConcept< ForwardRng >));

            return chunk_range< ForwardRng >( chunk_size, rng );
        }

        template < class ForwardRng >
        inline chunk_range< const ForwardRng > chunk( const ForwardRng& rng,
                                                      std::size_t chunk_size )
        {
            BOOST_RANGE_CONCEPT_ASSERT(
                (ForwardRangeConcept< const ForwardRng >));

            return chunk_range< const ForwardRng >( chunk_size, rng );
        }

    } // namespace 'adaptors'
} // namespace 'boost'

#endif
