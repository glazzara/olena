// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_DISPLAY_SAVE_HH
# define MLN_DISPLAY_SAVE_HH

/// \file mln/display/save.hh
///
/// Definition of function which saves an image on a temporary
/// files.

# include <mln/trait/image_from_grid.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/data/fill.hh>
# include <mln/data/paste.hh>
# include <mln/display/color_pretty.hh>
# include <mln/io/ppm/save.hh>

namespace mln
{

  namespace display
  {

    /// Map which stocks the location of the saved file by the id of
    /// the image.
    extern std::map<void*, std::string> map_saved_image_tmp_;

    /// Save an image in a temporary file in ppm format.
    ///
    /// \param[in] input_ the image to save.
    template <typename I>
    void
    save(const Image<I>& input_);

# ifndef MLN_INCLUDE_ONLY

    std::map<void*, std::string> map_saved_image_tmp_;

    namespace impl
    {
      template <typename I>
      inline
      void
      save(trait::value::kind::any, const Image<I>& input_)
      {
	trace::entering("display::impl::save");

	const I& input = exact (input_);
	image2d<value::rgb8> out = display::color_pretty(input);

	/// Use of mkstemp instead tempmap.
	char *tmp = (char*)malloc (12 * sizeof (char));
	strcpy(tmp, "/tmp/XXXXXX");
	if (mkstemp(tmp) == -1)
	  return;
	std::string path_tmp = tmp;

	io::ppm::save(out, path_tmp);

	map_saved_image_tmp_[(void*)input.id_ ()] = path_tmp;

	trace::exiting("display::impl::save");
      }

      template <typename I>
      inline
      void
      save(trait::value::kind::color, const Image<I>& input_)
      {
	trace::entering("display::impl::save");

	const I& input = exact (input_);

	/// Use of mkstemp instead tempmap.
	char *tmp = (char*)malloc (12 * sizeof (char));
	strcpy(tmp, "/tmp/XXXXXX");
	if (mkstemp(tmp) == -1)
	  return;
	std::string path_tmp = tmp;

	io::ppm::save(input, path_tmp);

	map_saved_image_tmp_[(void*)input.id_ ()] = path_tmp;

	trace::exiting("display::impl::save");
      }

    } // end of namespace mln::display::impl

    /// Facade.
    template <typename I>
    inline
    void
    save(const Image<I>& input_)
    {
      trace::entering("display::save");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      impl::save(mln_trait_value_kind(mln_value(I)) (), input);

      trace::exiting("display::save");
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::display

} // end of namespace mln


#endif //  ! MLN_DISPLAY_SAVE_HH
