// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
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

/// \file
///
/// Convert a Milena image to a Qimage.

#ifndef MLN_CONVERT_TO_QIMAGE_HH
# define MLN_CONVERT_TO_QIMAGE_HH

# include <QtGui/QImage>

# include <mln/core/concept/image.hh>
# include <mln/geom/nrows.hh>
# include <mln/geom/ncols.hh>
# include <mln/border/resize.hh>


# include <mln/value/qt/rgb32.hh>
# include <mln/value/rgb8.hh>

// FIXME: not generic.
# include <mln/core/alias/dpoint2d.hh>

# if QT_VERSION < 0x040000
#  error "Your version of Qt is too old and is not supported."
# endif


namespace mln
{

  namespace convert
  {

    /// \brief Convert a Milena image to a Qimage.
    ///
    //
    template <typename I>
    inline
    QImage
    to_qimage(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    // Implementation

    namespace impl
    {


      template <typename I>
      inline
      QImage to_qimage_scalar(const Image<I>& ima_)
      {
	const I& ima = exact(ima_);
	mln_precondition(ima.is_valid());

	const unsigned
	  nrows = geom::nrows(ima),
	  ncols = geom::ncols(ima);

# if QT_VERSION >= 0x040000 && QT_VERSION < 0x040400
	QImage qima(ncols, nrows, QImage::Format_RGB32);
	uchar * ptr_qima = qima.bits();
	unsigned padding = 0;
#  if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	++ptr_qima;
#  endif // ! Q_BYTE_ORDER
# else
	QImage qima(ncols, nrows, QImage::Format_RGB888);
	uchar * ptr_qima = qima.bits();
	unsigned padding = ncols % 4;
# endif // ! QT_VERSION

	const mln_value(I)* ptr_ima = &ima(ima.domain().pmin());
	unsigned offset = ima.delta_offset(dpoint2d(+1, - ncols));

	// Data is stored as ABGR so we skip the first value which is ignored.
	for (unsigned row = 0; row < nrows; ++row, ptr_ima += offset)
	{
	  for (unsigned col = 0; col < ncols; ++col)
	  {
	    const mln_value(I)& v = *ptr_ima++;
	    std::memset(ptr_qima, v, sizeof(mln_value(I)) * 3);

# if QT_VERSION >= 0x040000 && QT_VERSION < 0x040400
	    ptr_qima += 4;
# else
	    ptr_qima += 3;
# endif // ! QT_VERSION
	  }
	  ptr_qima += padding;
	}

	return qima;
      }



# if QT_VERSION >= 0x040000 && QT_VERSION < 0x040400

      template <typename I>
      inline
      QImage to_qimage_rgb8(const Image<I>& ima_)
      {
	const I& ima = exact(ima_);
	mln_precondition(ima.is_valid());

	const int
	  nrows = geom::nrows(ima),
	  ncols = geom::ncols(ima);

	QImage qima(ncols, nrows, QImage::Format_RGB32);
	uchar * ptr_qima = qima.scanLine(0);
	const mln_value(I)* ptr_ima = &ima(ima.domain().pmin());
	unsigned offset = ima.delta_offset(dpoint2d(+1, - ncols));

	for (unsigned row = 0; row < nrows; ++row, ptr_ima += offset)
	{
	  for (unsigned col = 0; col < ncols; ++col)
	  {
	    const mln::value::rgb8& v = *ptr_ima++;
#  if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	    // Memory representation : BBGGRRFF
	    *ptr_qima++ = v.blue();
	    *ptr_qima++ = v.green();
	    *ptr_qima = v.red();
	    ptr_qima += 2;
#  else /* Q_BIG_ENDIAN */
	    // Memory representation : FFRRGGBB
	    ++ptr_qima;
	    *ptr_qima++ = v.red();
	    *ptr_qima++ = v.green();
	    *ptr_qima = v.blue();
#  endif // ! Q_BYTE_ORDER
	  }
	}

	return qima;
      }

# else
      template <typename I>
      inline
      QImage to_qimage_rgb8(const Image<I>& ima_)
      {
	const I& ima = exact(ima_);
	mln_precondition(ima.is_valid());

	const int
	  nrows = geom::nrows(ima),
	  ncols = geom::ncols(ima);


	typedef mln_site(I) P;
	typedef mln_dpsite(P) DP;

	// Required by a one-shot data copy:
	mln::border::resize(ima, 0);

	unsigned
	  offset = ima.delta_offset(DP(+1, 0)),
	  line_offset = 0;

	QImage qima(ncols, nrows, QImage::Format_RGB888);
	for (int i = 0 ; i < qima.height(); ++i, line_offset += offset)
	  std::memcpy(qima.scanLine(i),
		      ima.buffer() + line_offset,
		      ima.ncols() * 3);

	return qima;
      }
# endif // ! QT_VERSION


      template <typename I>
      inline
      QImage to_qimage_qt_rgb32(const Image<I>& ima_)
      {
	const I& ima = exact(ima_);
	mln_precondition(ima.is_valid());

	const int
	  nrows = geom::nrows(ima),
	  ncols = geom::ncols(ima);


	// Required by a one-shot data copy:
	mln::border::resize(ima, 0);

	QImage qima(ncols, nrows, QImage::Format_RGB32);
	std::memcpy(qima.bits(), ima.buffer(), ima.nelements() * 4);

	return qima;
      }


    } // end of namespace mln::convert::impl



    // Dispatch

    namespace internal
    {

      template <typename I, typename S>
      inline
      QImage to_qimage_dispatch(const Image<I>& ima, const value::Scalar<S>&)
      {
	return impl::to_qimage_scalar(ima);
      }

      template <typename I>
      inline
      QImage to_qimage_dispatch(const Image<I>& ima, const value::rgb8&)
      {
	return impl::to_qimage_rgb8(ima);
      }

      template <typename I>
      inline
      QImage to_qimage_dispatch(const Image<I>& ima, const value::qt::rgb32&)
      {
	return impl::to_qimage_qt_rgb32(ima);
      }


      template <typename I, typename V>
      inline
      QImage to_qimage_dispatch(const Image<I>& ima, V&)
      {
	(void) ima;

	// Not supported yet!
	mlc_abort(I)::check();
	return QImage();
      }


      template <typename I>
      inline
      QImage to_qimage_dispatch(const Image<I>& ima)
      {
	typedef mln_value(I) V;
	return to_qimage_dispatch(ima, V());
      }

    } // end of namespace mln::convert::internal



    // Facade

    template <typename I>
    inline
    QImage to_qimage(const Image<I>& ima_)
    {
      mln_trace("convert::to_qimage");

      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      QImage output = internal::to_qimage_dispatch(ima);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln

#endif // ! MLN_CONVERT_TO_QIMAGE_HH
