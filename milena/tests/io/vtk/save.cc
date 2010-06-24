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

#include <mln/io/off/load.hh>
#include <mln/io/vtk/save.hh>

#include <mln/literal/colors.hh>

#include "tests/data.hh"


template <typename I>
inline
I
make_image(const mln::bin_2complex_image3df& bin_ima,
	   const std::vector<mln_value(I)>& values)
{
  I ima;
  mln::initialize(ima, bin_ima);
  mln_piter(I) p(ima.domain());
  unsigned i = 0;
  for_all(p)
    ima(p) = values[i++ % values.size()];
  return ima;
}


int
main()
{
  using namespace mln;

  // Boolean values.
  typedef bin_2complex_image3df bin_ima_t;
  bin_ima_t bin_ima;
  /* FIXME: It would be better not to depend on the OFF file loader to
     create the complex-based image, to be saved as a VTK file; build
     this image by hand instead?  */
  io::off::load(bin_ima, MLN_MESH_DIR "/tetrahedron.off");

  io::vtk::save(bin_ima, "save-tetrahedron-bool.vtk");

  unsigned nfaces = bin_ima.domain().cplx().nfaces();

  // `int_u8' values.
  {
    std::vector<value::int_u8> values(nfaces);
    for (unsigned i = 0; i < nfaces; ++i)
      values[i] = mln_max(value::int_u8) * i / nfaces;
    io::vtk::save(make_image<int_u8_2complex_image3df>(bin_ima, values),
		  "save-tetrahedron-int_u8.vtk");
  }

  // Floating-point values.
  {
    std::vector<float> values(nfaces);
    for (unsigned i = 0; i < nfaces; ++i)
      values[i] = mln_max(float) / nfaces * i;
    io::vtk::save(make_image<float_2complex_image3df>(bin_ima, values),
		  "save-tetrahedron-float.vtk");
  }

  // `rgb8' values.
  {
    value::rgb8 colors_array[] =
      { literal::red, literal::green, literal::blue, literal::brown,
	literal::lime, literal::orange, literal::pink, literal::purple,
	literal::teal, literal::violet, literal::cyan, literal::magenta,
	literal::yellow, literal::olive };
    const unsigned colors_size = sizeof(colors_array) / sizeof(value::rgb8);
    std::vector<value::rgb8> colors (colors_array, colors_array + colors_size);
    io::vtk::save(make_image<rgb8_2complex_image3df>(bin_ima, colors),
		  "save-tetrahedron-rgb8.vtk");
  }
}
