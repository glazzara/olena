// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_INTERNAL_DPOINTS_BASE_HH
# define MLN_CORE_INTERNAL_DPOINTS_BASE_HH

/*! \file mln/core/internal/dpoints_base.hh
 *
 * \brief Definition of a base class for classes based on a set of dpoints.
 */

# include <mln/core/internal/set_of.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/norm/linfty.hh>


namespace mln
{

  namespace internal
  {
 

    /*! \brief FIXME.
     */
    template <typename D, typename E>
    class dpoints_base_ : protected internal::set_of_<D>
    {
      typedef internal::set_of_<D> super_;
    public:

      /// Point associated type.
      typedef mln_point(D) point;

      /// Dpoint associated type.
      typedef D dpoint;


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
      unsigned ndpoints() const;

      /// Test if the delta-point \p dp belongs to the window.
      bool has(const D& dp) const;

      // Give the \p i-th delta-point.
      const D& dp(unsigned i) const;

      // Give the vector of delta-points.
      const std::vector<D>& vect() const;

    protected:
      dpoints_base_();
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename D, typename E>
    inline
    dpoints_base_<D,E>::dpoints_base_()
    {
    }

    template <typename D, typename E>
    inline
    bool dpoints_base_<D,E>::is_centered() const
    {
      static const D origin = all_to(0);
      return this->super_::has(origin);
    }

    template <typename D, typename E>
    inline
    bool dpoints_base_<D,E>::is_empty() const
    {
      return this->super_::is_empty();
    }

    template <typename D, typename E>
    inline
    unsigned dpoints_base_<D,E>::delta() const
    {
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
    dpoints_base_<D,E>::ndpoints() const
    {
      return this->super_::nelements();
    }

    template <typename D, typename E>
    inline
    const D&
    dpoints_base_<D,E>::dp(unsigned i) const
    {
      mln_precondition(i < ndpoints());
      return this->element(i);
    }

    template <typename D, typename E>
    inline
    const std::vector<D>&
    dpoints_base_<D,E>::vect() const
    {
      return this->super_::vect();
    }

    template <typename D, typename E>
    inline
    bool
    dpoints_base_<D,E>::has(const D& dp) const
    {
      return this->super_::has(dp);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_DPOINTS_BASE_HH
