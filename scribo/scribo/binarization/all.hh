// Copyright (C) 2010, 2011, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef SCRIBO_BINARIZATION_ALL_HH
# define SCRIBO_BINARIZATION_ALL_HH

/// \file
///
/// Include all headers located in scribo/binarization.

namespace scribo
{

  /// Namespace of binarization routines.
  namespace binarization
  {

  } // end of namespace scribo::binarization

} // end of namespace scribo


# include <scribo/binarization/global_threshold.hh>
# include <scribo/binarization/global_threshold_auto.hh>

# include <scribo/binarization/kim.hh>

# include <scribo/binarization/local_threshold.hh>

# include <scribo/binarization/niblack.hh>
# include <scribo/binarization/niblack_fast.hh>
# include <scribo/binarization/niblack_threshold.hh>

# include <scribo/binarization/otsu.hh>
# include <scribo/binarization/otsu_threshold.hh>

# include <scribo/binarization/sauvola.hh>
# include <scribo/binarization/sauvola_ms.hh>
# include <scribo/binarization/sauvola_ms_split.hh>
# include <scribo/binarization/sauvola_threshold.hh>

# include <scribo/binarization/singh.hh>

# include <scribo/binarization/wolf.hh>
# include <scribo/binarization/wolf_fast.hh>

#endif // ! SCRIBO_BINARIZATION_ALL_HH
