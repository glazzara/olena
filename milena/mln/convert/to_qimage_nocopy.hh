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

/// \file
///
/// Convert a Milena image to a Qimage.


#ifndef MLN_CONVERT_TO_QIMAGE_NOCOPY_HH
# define MLN_CONVERT_TO_QIMAGE_NOCOPY_HH

# include <QtGui/QImage>

# include <mln/core/concept/image.hh>
# include <mln/geom/nrows.hh>
# include <mln/geom/ncols.hh>
# include <mln/border/resize.hh>


# include <mln/value/qt/rgb32.hh>
# include <mln/value/rgb8.hh>

# if QT_VERSION < 0x040000
#  error "Your version of Qt is too old and is not supported."
# endif


namespace mln
{

  namespace convert
  {

    // Implementation

    namespace implementation
    {

      template <typename I>
      inline
      QImage to_qimage_nocopy_qt_rgb32(const Image<I>& ima_)
      {
	const I& ima = exact(ima_);
	mln_precondition(ima.is_valid());

	const int
	  nrows = geom::nrows(ima),
	  ncols = geom::ncols(ima);

	mln::border::resize(ima, 0);

	QImage qima((uchar *)(ima.buffer()), ncols, nrows,
		    QImage::Format_RGB32);

	return qima;
      }


    } // end of namespace mln::convert::implementation



    // Dispatch

    namespace internal
    {

      template <typename I>
      inline
      QImage to_qimage_nocopy_dispatch(const Image<I>& ima, const value::qt::rgb32&)
      {
	return implementation::to_qimage_nocopy_qt_rgb32(ima);
      }


      template <typename I, typename V>
      inline
      QImage to_qimage_nocopy_dispatch(const Image<I>& ima, V&)
      {
	// Not supported yet!
	mlc_abort(I)::check();
	return QImage();
      }


      template <typename I>
      inline
      QImage to_qimage_nocopy_dispatch(const Image<I>& ima)
      {
	typedef mln_value(I) V;
	return to_qimage_nocopy_dispatch(ima, V());
      }

    } // end of namespace mln::convert::internal



    // Facade

    template <typename I>
    inline
    QImage to_qimage_nocopy(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      QImage output = internal::to_qimage_nocopy_dispatch(ima);

      return output;
    }


  } // end of namespace mln::convert

} // end of namespace mln

#endif // ! MLN_CONVERT_TO_QIMAGE_NOCOPY_HH
