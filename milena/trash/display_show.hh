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

#ifndef MLN_DISPLAY_SHOW_HH
# define MLN_DISPLAY_SHOW_HH

/*! \file mln/display/show.hh
 *
 * \brief Display the temporary image which has just been saved with
 * given viewer (default display).
 */

# include <mln/trait/image_from_grid.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/image/image2d.hh>
# include <mln/display/save.hh>

# include <map>

namespace mln
{

  namespace display
  {

    /*! Show an image \p input_ previously saved by display::save,
     *  which displays whith \p cmd viewer in \p time seconds.
     *
     * \param[in] input_ the image to show.
     * \param[in] cmd The string which contains the programm of the
     * viewer which the user want to display with. By default its
     * value is "display".
     * \param[in] time The number of second of
     * display, 0 display permanently. By default the value is 0.
     *
     * \pre the image \p input_ is previously saved is map_saved_image_tmp_.
     *
     */
    template <typename I>
    void
    show(const Image<I>& input_, std::string cmd = "display", int time = 0);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      bool
      check_saved(const Image<I>& input_)
      {
	const I& input = exact(input_);

	typename std::map<void*, std::string>::iterator it = map_saved_image_tmp_.begin();

	for (; it != map_saved_image_tmp_.end(); ++it)
	  if ((*it).first == input.id_())
	    return true;
	return false;
      }

      template <typename I>
      inline
      void
      show(const Image<I>& input_, std::string cmd, int time)
      {
	mln_trace("display::impl::show");

	const I& input = exact(input_);

	std::stringstream st;
	if (time)
	  {
	    st << cmd << " " << map_saved_image_tmp_[(void*)input.id_()] << " &";
	    system(st.str().c_str());
	    st.str("");
	    st << "sleep " << time << " && pkill " << cmd;
	    system(st.str().c_str());
	  }
	else
	  {
	    st << cmd << " " << map_saved_image_tmp_[(void*)input.id_()];
	    system(st.str().c_str());
	  }
      }

    } // end of namespace mln::display::impl

    /// Facade.
    template <typename I>
    inline
    void
    show(const Image<I>& input_, std::string cmd = "display", int time = 0)
    {
      mln_trace("display::show");

      mln_precondition(impl::check_saved(input_));
      impl::show(input_, cmd, time);
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::display

} // end of namespace mln


#endif //  ! MLN_DISPLAY_SHOW_HH
