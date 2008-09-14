// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
// License.  
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_INTERNAL_COMPLEX_ITER_BASE_HH
# define MLN_CORE_INTERNAL_COMPLEX_ITER_BASE_HH

/// \file mln/core/internal/complex_iter_base.hh
/// \brief Definition of an implementation (factoring) class for
/// iterators on mln::complex.

# include <limits>

# include <mln/metal/equal.hh>

# include <mln/core/concept/iterator.hh>
# include <mln/core/complex.hh>

namespace mln
{

  namespace internal
  {
    /// \brief Factoring class for iterators on mln::complex.
    ///
    /// \arg \p F The type of the face handle.
    /// \arg \p E The type exact type of the iterator.
    template <typename F, typename E>
    class complex_iter_base_ : public Iterator<E>
    {
      typedef complex_iter_base_<F, E> self_;

    public:
      typedef F face;
      // FIXME: Maybe we could just get the dimension D of the face's
      // complex, an define complex_type as mln::complex<D>?
      typedef typename F::complex_type complex_type;

      /// Construction and assignment.
      /// \{
      /* FIXME: Keep this non-const?  See a (big) comment about this in
	 milena/tests/complex_image.cc.   */
      complex_iter_base_();
      complex_iter_base_(complex_type& c);
      complex_iter_base_(const self_& rhs);
      self_& operator= (const self_& rhs);
      /// \}

      /// Manipulation.
      /// \{
      /// Test if the iterator is valid.
      bool is_valid() const;
      /// Invalidate the iterator.
      void invalidate();
      /// \}

      /// Conversion and accessors.
      /// \{
      /// Reference to the corresponding face handle.
      const face& to_face () const;
      /// Convert the iterator into an face handle.
      operator face() const;
      /// \}

    protected:
      /// The face handle this iterator is pointing to.
      face face_;
    };


    /* FIXME: This hand-made delegation is painful.  We should rely on
       the general mechanism provided by Point_Site.  But then again, we
       need to refine/adjust the interface of Point_Site w.r.t. the
       mandatory conversions to points.  */
    template <typename F, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const complex_iter_base_<F, E>& p);

  } // end of mln::internal



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename F, typename E>
    inline
    complex_iter_base_<F, E>::complex_iter_base_()
    {
      // Ensure F and E are compatible.
      mlc_equal(F, typename E::face)::check();

      invalidate();
    }

    template <typename F, typename E>
    inline
    complex_iter_base_<F, E>::complex_iter_base_(complex_type& c)
    {
      // Ensure F and E are compatible.
      mlc_equal(F, typename E::face)::check();

      face_.set_cplx(c);
      // Invalidate face_.
      invalidate();
    }

    template <typename F, typename E>
    inline
    complex_iter_base_<F, E>::complex_iter_base_(const complex_iter_base_<F, E>& rhs)
      : face_(rhs.face_)
    {
      // Ensure F and E are compatible.
      mlc_equal(F, typename E::face)::check();
    }

    template <typename F, typename E>
    inline
    complex_iter_base_<F, E>&
    complex_iter_base_<F, E>::operator=(const complex_iter_base_<F, E>& rhs)
    {
      if (&rhs == this)
	return *this;
      face_ = rhs.face_;
      return *this;
    }

    template <typename F, typename E>
    inline
    bool
    complex_iter_base_<F, E>::is_valid() const
    {
      return face_.is_valid();
    }

    template <typename F, typename E>
    inline
    void
    complex_iter_base_<F, E>::invalidate()
    {
      face_.invalidate();
    }

    template <typename F, typename E>
    inline
    const F&
    complex_iter_base_<F, E>::to_face() const
    {
      return face_;
    }

    template <typename F, typename E>
    inline
    complex_iter_base_<F, E>::operator F() const
    {
      mln_precondition(is_valid());
      return face_;
    }


    template <typename F, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const complex_iter_base_<F, E>& p)
    {
    /* FIXME: We should use p.to_face() here, but as it lacks the
       precondition the conversion operator has, so we use the latter.

       We should
       - rename `to_face' as `to_face_';
       - write a new `to_face' routine checking the validity of the
         iterator;
       - have the conversion operator to face use this new `to_face'
         routine;
       - adjust former clients of `to_face'

       This is a general remark that applies to all iterators of
       Milena.  */
      F f = p;
      return ostr << "(dim = " << f.n() << ", id = " << f.face_id() << ')';
    }

  } // end of mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of mln

#endif // ! MLN_CORE_INTERNAL_COMPLEX_ITER_BASE_HH
