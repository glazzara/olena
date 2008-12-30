// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_WIN_LINE_HH
# define MLN_WIN_LINE_HH

/// \file mln/win/line.hh
///
/// Definition of the mln::win::line window.

# include <mln/core/internal/classical_window_base.hh>
# include <mln/core/dpoint.hh>


namespace mln
{

  // Forward declaration.
  namespace win { template <typename M, unsigned i, typename C> struct line; }


  namespace trait
  {

    template <typename M, unsigned i, typename C>
    struct window_< mln::win::line<M,i,C> > : classical_window_
    {
    };

  } // end of namespace trait



  namespace win
  {
 
    /*! \brief Generic line window defined on a given grid in the
     *  given dimension.
     *
     * An line is centered and symmetric; so its length is odd.
     *
     * M is the given grid on which the line is defined, i is the
     * given dimension of the line end C is the type of the
     * coordinates.
     *
     * \see mln::win::hline2d for an exemple of his use.
     */
    template <typename M, unsigned i, typename C>
    struct line : public internal::classical_window_base< dpoint<M, C>, line<M,i,C> >
    {
      /// Direction.
      enum { dir = i };

      /// Constructor.
      /// \param[in] length Length of the line.
      /// \pre \p length is odd.
      line(unsigned length);
	
      /// Give the line length.
      unsigned length() const;
	
      /// Give the line size, that is, its length.
      unsigned size() const;
	
      /// Give the maximum coordinate gap between the window
      /// center and a window point.
      unsigned delta_() const;

      void print_(std::ostream& ostr) const;

    protected:
      unsigned length_;
    };

 

# ifndef MLN_INCLUDE_ONLY


    template <typename M, unsigned i, typename C>
    inline
    line<M,i,C>::line(unsigned length)
      : length_(length)
    {
      metal::bool_< i < M::dim >::check();
      mln_precondition(length % 2 == 1);
      dpoint<M,C> n;
      n.set_all(0);
      const def::coord
	dc = static_cast<def::coord>(length / 2),
	minus_dc = static_cast<def::coord>(- dc);
      for (def::coord c = minus_dc; c <= dc; ++c)
      {
	n[i] = c;
	this->insert(n);
      }
    }

    template <typename M, unsigned i, typename C>
    inline
    unsigned line<M,i,C>::length() const
    {
      return length_;
    }

    template <typename M, unsigned i, typename C>
    inline
    unsigned line<M,i,C>::size() const
    {
      return length_;
    }

    template <typename M, unsigned i, typename C>
    inline
    unsigned line<M,i,C>::delta_() const
    {
      return length_ / 2;
    }

    template <typename M, unsigned i, typename C>
    inline
    void
    line<M,i,C>::print_(std::ostream& ostr) const
    {
      ostr << "[line: length=" << length_ << ']';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_WIN_LINE_HH
