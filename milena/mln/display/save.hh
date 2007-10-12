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

#ifndef MLN_DISPLAY_SAVE_HH
# define MLN_DISPLAY_SAVE_HH

/*! \file mln/display/save.hh
 *
 * \brief Create a pretty image, which display its content
 *  with red value for non-defined point site.
 */

# include <mln/trait/image_from_mesh.hh>
# include <mln/core/image_if_value.hh>
# include <mln/core/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/level/fill.hh>
# include <mln/level/paste.hh>
# include <mln/display/color_pretty.hh>
# include <mln/io/ppm/save.hh>

namespace mln
{

  namespace display
  {

    std::map<void*, std::string> map_saved_image_tmp_;

    template <typename I>
    void
    save(const Image<I>& input_);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
    template <typename I>
    void
    save(const Image<I>& input_)
    {
      const I& input = exact (input_);
      image2d<value::rgb8> out = display::color_pretty(input);
      
      /// Use of mkstemp instead tempmap.
      char *tmp = (char*)malloc (12 * sizeof (char));
      strcpy(tmp, "/tmp/XXXXXX");
      if (mkstemp(tmp) == -1)
	return;
      std::string path_tmp = tmp;
      
      io::ppm::save(out, path_tmp);
      std::cout << input.id_ () << " = " << path_tmp << std::endl;
      map_saved_image_tmp_[(void*)input.id_ ()] = path_tmp;
    }

    } // end of namespace mln::display::impl

    /// Facade.
    template <typename I>
    void
    save(const Image<I>& input_)
    {
      return impl::save(input_);
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::display

} // end of namespace mln


#endif //  ! MLN_DISPLAY_SAVE_HH
