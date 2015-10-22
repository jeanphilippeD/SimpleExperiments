#ifndef BOOST_RANGE_ADAPTOR_JOINED_HPP_INCLUDED
#define BOOST_RANGE_ADAPTOR_JOINED_HPP_INCLUDED

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
        namespace view
        {
            template < class Iter >
            class join_iterator;

            template < class Iter >
            struct join_iterator_base_gen
            {
                typedef typename Iter::value_type sub_range_t;
                typedef decltype( boost::begin( sub_range_t() ) ) sub_iter_t;

                typedef typename sub_iter_t::value_type value_type;

                typedef iterator_facade< join_iterator< Iter >,
                                         value_type,
                                         forward_traversal_tag,
                                         value_type > base_t;
            };

            // join_iterator for wrapping a forward traversal iterator
            template < class Iter >
            class join_iterator : public join_iterator_base_gen< Iter >::base_t
            {
                typedef typename join_iterator_base_gen< Iter >::base_t super_t;
                typedef typename join_iterator_base_gen< Iter >::sub_iter_t
                    sub_iter_t;

                friend class ::boost::iterator_core_access;

            public:
                typedef Iter iter_t;

                join_iterator()
                : m_it(),
                  m_last(),
                  m_is_it_valid( false ),
                  m_sub_it(),
                  m_sub_last()
                {
                }

                join_iterator( iter_t it, iter_t last )
                : m_it( it ),
                  m_last( last ),
                  m_it_valid( m_it != m_last ),
                  m_sub_it( m_it_valid ? boost::begin( *m_it ) : sub_iter_t() ),
                  m_sub_last( m_it_valid ? boost::end( *m_it ) : sub_iter_t() )
                {
                    ensure_valid_sub_it_or_last();
                }

            private:
                void increment()
                {
                    BOOST_ASSERT( m_it_valid );

                    ++m_sub_it;
                    ensure_valid_sub_it_or_last();
                }

                typename super_t::reference dereference() const
                {
                    BOOST_ASSERT( m_it_valid );

                    return *m_sub_it;
                }

                bool equal( const join_iterator& other ) const
                {
                    BOOST_ASSERT( m_it_valid != other.m_it_valid ||
                                  !m_it_valid || m_it == other.m_it ||
                                  m_sub_last == other.m_sub_last );

                    return m_it_valid == other.m_it_valid &&
                           ( !m_it_valid || ( m_it == other.m_it &&
                                              m_sub_it == other.m_sub_it ) );
                }

                void ensure_valid_sub_it_or_last()
                {
                    while ( m_it_valid && m_sub_it == m_sub_last )
                    {
                        m_it_valid = ( ++m_it != m_last );
                        m_sub_it = ( m_it_valid ? boost::begin( *m_it ) :
                                                  sub_iter_t() );
                        m_sub_last =
                            ( m_it_valid ? boost::end( *m_it ) : sub_iter_t() );
                    }
                }

                iter_t m_it;
                iter_t m_next;
                iter_t m_last;
                bool m_it_valid;
                sub_iter_t m_sub_it;
                sub_iter_t m_sub_last;
            };

            template < class Range >
            struct join_range
                : iterator_range<
                      join_iterator< typename range_iterator< Range >::type > >
            {
            public:
                // join_range() {}
                join_range( Range& r )
                : iterator_range(
                      iterator( boost::begin( r ), boost::end( r ) ),
                      iterator( boost::end( r ), boost::end( r ) ) )
                {
                }
            };

            class join_forwarder
            {
            };

            template < class ForwardRng >
            inline join_range< ForwardRng > operator|(
                ForwardRng& rng,
                const join_forwarder& /*tag*/ )
            {
                BOOST_RANGE_CONCEPT_ASSERT(
                    (ForwardRangeConcept< ForwardRng >));

                return join_range< ForwardRng >( rng );
            }

            template < class ForwardRng >
            inline join_range< const ForwardRng > operator|(
                const ForwardRng& rng,
                const join_forwarder& /*tag*/ )
            {
                BOOST_RANGE_CONCEPT_ASSERT(
                    (ForwardRangeConcept< const ForwardRng >));

                return join_range< const ForwardRng >( rng );
            }
        } // namespace view
    } // namespace range_detail

    namespace view
    {
        using range_detail::view::join_range;
    }
    namespace adaptors
    {
        namespace view
        {
            using boost::range_detail::view::join_range;

            namespace
            {
                const range_detail::view::join_forwarder joined =
                    range_detail::view::join_forwarder();
            }

            template < class ForwardRng >
            inline join_range< ForwardRng > join( ForwardRng& rng )
            {
                BOOST_RANGE_CONCEPT_ASSERT(
                    (ForwardRangeConcept< ForwardRng >));

                return join_range< ForwardRng >( rng );
            }

            template < class ForwardRng >
            inline join_range< const ForwardRng > join( const ForwardRng& rng )
            {
                BOOST_RANGE_CONCEPT_ASSERT(
                    (ForwardRangeConcept< const ForwardRng >));

                return join_range< const ForwardRng >( rng );
            }
        } // namespace view
    } // namespace 'adaptors'
} // namespace 'boost'

#endif
