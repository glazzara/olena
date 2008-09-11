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

#ifndef MLN_CORE_NEIGHB_HH
# define MLN_CORE_NEIGHB_HH

/*! \file mln/core/neighb.hh
 *
 * \brief Definition of a window-to-neighborhood adapter.
 */

# include <mln/core/internal/neighborhood_base.hh>



namespace mln
{
 

  /*! \brief Adapter class from window to neighborhood.
   */
  template <typename W>
  class neighb : public internal::neighborhood_base< mln_dpsite(W), neighb<D> >,
		 private mlc_is_a(W, Window)::check_t
  {
  public:

    /// Forward site iterator associated type.
    typedef mln_fwd_qiter(W) fwd_niter;

    /// Backward site iterator associated type.
    typedef mln_bkd_qiter(W) bkd_niter;

    /// Site iterator associated type.
    typedef mln_qiter(W) niter;


    /// Constructor without argument.
    neighb();

    /// Constructor from a window \p win.
    neighb(const W& win);

    /// Get the corresponding window.
    const W& win() const;

  private:
    W win_;
  };



  namespace convert
  {
    namespace impl
    {

      template <typename W>
      void
      from_to_(const mln::neighb<W>& from, W& to);

    } // end of namespace convert::impl

  } // end of namespace convert

 

# ifndef MLN_INCLUDE_ONLY

  template <typename W>
  inline
  neighb<W>::neighb()
  {
  }

  template <typename W>
  inline
  neighb<W>::neighb(const W& win)
  {
    mln_precondition(win.is_neighbable_());
    win_ = win;
  }

  template <typename W>
  inline
  const W&
  neighb<W>::win() const
  {
    return win_;
  }


  namespace convert
  {
    namespace impl
    {

      template <typename W>
      void
      from_to_(const mln::neighb<W>& from, W& to)
      {
	to = from.win();
      }

    } // end of namespace convert::impl

  } // end of namespace convert

 
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_NEIGHB_HH
