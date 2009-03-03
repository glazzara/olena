// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_WIN_BALL_HH
# define MLN_WIN_BALL_HH

/// \file mln/win/ball.hh
///
/// Definition of the mln::win::ball window.

# include <mln/core/internal/classical_window_base.hh>
# include <mln/core/dpoint.hh>
# include <mln/core/site_set/box.hh>
# include <mln/fun/i2v/all_to.hh>


namespace mln
{

  // Forward declaration.
  namespace win { template <typename G, typename C> struct ball; }


  namespace trait
  {

    template <typename G, typename C>
    struct window_< mln::win::ball<G,C> > : classical_window_
    {
    };

  } // end of namespace trait



  namespace win
  {
 
    /*! \brief Generic ball window defined on a given grid.
     *
     * A ball is centered and symmetric; so its diameter is odd.
     *
     * G is the given grid on which the ball is defined and C is the
     * type of coordinates.
     */
    template <typename G, typename C>
    struct ball : public internal::classical_window_base< dpoint<G,C>, ball<G,C> >
    {

      /// Constructor.
      /// \param[in] diameter Diameter of the ball.
      /// \pre \p diameter is odd.
      ball(unsigned diameter);
	
      /// Give the ball diameter.
      unsigned diameter() const;

      /// Give the maximum coordinate gap between the window
      /// center and a window point.
      unsigned delta_() const;

      void print_(std::ostream& ostr) const;

    protected:
      unsigned diameter_;
    };

 

# ifndef MLN_INCLUDE_ONLY


    template <typename G, typename C>
    inline
    ball<G,C>::ball(unsigned diameter)
      : diameter_(diameter)
    {
      mln_precondition(diameter % 2 == 1);
      int
	radius = diameter / 2,
	sqr_radius = radius * radius;
      typedef point<G,C> P;
      box<P> b(all_to(-radius), all_to(+radius));
      P O = literal::origin;
      mln_piter(box<P>) p(b);
      for_all(p)
      {
	unsigned d = 0;
	for (unsigned i = 0; i < P::dim; ++i)
	  d += p[i] * p[i];
	if (d <= sqr_radius)
	  this->insert(p - O);
      }
    }

    template <typename G, typename C>
    inline
    unsigned ball<G,C>::diameter() const
    {
      return diameter_;
    }

    template <typename G, typename C>
    inline
    unsigned ball<G,C>::delta_() const
    {
      return diameter_ / 2;
    }

    template <typename G, typename C>
    inline
    void
    ball<G,C>::print_(std::ostream& ostr) const
    {
      ostr << "[" << G::dim << "D ball: diameter=" << diameter_ << ']';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_WIN_BALL_HH
