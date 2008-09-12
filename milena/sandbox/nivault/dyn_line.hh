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

#ifndef MLN_CORE_WIN_DYN_LINE_HH
# define MLN_CORE_WIN_DYN_LINE_HH

/*! \file mln/win/dyn_line.hh
 *
 * \brief Definition of the mln::win::dyn_line window.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/internal/dpoints_base.hh>
# include <mln/core/dpoint.hh>
# include <mln/core/dpsites_piter.hh>


namespace mln
{

  namespace win
  {
 
    template <typename M, typename C>
    struct dyn_line : public Window< dyn_line<M,C> >,
		      public internal::dpoints_base_<dpoint_<M, C>, point_<M, C> >
    {
      /// Point associated type.
      typedef point_<M, int> point;

      /// Dpoint associated type.
      typedef dpoint_<M, int> dpoint;

      /// Site_Iterator type to browse a dyn_line forward
      typedef dpsites_fwd_piter<dpoint> fwd_qiter;

      /// Site_Iterator type to browse a dyn_line backward
      typedef dpsites_bkd_piter<dpoint> bkd_qiter;

      /// Same as fwd_qiter
      typedef fwd_qiter qiter;

      /*! \brief Constructor.
       *
       * \param[in] length Length of the dyn_line.
       *
       * \pre \p length is odd.
       */
      dyn_line(unsigned length, unsigned direction);

      /*! \brief Test if the window is centered.
       *
       * \return True.
       */
      bool is_centered() const;
	
      /*! \brief Test if the window is symmetric.
       *
       * \return true.
       */
      bool is_symmetric() const;
	
      /*! \brief Give the dyn_line length, that is, its width.
       */
      unsigned length() const;
	
      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta() const;

      /// Apply a central symmetry to the target window.
      dyn_line<M,C>& sym();
		
      protected:
	unsigned length_;
    };


    /*! \brief Print an dyn_line window \p win into the output
     *  stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] win An dyn_line window.
     *
     * \return The modified output stream \p ostr.
     *
     * \relates mln::win::dyn_line
     */
    template <typename M, typename C>
    std::ostream& operator<<(std::ostream& ostr, const dyn_line<M,C>& win);

 

# ifndef MLN_INCLUDE_ONLY


    template <typename M, typename C>
    dyn_line<M,C>::dyn_line(unsigned length, unsigned i)
      : length_(length)
    {
      mln_precondition(i < M::dim);
      mln_precondition(length % 2 == 1);
      const int dc = length / 2;
      for (int c = - dc; c <= dc; ++c)
      {
	dpoint n;
	n.set_all(0);
	n[i] = c;
	this->insert(n);
      }
    }

    template <typename M, typename C>
    bool dyn_line<M,C>::is_centered() const
    {
      return true;
    }

    template <typename M, typename C>
    bool dyn_line<M,C>::is_symmetric() const
    {
      return true;
    }

    template <typename M, typename C>
    unsigned dyn_line<M,C>::length() const
    {
      return length_;
    }

    template <typename M, typename C>
    unsigned dyn_line<M,C>::delta() const
    {
      return length_ / 2;
    }

    template <typename M, typename C>
    dyn_line<M,C>& dyn_line<M,C>::sym()
    {
      return *this;
    }

    template <typename M, typename C>
    std::ostream& operator<<(std::ostream& ostr, const dyn_line<M,C>& win)
    {
      ostr << "[dyn_line: length=" << win.length() << ']';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_CORE_WIN_DYN_LINE_HH
