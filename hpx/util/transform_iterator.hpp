//  Copyright (c) 2007-2015 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_UTIL_TRANSFORM_ITERATOR_MAR_19_2015_0813AM)
#define HPX_UTIL_TRANSFORM_ITERATOR_MAR_19_2015_0813AM

#include <hpx/util/result_of.hpp>
#include <hpx/traits/segmented_iterator_traits.hpp>

#include <boost/iterator/iterator_adaptor.hpp>

#include <type_traits>
#include <iterator>

namespace hpx { namespace util
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Transformer,
            typename Reference = void, typename Value = void>
    class transform_iterator;

    namespace detail
    {
        template <typename Iterator, typename Transformer, typename Reference,
            typename Value>
        struct transform_iterator_base
        {
            typedef typename std::conditional<
                    std::is_void<Reference>::value,
                    typename util::result_of<Transformer(Iterator)>::type,
                    Reference
                >::type reference_type;

            typedef typename std::conditional<
                    std::is_void<Value>::value,
                    typename std::remove_reference<reference_type>::type,
                    Value
                >::type value_type;

            typedef boost::iterator_adaptor<
                transform_iterator<Iterator, Transformer, Reference, Value>,
                Iterator, value_type, boost::use_default, value_type
            > type;
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    // The main difference to boost::transform_iterator is that the transformer
    // function will be invoked with the iterator, not with the result of
    // dereferencing the base iterator.
    template <typename Iterator, typename Transformer, typename Reference,
        typename Value>
    class transform_iterator
      : public detail::transform_iterator_base<
            Iterator, Transformer, Reference, Value
        >::type
    {
    private:
        typedef typename detail::transform_iterator_base<
                Iterator, Transformer, Reference, Value
            >::type base_type;

    public:
        transform_iterator() {}

        explicit transform_iterator(Iterator const& it)
          : base_type(it)
        {}
        transform_iterator(Iterator const& it, Transformer const& f)
          : base_type(it), transformer_(f)
        {}

        Transformer const& transformer() const
        {
            return transformer_;
        }

    private:
        friend class boost::iterator_core_access;

        typename base_type::reference dereference() const
        {
            return transformer_(this->base());
        }

        Transformer transformer_;
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Transformer, typename Iterator>
    inline transform_iterator<Iterator, Transformer>
    make_transform_iterator(Iterator const& it, Transformer const& f)
    {
        return transform_iterator<Iterator, Transformer>(it, f);
    }

    template <typename Transformer, typename Iterator>
    inline transform_iterator<Iterator, Transformer>
    make_transform_iterator(Iterator const& it)
    {
        return transform_iterator<Iterator, Transformer>(it, Transformer());
    }
}}

#endif

