// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_BINARIZATION_INTERNAL_LOCAL_THRESHOLD_DEBUG_HH
# define SCRIBO_BINARIZATION_INTERNAL_LOCAL_THRESHOLD_DEBUG_HH

/// \file
///
/// \brief Declare all debug related variables for local based
/// algorithms.


/// FIXME: A struct may be a bit better...


# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG

#  ifndef MLN_INCLUDE_ONLY


namespace scribo
{

  namespace binarization
  {

    using namespace mln;

    namespace internal
    {

      const char* stddev_image_output = 0;
      const char* mean_image_output = 0;
      const char* threshold_image_output = 0;

      const char* scale_image_output = 0;
      const char* scale_iz_image_output = 0;

      const char* alpham_image_output = 0;
      const char* alphacond_image_output = 0;

      // Declare debug images.
      image2d<double> debug_stddev;
      image2d<double> debug_mean;
      image2d<double> debug_threshold;

      image2d<double> debug_alpham;
      image2d<bool> debug_alphacond;

      double mean_debug_factor = 1.0;
      double stddev_debug_factor = 1.0;
      double alpham_debug_factor = 2.0;

    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo


#  endif // ! MLN_INCLUDE_ONLY

# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG


#endif // ! SCRIBO_BINARIZATION_INTERNAL_LOCAL_THRESHOLD_DEBUG_HH
