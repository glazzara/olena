// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_INTERNAL_INDEX_SITE_BASE_HH
# define MLN_CORE_INTERNAL_INDEX_SITE_BASE_HH

# include <mln/core/concept/index_site.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/value/concept/integer.hh>

namespace mln
{
  /// \{ Forward declarations.
  namespace literal
  {
    struct zero_t;
    struct one_t;
  }

  template <typename E> struct dindex_site;
  /// \}


  namespace internal
  {

    template <typename E> struct index_site_base;

    template <typename E = void>
    struct index_site_base : public Index_Site<E>
    {
      typedef index_site_base<E> self;

      typedef index_site_base<E> site;
      typedef index_site_base<E> psite;

      typedef dindex_site<E> delta;
      typedef dindex_site<E> dpsite;

      typedef unsigned index_t;

      /// Constructors
      /// \{
      index_site_base();
      explicit index_site_base(unsigned i);
      index_site_base(const literal::zero_t&);
      index_site_base(const literal::one_t&);
      /// \}

      /// Index materials
      /// \{
      /// Return the current index.
      const index_t& index() const;
      /// Change the current index.
      void change_index(const index_t&);
      /// Increment the current index.
      void inc_index();
      /// Decrement the current index.
      void dec_index();
      /// \}

      /// Operators
      /// \{
      self& operator+= (const delta&);
      self& operator-= (const delta&);
      /// \}

    protected:
      index_t i_;
    };


    /// subject_impl specialization (Proxy)
    /// \{

    template <typename P, typename E>
    struct subject_impl< const index_site_base<P>, E >
    {
      //const typename index_site_base<P>::index_t& index() const;

    private:
      const E& exact_() const;
    };

    template <typename P, typename E>
    struct subject_impl< index_site_base<P>, E > :
      subject_impl< const index_site_base<P>, E >
    {
      //void change_index(const typename index_site_base<P>::index_t& idx);
      void inc_index();
      void dec_index();

    private:
      E& exact_();
    };

    /// \}


# ifndef MLN_INCLUDE_ONLY

    template <typename E>
    inline
    index_site_base<E>::index_site_base()
    {
    }

    template <typename E>
    inline
    index_site_base<E>::index_site_base(unsigned i)
      : i_ (i)
    {
    }

    template <typename E>
    inline
    index_site_base<E>::index_site_base(const literal::zero_t&)
      : i_ (0)
    {
    }

    template <typename E>
    inline
    index_site_base<E>::index_site_base(const literal::one_t&)
      : i_ (1)
    {
    }

    template <typename E>
    inline
    const typename index_site_base<E>::index_t&
    index_site_base<E>::index() const
    {
      return i_;
    }

    template <typename E>
    inline
    void
    index_site_base<E>::dec_index()
    {
      --i_;
    }

    template <typename E>
    inline
    void
    index_site_base<E>::inc_index()
    {
      ++i_;
    }


    template <typename E>
    inline
    void
    index_site_base<E>::change_index(const index_t& idx)
    {
      i_ = idx;
    }

    template <typename E>
    inline
    index_site_base<E>&
    index_site_base<E>::operator+=(const delta& dp)
    {
      i_ += dp.value();
      return *this;
    }

    template <typename E>
    inline
    index_site_base<E>&
    index_site_base<E>::operator-=(const delta& dp)
    {
      mln_precondition(dp.value() >= i_);
      i_ -= dp.value();
      return *this;
    }


    template <typename P, typename E>
    const E&
    subject_impl< const index_site_base<P>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename P, typename E>
    E&
    subject_impl< index_site_base<P>, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }

    // template <typename P, typename E>
    // const typename index_site_base<P>::index_t&
    // subject_impl< const index_site_base<P>, E >::index() const
    // {
    //   return exact_().get_subject().index();
    // }

    // template <typename P, typename E>
    // void
    // subject_impl< index_site_base<P>, E >::change_index(const typename index_site_base<P>::index_t& idx)
    // {
    //   return exact_().get_subject().change_index(idx);
    // }

    template <typename P, typename E>
    void
    subject_impl< index_site_base<P>, E >::inc_index()
    {
      return exact_().get_subject().inc_index();
    }

    template <typename P, typename E>
    void
    subject_impl< index_site_base<P>, E >::dec_index()
    {
      return exact_().get_subject().dec_index();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

# include <mln/core/dindex_site.hh>

#endif // !MLN_CORE_INDEX_SITE_BASE_HH
