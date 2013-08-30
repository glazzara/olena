// Copyright (C) 2007, 2013 EPITA Research and Development Laboratory
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

    /*! Save and show an image \p input_, which displays whith \p cmd
     *  viewer in \p time seconds.
     *
     * \param[in] input_ the image to show.
     * \param[in] cmd The string which contains the programm of the
     * viewer which the user want to display with. By default its
     * value is "display".
     * \param[in] time The number of second of
     * display, 0 display permanently. By default the value is 0.
     *
     * \warning This routine doesn't erase the temporary created file.
     *
     */
    template <typename I>
    void
    save_and_show(const Image<I>& input_, std::string cmd = "display", int time = 0);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      template <typename I>
      inline
      void
      save_and_show_(const Image<I>& input,
		     std::string cmd, int time)
      {
	mln_trace("display::impl::save_and_show_");

	display::save(input);
 	show(input, cmd, time);
      }

    } // end of namespace mln::display::impl

    /// Facade.
    template <typename I>
    inline
    void
    save_and_show(const Image<I>& input_, std::string cmd = "display", int time = 0)
    {
      mln_trace("display::save_and_show");

      impl::save_and_show_(input_, cmd, time);
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::display

} // end of namespace mln


#endif //  ! MLN_DISPLAY_SAVE_AND_SHOW_HH
