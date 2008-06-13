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

#ifndef MLN_CORE_INTERNAL_BASIC_WINDOW_IMPL_HH
# define MLN_CORE_INTERNAL_BASIC_WINDOW_IMPL_HH

/*! \file mln/core/internal/basic_window_impl.hh
 *
 * \brief Definition of a base class for classes based on a set of dpoints.
 */

# include <mln/util/set.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/norm/linfty.hh>


namespace mln
{

  // Fwd decls.
  template <typename V> class dpsites_fwd_piter;
  template <typename V> class dpsites_bkd_piter;
 

  namespace internal
  {

    /*! \internal FIXME.
     *
     */
    template <typename D, typename E>
    class basic_window_impl
    {
      util::set<D> dps_;

    public:


      /*! \brief Site_Iterator type to browse the points of a basic window
       * w.r.t. the ordering of delta-points.
       */
      typedef dpsites_fwd_piter<E> fwd_qiter;
      
      /*! \brief Site_Iterator type to browse the points of a basic window
       * w.r.t. the reverse ordering of delta-points.
       */
      typedef dpsites_bkd_piter<E> bkd_qiter;

      /*! \brief Site_Iterator type to browse the points of a basic window
       * whatever the ordering of delta-points.
       */
      typedef fwd_qiter qiter;


      /*! \brief Test if the window is empty (null size; no delta-point).
       */
      bool is_empty() const;

      /*! \brief Give the maximum coordinate gap between the window
	center and a window point.
      */
      unsigned delta() const;

      /// Give the number of delta-points.
      unsigned ndpoints() const;

      /// Test if the delta-point \p dp belongs to the window.
      bool has(const D& dp) const;

      // Give the \p i-th delta-point.
      const D& dp(unsigned i) const;


      // Give the vector of delta-points.
      const std::vector<D>& std_vector() const;


      /// Insert a delta-point \p dp.
      E& insert(const D& dp);

      /// \{ Insertion of a delta-point with different numbers of
      /// arguments (coordinates) w.r.t. the dimension.
      E& insert(const mln_coord(D)& dind); // For 1D.
      
      E& insert(const mln_coord(D)& drow,
		const mln_coord(D)& dcol); // For 2D.
      
      E& insert(const mln_coord(D)& dsli,
		const mln_coord(D)& drow,
		const mln_coord(D)& dcol); // For 3D.
      /// \}


      const util::set<D>& dps_hook() const;

    protected:

      basic_window_impl();

      void insert_(const D& dp); // The only routine to effectively insert a dp.
      // This is a default implementation.  This routine can be
      // overidden in sub-classes.
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename D, typename E>
    inline
    basic_window_impl<D,E>::basic_window_impl()
    {
    }

    template <typename D, typename E>
    inline
    bool basic_window_impl<D,E>::is_empty() const
    {
      return dps_.is_empty();
    }

    template <typename D, typename E>
    inline
    unsigned basic_window_impl<D,E>::delta() const
    {
      // FIXME: Is-it correct?
      unsigned d = 0;
      const unsigned n = ndpoints();
      for (unsigned i = 0; i < n; ++i)
	{
	  unsigned dd = norm::linfty(dp(i).to_vec());
	  if (dd > d)
	    d = dd;
	}
      return d;
    }

    template <typename D, typename E>
    inline
    unsigned
    basic_window_impl<D,E>::ndpoints() const
    {
      return dps_.nelements();
    }

    template <typename D, typename E>
    inline
    const D&
    basic_window_impl<D,E>::dp(unsigned i) const
    {
      mln_precondition(i < ndpoints());
      return dps_[i];
    }

    template <typename D, typename E>
    inline
    const std::vector<D>&
    basic_window_impl<D,E>::std_vector() const
    {
      return dps_.vect();
    }

    template <typename D, typename E>
    inline
    bool
    basic_window_impl<D,E>::has(const D& dp) const
    {
      return dps_.has(dp);
    }

    template <typename D, typename E>
    inline
    E&
    basic_window_impl<D,E>::insert(const D& dp)
    {
      E& self_ = internal::force_exact<E>(*this);
      self_.insert_(dp);
      return self_;
    }

    template <typename D, typename E>
    inline
    E&
    basic_window_impl<D,E>::insert(const mln_coord(D)& dind)
    {
      D dp(dind);
      return insert(dp);
    }

    template <typename D, typename E>
    inline
    E&
    basic_window_impl<D,E>::insert(const mln_coord(D)& drow,
				   const mln_coord(D)& dcol)
    {
      D dp(drow, dcol);
      return insert(dp);
    }

    template <typename D, typename E>
    inline
    E&
    basic_window_impl<D,E>::insert(const mln_coord(D)& dsli,
				   const mln_coord(D)& drow,
				   const mln_coord(D)& dcol)
    {
      D dp(dsli, drow, dcol);
      return insert(dp);
    }

    template <typename D, typename E>
    inline
    void
    basic_window_impl<D,E>::insert_(const D& dp)
    {
      mln_precondition(! has(dp));
      dps_.insert(dp);
    }

    template <typename D, typename E>
    inline
    const util::set<D>&
    basic_window_impl<D,E>::dps_hook() const
    {
      return dps_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


# include <mln/core/dpsites_piter.hh>


#endif // ! MLN_CORE_INTERNAL_BASIC_WINDOW_IMPLHH
