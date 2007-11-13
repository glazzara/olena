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

#ifndef MLN_DISPLAY_SAVE_AND_SHOW_HH
# define MLN_DISPLAY_SAVE_AND_SHOW_HH

/*! \file mln/display/save_and_show.hh
 *
 * \brief Definition of function which saves and shows an image in the
 * same call.
 */

# include <mln/display/save.hh>
# include <mln/display/show.hh>

namespace mln
{

  namespace display
  {

    template <typename I>
    void
    save_and_show(const Image<I>& input_, std::string cmd);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      template <typename I>
      void
      save_and_show_(trait::value::kind::color,
		     const Image<I>& input, std::string cmd)
      {
	save_color(input);
	show(input, cmd);
      }

      template <typename I>
      void
      save_and_show_(trait::value::kind::any,
		     const Image<I>& input,
		     std::string cmd)
      {
	save(input);
 	show(input, cmd);
      }

    } // end of namespace mln::display::impl

    /// Facade.
    template <typename I>
    void
    save_and_show(const Image<I>& input_, std::string cmd)
    {
      impl::save_and_show_(mln_trait_value_kind(mln_value(I)) (),
			   input_, cmd);
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::display

} // end of namespace mln


#endif //  ! MLN_DISPLAY_SAVE_AND_SHOW_HH
