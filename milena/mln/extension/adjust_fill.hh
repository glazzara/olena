// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_EXTENSION_ADJUST_FILL_HH
# define MLN_EXTENSION_ADJUST_FILL_HH

/// \file
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

    /*! \brief Adjust then fill.
     *
     * \ingroup mlnborderext
     */
    template <typename I, typename W>
    void adjust_fill(const Image<I>& ima,
		     const Window<W>& win,
		     const mln_value(I)& val);

    /*! \overload
     *
     * \ingroup mlnborderext
     */
    template <typename I, typename W>
    void adjust_fill(const Image<I>& ima,
		     const Weighted_Window<W>& wwin,
		     const mln_value(I)& val);

    /*! \overload
     *
     * \ingroup mlnborderext
     */
    template <typename I, typename N>
    void adjust_fill(const Image<I>& ima,
		     const Neighborhood<N>& nbh,
		     const mln_value(I)& val);

    /*! \overload
     *
     * \ingroup mlnborderext
     */
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


#endif // ! MLN_EXTENSION_ADJUST_FILL_HH
