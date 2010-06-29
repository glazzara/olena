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

#ifndef SCRIBO_DEMO_SHARED_SRC_TO_MLN_IMAGE_HH
# define SCRIBO_DEMO_SHARED_SRC_TO_MLN_IMAGE_HH

# include <QImage>
# include <QPoint>

# include <mln/core/image/image2d.hh>
# include <mln/value/qt/rgb32.hh>
# include <mln/make/box2d.hh>

namespace scribo
{

  namespace demo
  {

    namespace shared
    {
      using namespace mln;

      image2d<value::qt::rgb32>
      to_mln_image(const QImage& ima, const QPoint& origin)
      {
	trace::entering("scribo::demo::shared");

	mln_assertion(!ima.isNull());

	image2d<value::qt::rgb32>
	  output(make::box2d(origin.y(),
			     origin.x(),
			     origin.y() + ima.height() - 1,
			     origin.x() + ima.width() - 1), 0);

	memcpy(output.buffer(), ima.bits(), ima.numBytes());

	mln_assertion(output.nrows() == (unsigned) ima.height());
	mln_assertion(output.ncols() == (unsigned) ima.width());

	trace::exiting("scribo::demo::shared");
	return output;
      }



    } // end of namespace scribo::demo::shared

  } // end of namespace scribo::demo

} // end of namespace scribo


#endif // ! SCRIBO_DEMO_SHARED_SRC_TO_MLN_IMAGE_HH
