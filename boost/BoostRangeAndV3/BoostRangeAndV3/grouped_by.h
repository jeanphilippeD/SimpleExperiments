#ifndef BOOST_RANGE_ADAPTOR_GROUPED_BY_HPP_INCLUDED
#define BOOST_RANGE_ADAPTOR_GROUPED_BY_HPP_INCLUDED

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
        template < class Pred, class Iter >
        class group_by_iterator;

        template < class Pred, class Iter >
        struct group_by_iterator_base_gen
        {
            typedef typename iterator_range< Iter >::type value_type;

            typedef iterator_facade< group_by_iterator< Pred, Iter >,
                                     value_type,
                                     forward_traversal_tag,
                                     value_type > base_t;
        };

        // group_by_iterator for wrapping a forward traversal iterator
        template < class Pred, class Iter >
        class group_by_iterator
            : public group_by_iterator_base_gen< Pred, Iter >::base_t
        {
            typedef typename group_by_iterator_base_gen< Pred, Iter >::base_t super_t;

            struct negated_pred
            {
                explicit negated_pred( Pred pred ) : m_pred( pred ) {}

                template < typename T >
                bool operator()( const T& v1, const T& v2 )
                {
                    return !m_pred( v1, v2 );
                }

                Pred m_pred;
            };
            typedef
                typename skip_iterator< Iter, negated_pred, true /* default_pass */ >
                    increment_iterator;

            friend class ::boost::iterator_core_access;

        public:
            typedef Pred pred_t;
            typedef Iter iter_t;

            group_by_iterator() : m_it(), m_next() {}

            group_by_iterator( pred_t p, iter_t it, iter_t last )
            : m_it( it ), m_next( it, last, negated_pred( p ) )
            {
                increment();
            }

        private:
            void increment()
            {
                m_it = m_next.base();
                if (m_next.base() != m_next.m_last)
                {
                    ++m_next;
                }
            }

            typename super_t::reference dereference() const
            {
                return typename super_t::reference( m_it, m_next.base() );
            }

            bool equal( const group_by_iterator& other ) const
            {
                return m_it == other.m_it;
            }

            iter_t m_it;
            increment_iterator m_next;
        };

        template < class Pred, class Range >
        struct group_by_range
            : iterator_range<
                  group_by_iterator< Pred,
                                     typename range_iterator< Range >::type > >
        {
        private:
            //typedef group_by_iterator< Pred,
            //                           typename range_iterator< Range >::type >
            //    group_by_iter;
            //typedef iterator_range< skip_iter > base_range;

        public:
            //group_by_range() {}
            group_by_range( const Pred& p, Range& r )
            : iterator_range( iterator( p, boost::begin( r ), boost::end( r ) ),
                              iterator( p, boost::end( r ), boost::end( r ) ) )
            {
            }
        };

        template < class Pred >
        class group_by_holder : public holder< Pred >
        {
        public:
            explicit group_by_holder( Pred value ) : holder< Pred >( value ) {}
        };

        template< class ForwardRng, class BinPredicate >
        inline group_by_range< BinPredicate, ForwardRng > operator|(
            ForwardRng& rng,
            const group_by_holder< BinPredicate >& pred )
        {
            //BOOST_RANGE_CONCEPT_ASSERT((ForwardRangeConcept<ForwardRng>));

            return group_by_range< BinPredicate, ForwardRng >( pred.val, rng );
        }

        template< class ForwardRng, class BinPredicate >
        inline group_by_range< BinPredicate, const ForwardRng > operator|(
            const ForwardRng& rng,
            const group_by_holder< BinPredicate >& pred )
        {
            //BOOST_RANGE_CONCEPT_ASSERT((ForwardRangeConcept<const ForwardRng>));

            return group_by_range< BinPredicate, const ForwardRng >( pred.val, rng );
        }

    } // namespace range_detail

    using range_detail::group_by_range;

    namespace adaptors
    {

        namespace
        {
            const range_detail::forwarder< range_detail::group_by_holder >
                grouped_by =
                    range_detail::forwarder< range_detail::group_by_holder >();
        }

        template < class ForwardRng, class BinPredicate >
        inline group_by_range< BinPredicate, ForwardRng > group_by(
            ForwardRng& rng,
            BinPredicate group_by_pred )
        {
            BOOST_RANGE_CONCEPT_ASSERT( (ForwardRangeConcept< ForwardRng >));

            return group_by_range< BinPredicate, ForwardRng >( group_by_pred,
                                                               rng );
        }

        template < class ForwardRng, class BinPredicate >
        inline group_by_range< BinPredicate, const ForwardRng > group_by(
            const ForwardRng& rng,
            BinPredicate group_by_pred )
        {
            BOOST_RANGE_CONCEPT_ASSERT(
                (ForwardRangeConcept< const ForwardRng >));

            return group_by_range< BinPredicate, const ForwardRng >(
                step, group_by_pred );
        }

    } // namespace 'adaptors'
} // namespace 'boost'

#endif
