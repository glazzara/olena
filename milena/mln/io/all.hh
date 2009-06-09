// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_ALL_HH
# define MLN_IO_ALL_HH

/// \file
/// \brief Inclusion of all I/O routines.


namespace mln
{

  /// Namespace of input/output handling.
  namespace io
  {
    /// Internal namespace of io namespace.
    namespace internal {}
  }

}


/*-----------------------.
| Built-in I/O support.  |
`-----------------------*/

# include <mln/io/cloud/all.hh>
# include <mln/io/dump/all.hh>
# include <mln/io/pbm/all.hh>
# include <mln/io/pfm/all.hh>
# include <mln/io/pgm/all.hh>
# include <mln/io/plot/all.hh>
# include <mln/io/pnm/all.hh>
# include <mln/io/ppm/all.hh>
# include <mln/io/txt/all.hh>
# include <mln/io/off/all.hh>


/*--------------------------------------------------.
| I/O routines depending on a third-party library.  |
`--------------------------------------------------*/

# ifdef HAVE_GDCM
#  include <mln/io/dicom/all.hh>
# endif // ! HAVE_GDCM

# ifdef HAVE_CFITSIO
#  include <mln/io/fits/all.hh>
# endif // ! HAVE_CFITSIO

# ifdef HAVE_MAGICKXX
#  include <mln/io/magick/all.hh>
# endif // ! HAVE_MAGICKXX

# ifdef HAVE_TIFF
#  include <mln/io/tiff/all.hh>
# endif // ! HAVE_LIBTIFF

#endif // ! MLN_IO_ALL_HH
