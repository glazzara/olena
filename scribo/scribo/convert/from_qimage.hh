// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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



#ifndef SCRIBO_CONVERT_FROM_QIMAGE_HH
# define SCRIBO_CONVERT_FROM_QIMAGE_HH

/// \file
///
/// Extract text from a document.


# include <QtGui/QImage>

# include <mln/core/image/image2d.hh>
# include <mln/value/qt/rgb32.hh>

# if QT_VERSION < 0x040000
#  error "Qt library too old. You need at least Qt 4.x."
# endif // ! QT_VERSION


namespace scribo
{

  namespace convert
  {

    /*! \brief Convert a QImage to mln::image2d.

      \param[in] ima A QImage. Prefer using QImage::Format_RGB32
      image format to avoid conversions.

      \return A RGB8 2D image in Milena's format.
     */
    mln::image2d<mln::value::qt::rgb32>
    from_qimage(const QImage& ima);


# ifndef MLN_INCLUDE_ONLY

    mln::image2d<mln::value::qt::rgb32>
    from_qimage(const QImage& ima)
    {
      QImage tmp = ima;

      if (ima.format() != QImage::Format_RGB32)
	tmp = ima.convertToFormat(QImage::Format_RGB32);

      const int
	nrows = tmp.height(),
	ncols = tmp.width();

      mln::image2d<mln::value::qt::rgb32> output(nrows, ncols, 0);

      QImage qima(ncols, nrows, QImage::Format_RGB32);
      std::memcpy(output.buffer(),
		  tmp.scanLine(0),
		  output.nelements() * 4);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::convert

} // end of namespace scribo

#endif // ! SCRIBO_CONVERT_FROM_QIMAGE_HH
