// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_MAKE_DEBUG_SAVE_LABEL_IMAGE_HH
# define SCRIBO_MAKE_DEBUG_SAVE_LABEL_IMAGE_HH

/// \file
///
/// \brief Save a labeled image in a color image.

# include <mln/core/concept/image.hh>
# include <mln/labeling/colorize.hh>
# include <mln/value/rgb8.hh>
# include <mln/io/ppm/save.hh>

namespace scribo
{

  namespace debug
  {

    using namespace mln;

    /// \brief Save a labeled image in a color image.
    ///
    /// \param[in] lbl A label image.
    /// \param[in] nlabels The number of labels.
    /// \param[in] filename The output file name.
    ///
    /// \ingroup grpalgodebug
    //
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
      mln_trace("scribo::debug::save_label_image");
      mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
      mln_precondition(exact(lbl).is_valid());

      io::ppm::save(labeling::colorize(value::rgb8(), lbl, nlabels), filename);

    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_MAKE_DEBUG_SAVE_LABEL_IMAGE_HH
