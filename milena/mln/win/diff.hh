// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_WIN_DIFF_HH
# define MLN_WIN_DIFF_HH

/// \file
///
/// \brief Set difference between a couple of windows or neighborhoods.

# include <mln/trait/op/minus.hh>
# include <mln/core/window.hh>
# include <mln/core/neighb.hh>



namespace mln
{

  namespace win
  {

    /// \brief Set difference between a couple of windows \p win1 and \p
    /// win2.
    //
    template <typename W1, typename W2>
    mln_regular(W1)
    diff(const Window<W1>& win1, const Window<W2>& win2);


    /// \brief Set difference between a couple of neighborhoods \p nbh1
    /// and \p nbh2.
    //
    template <typename N1, typename N2>
    neighb<mln_deduce(N1, window, regular)>
    diff(const Neighborhood<N1>& nbh1, const Neighborhood<N2>& nbh2);

  } // end of namespace mln::win


  /// \brief Set difference between a couple of windows \p win1 and \p
  /// win2.  It just calls mln::win::diff.
  ///
  //
  template <typename W1, typename W2>
  mln_regular(W1)
  operator-(const Window<W1>& win1, const Window<W2>& win2);

  /// \brief Set difference between a couple of neighborhoods \p nbh1
  /// and \p nbh2.  It just calls mln::win::diff.
  //
  template <typename N1, typename N2>
  neighb<mln_deduce(N1, window, regular)>
  operator-(const Neighborhood<N1>& nbh1, const Neighborhood<N2>& nbh2);


  namespace trait
  {

    template <typename W1, typename W2>
    struct set_binary_< op::minus, mln::Window, W1,  mln::Window, W2 >
    {
      typedef mln_regular(W1) ret;
    };
    

    template <typename N1, typename N2>
    struct set_binary_< op::minus, mln::Neighborhood, N1,  mln::Neighborhood, N2 >
    {
      typedef mln::neighb<mln_deduce(N1, window, regular)> ret;
    };

  } // end of namespace mln::trait



# ifndef MLN_INCLUDE_ONLY

  namespace win
  {

    template <typename W1, typename W2>
    inline
    mln_regular(W1)
    diff(const Window<W1>& win1_, const Window<W2>& win2_)
    {
      mln_trace("win::diff");

      mln_is_simple_window(W1)::check();
      mln_is_simple_window(W2)::check();

      const W1& win1 = exact(win1_);
      const W2& win2 = exact(win2_);
      mln_regular(W1) tmp;

      const unsigned n = win1.size();
      for (unsigned i = 0; i < n; ++i)
	{
	  if (win2.has(win1.dp(i)))
	    continue;
	  tmp.insert(win1.dp(i));
	}

      return tmp;
    }

    template <typename N1, typename N2>
    neighb<mln_deduce(N1, window, regular)>
    diff(const Neighborhood<N1>& nbh1, const Neighborhood<N2>& nbh2)
    {
      typedef mln_deduce(N1, window, regular) W1;
      W1 win = diff(exact(nbh1).win(), exact(nbh2).win());
      neighb<W1> tmp(win);
      return tmp;
    }

  } // end of namespace mln::win


  // Operators.

  template <typename W1, typename W2>
  inline
  mln_regular(W1)
  operator-(const Window<W1>& win1, const Window<W2>& win2)
  {
    return win::diff(win1, win2);
  }

  template <typename N1, typename N2>
  neighb<mln_deduce(N1, window, regular)>
  operator-(const Neighborhood<N1>& nbh1, const Neighborhood<N2>& nbh2)
  {
    return win::diff(nbh1, nbh2);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_WIN_DIFF_HH
