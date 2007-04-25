// Copyright (C) 2007 EPITA
// Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_CORE_INTERNAL_FAST_ITERATOR_BASE_HH
# define OLN_CORE_INTERNAL_FAST_ITERATOR_BASE_HH

# include <oln/core/concept/fast_iterator.hh>

namespace oln
{

  // fast iterator on image based on a array

  // Fwd decl.
  namespace internal { template <typename Exact> class fast_iterator_base_; }

  // Super type.
  template <typename Exact>
  struct super_trait_< internal::fast_iterator_base_<Exact> >
  {
    typedef Fast_Iterator<Exact> ret;
  };

  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::fast_iterator_base_<Exact> >
  {
    typedef stc::abstract value;
  };

  namespace internal
  {
    // Fast iterator for image in one dimension
    template <typename Exact>
    class fast_iterator_base_ : public Fast_Iterator< Exact >
    {
      typedef fast_iterator_base_<Exact> current;
      typedef Fast_Iterator<Exact> super;
    public:
      stc_using(value);

      void impl_start();
      void impl_next();
      void impl_invalidate();
      bool impl_is_valid() const;

      value& impl_dereference();
      const value& impl_dereference() const;

    protected:
      fast_iterator_base_();

      value* start_;            // buffer start
      value* current_elt_;
      value* eoi_;              // buffer end;
    };

# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    fast_iterator_base_<Exact>::fast_iterator_base_()
    {
    }

    template <typename Exact>
    void
    fast_iterator_base_<Exact>::impl_start()
    {
      this->current_elt_ = this->start_;
    }

    template <typename Exact>
    void
    fast_iterator_base_<Exact>::impl_invalidate()
    {
      this->current_elt_ = this->eoi_;
    }

    template <typename Exact>
    bool
    fast_iterator_base_<Exact>::impl_is_valid() const
    {
      return this->current_elt_ != this->eoi_;
    }

    template <typename Exact>
    typename fast_iterator_base_<Exact>::value&
    fast_iterator_base_<Exact>::impl_dereference()
    {

      return *(this->current_elt_);
    }

    template <typename Exact>
    const typename  fast_iterator_base_<Exact>::value&
    fast_iterator_base_<Exact>::impl_dereference() const
    {
      return *(this->current_elt_);
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace internal


  // fast iterator on array image without border

  // Fwd decl.
  namespace internal
  {
    template <typename Exact> class fast_iterator_without_b_;
  }

  // Super type.
  template <typename Exact>
  struct super_trait_< internal::fast_iterator_without_b_<Exact> >
  {
    typedef internal::fast_iterator_base_<Exact> ret;
  };

  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::fast_iterator_without_b_<Exact> >
  {
  };

  namespace internal
  {

    template <typename Exact>
    class fast_iterator_without_b_ : public internal::fast_iterator_base_<Exact>
    {
      typedef fast_iterator_without_b_<Exact> current;
      typedef internal::fast_iterator_base_<Exact> super;

    public:
      void impl_next();

    protected:
      fast_iterator_without_b_();
    };

# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    fast_iterator_without_b_<Exact>::fast_iterator_without_b_()
    {
    }

    template <typename Exact>
    void
    fast_iterator_without_b_<Exact>::impl_next()
    {
      ++(this->current_elt_);
    }

# endif // ! OLN_INCLUDE_ONLY


  } // end of namespace internal

} // end of namespace oln

#endif // OLN_CORE_1D_FAST_ITERATOR_1D_HH
