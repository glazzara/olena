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

#ifndef MLN_LEVEL_REPLACE_HH
# define MLN_LEVEL_REPLACE_HH

/*! \file mln/level/replace.hh
 *
 * \brief Replace the contents of an image into another one.
 *
 */

# include <mln/core/concept/image.hh>
# include <mln/core/image/image_if.hh>

# include <mln/level/fill.hh>
# include <mln/pw/value.hh>
# include <mln/pw/cst.hh>

namespace mln
{

  namespace level
  {

    /*! Replace \p old_value by \p new_value in the image \p input
     *
     * \param[in] input The input image.
     * \param[in] old_value The value to be replaced...
     * \param[in] new_value ...by this one.
     *
     */
    template <typename I>
    void replace(Image<I>& input, const mln_value(I)& old_value,
		 const mln_value(I)& new_value);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	inline
	void replace_(Image<I>& input_, const mln_value(I)& old_value,
		      const mln_value(I)& new_value)
	{
	  trace::entering("level::impl::generic::replace");
	  level::fill((exact(input_) | pw::value(input_) == pw::cst(old_value)).rw(),  new_value);
	  trace::exiting("level::impl::generic::replace");
	}

      } // end of namespace mln::level::impl::generic

    } // end of namespace mln::level::impl


    // Facade.
    template <typename I>
    void replace(Image<I>& input, const mln_value(I)& old_value,
		 const mln_value(I)& new_value)
    {
      trace::entering("level::replace");

      mln_precondition(exact(input).has_data());

      impl::generic::replace_<I>(exact(input), old_value, new_value);
      trace::exiting("level::replace");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_REPLACE_HH
