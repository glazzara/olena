// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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


#ifndef SCRIBO_MAKE_DEBUG_SAVE_LABEL_IMAGE_HH
# define SCRIBO_MAKE_DEBUG_SAVE_LABEL_IMAGE_HH

/// \file scribo/debug/save_label_image.hh
///
/// Save a labeled image in a color image.

namespace scribo
{

  namespace debug
  {

    /// Save a labeled image in a color image.
    template <typename I>
    void
    save_label_image(const Image<I>& lbl, const mln_value(I)& nlabels,
		     const char *filename);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    void
    save_label_image(const Image<I>& lbl, const mln_value(I)& nlabels,
		     const char *filename)
    {
      trace::entering("scribo::debug::save_label_image");
      mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
      mln_precondition(exact(lbl).is_valid());

      io::ppm::save(debug::colorize(rgb8(), lbl, nlabels), filename);

      trace::exiting("scribo::debug::save_label_image");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_MAKE_DEBUG_SAVE_LABEL_IMAGE_HH
