// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_INTERNAL_DPSITES_IMPL_HH
# define MLN_CORE_INTERNAL_DPSITES_IMPL_HH

/*! \file mln/core/internal/dpsites_impl.hh
 *
 * \brief Definition of a base class for classes based on a set of dpoints.
 *
 * \todo Rename as dpsites_impl.
 *
 * \todo Remove the .vect() method.
 */

# include <mln/core/window.hh>
# include <mln/core/dpsites_piter.hh>



namespace mln
{

  namespace internal
  {

    /*! FIXME.
     *
     */
    template <typename D, typename E>
    class dpsites_impl
    {
    public:

      /// Forward site iterator associated type.
      typedef dpsites_fwd_piter<E> fwd_qiter;

      /// Backward site iterator associated type.
      typedef dpsites_fwd_piter<E> bkd_qiter;

      /// Site iterator associated type.
      typedef fwd_qiter qiter;


      /*! \brief Test if the window is centered.
       *
       * \return True if the delta-point 0 belongs to the window.
       */
      bool is_centered() const;

      /*! \brief Test if the window is empty (null size; no delta-point).
       */
      bool is_empty() const;

      /*! \brief Give the maximum coordinate gap between the window
	center and a window point.
      */
      unsigned delta() const;

      /// Give the number of delta-points.
      unsigned size() const;

      /// Test if the delta-point \p dp belongs to the window.
      bool has(const D& dp) const;

      // Give the \p i-th delta-point.
      const D& dp(unsigned i) const;

      // Give the vector of delta-points.
      const std::vector<D>& vect() const;

      // Give the vector of delta-points.
      const std::vector<D>& std_vector() const;

    protected:

      dpsites_impl();

      void insert(const D& d);

      mln::window<D> win_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename D, typename E>
    inline
    dpsites_impl<D,E>::dpsites_impl()
    {
    }

    template <typename D, typename E>
    inline
    bool dpsites_impl<D,E>::is_centered() const
    {
      return win_.is_centered();
    }

    template <typename D, typename E>
    inline
    bool dpsites_impl<D,E>::is_empty() const
    {
      return win_.is_empty();
    }

    template <typename D, typename E>
    inline
    unsigned dpsites_impl<D,E>::delta() const
    {
      return win_.delta();
    }

    template <typename D, typename E>
    inline
    unsigned
    dpsites_impl<D,E>::size() const
    {
      return win_.size();
    }

    template <typename D, typename E>
    inline
    const D&
    dpsites_impl<D,E>::dp(unsigned i) const
    {
      mln_precondition(i < size());
      return win_.dp(i);
    }

    template <typename D, typename E>
    inline
    const std::vector<D>&
    dpsites_impl<D,E>::std_vector() const
    {
      return win_.std_vector();
    }

    template <typename D, typename E>
    inline
    const std::vector<D>&
    dpsites_impl<D,E>::vect() const
    {
      return std_vector();
    }

    template <typename D, typename E>
    inline
    bool
    dpsites_impl<D,E>::has(const D& dp) const
    {
      return win_.has(dp);
    }

    template <typename D, typename E>
    inline
    void
    dpsites_impl<D,E>::insert(const D& d)
    {
      win_.insert(d);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_DPSITES_IMPL_HH
