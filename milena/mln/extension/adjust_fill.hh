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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_EXTENSION_ADJUST_FILL_HH
# define MLN_CORE_EXTENSION_ADJUST_FILL_HH

/// \file mln/extension/adjust_fill.hh
///
/// Adjust then fill the domain extension.
///
/// \todo Fix doc.

# include <mln/extension/adjust.hh>
# include <mln/extension/fill.hh>


namespace mln
{

  namespace extension
  {

    /// Adjust then fill.
    template <typename I, typename W>
    void adjust_fill(const Image<I>& ima,
		     const Window<W>& win,
		     const mln_value(I)& val);

    template <typename I, typename W>
    void adjust_fill(const Image<I>& ima,
		     const Weighted_Window<W>& wwin,
		     const mln_value(I)& val);

    template <typename I, typename N>
    void adjust_fill(const Image<I>& ima,
		     const Neighborhood<N>& nbh,
		     const mln_value(I)& val);

    template <typename I>
    void adjust_fill(const Image<I>& ima,
		     unsigned delta,
		     const mln_value(I)& val);


# ifndef MLN_INCLUDE_ONLY

    // Facades.

    template <typename I, typename W>
    void adjust_fill(const Image<I>& ima,
		     const Window<W>& win,
		     const mln_value(I)& val)
    {
      trace::entering("extension::adjust_fill");
      extension::adjust(ima, win);
      extension::fill(ima, val);
      trace::exiting("extension::adjust_fill");
    }

    template <typename I, typename W>
    void adjust_fill(const Image<I>& ima,
		     const Weighted_Window<W>& wwin,
		     const mln_value(I)& val)
    {
      trace::entering("extension::adjust_fill");
      extension::adjust(ima, wwin);
      extension::fill(ima, val);
      trace::exiting("extension::adjust_fill");
    }

    template <typename I, typename N>
    void adjust_fill(const Image<I>& ima,
		     const Neighborhood<N>& nbh,
		     const mln_value(I)& val)
    {
      trace::entering("extension::adjust_fill");
      extension::adjust(ima, nbh);
      extension::fill(ima, val);
      trace::exiting("extension::adjust_fill");
    }

    template <typename I>
    void adjust_fill(const Image<I>& ima,
		     unsigned delta,
		     const mln_value(I)& val)
    {
      trace::entering("extension::adjust_fill");
      extension::adjust(ima, delta);
      extension::fill(ima, val);
      trace::exiting("extension::adjust_fill");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::extension

} // end of namespace mln


#endif // ! MLN_CORE_EXTENSION_ADJUST_FILL_HH
