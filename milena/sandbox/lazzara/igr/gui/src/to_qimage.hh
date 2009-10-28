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


#ifndef MLN_DEMO_SRC_TO_QIMAGE_HH
# define MLN_DEMO_SRC_TO_QIMAGE_HH

# include <QtGui/QImage>

# include <mln/core/concept/image.hh>
# include <mln/geom/nrows.hh>
# include <mln/geom/ncols.hh>
# include <mln/border/resize.hh>

namespace mln
{

  template <typename I>
  inline
  QImage to_qimage(const Image<I>& ima_)
  {
    const I& ima = exact(ima_);
    mln_precondition(ima.is_valid());

    const int
      nrows = geom::nrows(ima),
      ncols = geom::ncols(ima);

    // Required by a one-shot data copy:
    mln::border::resize(ima, 0);

    QImage qima(ncols, nrows, QImage::Format_RGB888);
    std::memcpy(qima.scanLine(0),
		ima.buffer(),
		ima.nelements() * 3);
    return qima;
  }

} // end of namespace mln

#endif // ! MLN_DEMO_SRC_TO_QIMAGE_HH
