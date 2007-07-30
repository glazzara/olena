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

#ifndef MLN_CORE_INTERNAL_RUN_PSITE_HH
# define MLN_CORE_INTERNAL_RUN_PSITE_HH

/*! \file mln/core/internal/run_psite.hh
 *
 * \brief Definition of class mln::internal::run_psite_ for internal use only
 */

namespace mln
{

  namespace internal
  {

    /*! \brief Psite class used in run_image_.
     *
     * Parameter \c P is the type of the image point.
     */
    template <typename P>
    class run_psite
    {
    public:
      run_psite();
      run_psite(P point, unsigned index, unsigned pset_pos);

      operator P () const;
      /// Return the point at the start of the current run.
      P& range_start_();
      /// Return the point at the start of the current run.
      const P& range_start_() const;
      /// Return the position of this psite in the point set.
      unsigned pset_pos_() const;
      /// Return the position of this psite in the point set.
      unsigned& pset_pos_();
      /// Return the position of this psite in the current range.
      unsigned index_() const;
      /// Return the position of this psite in the current range.
      unsigned& index_();

    protected:
      /// Start of the psite range.
      P point_;
      /// Position in the psite range.
      unsigned range_index_;
      /// Position of the psite in the point set.
      unsigned pset_position_;
    };

# ifndef MLN_INCLUDE_ONLY
    template <typename P>
    run_psite<P>::run_psite()
    {
    }

    template <typename P>
    run_psite<P>::run_psite(P point, unsigned index, unsigned pset_pos) :
      point_(point),
      range_index_(index),
      pset_position_(pset_pos)
    {
    }

    template <typename P>
    run_psite<P>::operator P() const
    {
      P tmp = point_;
      tmp[0] += range_index_;
      return tmp;
    }

    template <typename P>
    const P&
    run_psite<P>::range_start_() const
    {
      return point_;
    }

    template <typename P>
    P&
    run_psite<P>::range_start_()
    {
      return point_;
    }

    template <typename P>
    unsigned
    run_psite<P>::pset_pos_() const
    {
      return pset_position_;
    }

    template <typename P>
    unsigned&
    run_psite<P>::pset_pos_()
    {
      return pset_position_;
    }

    template <typename P>
    unsigned
    run_psite<P>::index_() const
    {
      return range_index_;
    }

    template <typename P>
    unsigned&
    run_psite<P>::index_()
    {
      return range_index_;
    }
# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_RUN_PSITE_HH
