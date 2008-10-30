// Copyright (C) 2008 EPITA Research and Development Laboratory
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

/*! \file mln/extension/adjust_fill.hh
 *
 * \brief Adjust then fill the domain extension.
 *
 * \todo Fix doc.
 */

# include <mln/border/adjust.hh>
# include <mln/extension/fill.hh>
# include <mln/geom/delta.hh>


namespace mln
{

  namespace extension
  {

    /*! Fill the domain extension of image \p ima with the
     *  single value \p v.
     *
     * \param[in,out] ima The image whose domain extension is to be filled.
     * \param[in] val The value to assign.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Optimize with memset if possible.
     */
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

    namespace impl
    {

      template <typename I, typename V>
      void do_adjust_fill(const I& ima,
			  unsigned delta,
			  const V& val)
      {
	mln_precondition(exact(ima).has_data());
	// mln_precondition(exact(win_like).is_valid());

	border::adjust(ima, delta);
	extension::fill(ima, val);
      }

      template <typename I, typename W, typename V>
      void do_adjust_fill(const I& ima,
			  const W& win_like,
			  const V& val)
      {
	mln_precondition(exact(ima).has_data());
	// mln_precondition(exact(win_like).is_valid());

	border::adjust(ima, geom::delta(win_like));
	extension::fill(ima, val);
      }

    } // end of namespace mln::extension::impl


    // Facades.

    template <typename I, typename W>
    void adjust_fill(const Image<I>& ima,
		     const Window<W>& win,
		     const mln_value(I)& val)
    {
      trace::entering("extension::adjust_fill");
      impl::do_adjust_fill(ima, win, val);
      trace::exiting("extension::adjust_fill");
    }

    template <typename I, typename W>
    void adjust_fill(const Image<I>& ima,
		     const Weighted_Window<W>& wwin,
		     const mln_value(I)& val)
    {
      trace::entering("extension::adjust_fill");
      impl::do_adjust_fill(ima, wwin, val);
      trace::exiting("extension::adjust_fill");
    }

    template <typename I, typename N>
    void adjust_fill(const Image<I>& ima,
		     const Neighborhood<N>& nbh,
		     const mln_value(I)& val)
    {
      trace::entering("extension::adjust_fill");
      impl::do_adjust_fill(ima, nbh, val);
      trace::exiting("extension::adjust_fill");
    }

    template <typename I>
    void adjust_fill(const Image<I>& ima,
		     unsigned delta,
		     const mln_value(I)& val)
    {
      trace::entering("extension::adjust_fill");
      impl::do_adjust_fill(ima, delta, val);
      trace::exiting("extension::adjust_fill");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::extension

} // end of namespace mln


#endif // ! MLN_CORE_EXTENSION_FILL_HH
