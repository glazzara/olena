// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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
/// \brief A program computing a binary skeleton of a 3D image,
/// preserving end points.

#include <fstream>

#include <mln/core/image/image3d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/make/box3d.hh>

#include <mln/core/image/vmorph/fun_image.hh>
#include <mln/fun/v2b/lnot.hh>

#include <mln/border/fill.hh>

#include <mln/topo/skeleton/breadth_first_thinning.hh>

#include <mln/labeling/blobs.hh>

#include <mln/topo/is_simple_point3d.hh>
#include <mln/topo/detach_point.hh>
#include <mln/topo/is_not_end_point.hh>

#include <mln/math/sqr.hh>
#include <mln/math/sqrt.hh>

#include "apps/data.hh"


// FIXME: Have this program take a 3D image as input (instead of
// hard-coding it here).

// FIXME: Do not use a dedicated I/O routines.  Either convert the data
// to another format, or move this routine into mln/io.

mln::image3d<bool>
load_pgm_3d(const std::string& filename)
{
  std::ifstream file(filename.c_str());

  std::string magic;
  std::getline(file, magic);
  mln_assertion(magic == "P5");

  const size_t ncomments = 3;
  std::string comment[ncomments];
  std::string expected[ncomments] = { "#xdim 1", "#ydim 1", "#zdim 1" };
  // Avoid a warning when NDEBUG is defined.
  (void) expected;
  for (size_t i = 0; i < ncomments; ++i)
    {
      std::getline(file, comment[i]);
      mln_assertion(comment[i] == expected[i]);
    }

  // FIXME: What is the right order?  Does the number of slices
  // appears first or last?
  int nslis, nrows, ncols;
  file >> nslis >> nrows >> ncols;

  // FIXME: We just ignore max_val.
  unsigned max_val;
  file >> max_val;

  mln::image3d<bool> ima(nslis, nrows, ncols);
  char val;
  mln::point3d p;
  for (p.sli() = 0; p.sli() < nslis; ++p.sli())
    for (p.row() = 0; p.row() < nrows; ++p.row())
      for (p.col() = 0; p.col() < ncols; ++p.col())
	{
	  file.read(&val, 1);
	  ima(p) = val;
	}
  return ima;
}

void
save_raw_3d(const mln::image3d<bool>& ima, const std::string& filename)
{
  std::ofstream file(filename.c_str());
  mln_fwd_piter_(mln::image3d<bool>) p(ima.domain());
  for_all(p)
    file << static_cast<char>(ima(p) ? 0xff : 0x00);
}

// FIXME: Again, this is a dedicated routine.  Use something from
// subsample.
mln::image3d<bool>
subsampling_3d(const mln::image3d<bool>& input, unsigned factor)
{
  using namespace mln;
  typedef image3d<bool> I;
  /* This subsampling procedure may not tak into account pixels at the
     end of each sloce/row/column if the corresponding dimension is
     not a multiple of FACTOR.  */
  I output(input.nslices() / factor,
	   input.nrows()    / factor,
	   input.ncols()   / factor);
  mln_piter_(I) po(output.domain());
  for_all(po)
  {
    // Bounds of the browsed box.
    point3d pi_min(po.sli() * factor,
		   po.row() * factor, 
		   po.col() * factor);
    point3d pi_max((po.sli() + 1) * factor - 1,
		   (po.row() + 1) * factor - 1,
		   (po.col() + 1) * factor - 1);
    box3d sample_box(pi_min, pi_max);
    size_t sample_size = sample_box.nsites();

    // Count the number of `true' and `false' values and set OUTPUT(P)
    // to the prevalent value.
    unsigned ntrue_vals = 0;
    unsigned nfalse_vals = 0;
    mln_piter_(box3d) pi(sample_box);
    for_all(pi)
    {
      if (input(pi))
	++ntrue_vals;
      else
	++nfalse_vals;
      // Optimization: if one of the values (`true' or `false' has
      // absolute majority, stop here).
      if (   ntrue_vals  > sample_size / 2
	  || nfalse_vals > sample_size / 2)
	break;
    }
    output(po) = (ntrue_vals >= nfalse_vals);
  }
  return output;
}

/// \brief Draw a torus on a binary 3D image.
///
/// \param ima         The image to write into.
/// \param axis_dim    The dimentsion corresponding to the axis of
///                    the torus.
/// \param int_radius  Internal radius of the torus.
/// \param ext_radius  External radius of the torus.
void
draw_torus(mln::image3d<bool>& ima, unsigned axis_dim,
	   unsigned int_radius, unsigned ext_radius)
{
  using namespace mln;

  // Check the axis dimension.
  mln_precondition(0 <= axis_dim && axis_dim < 3);
  // Dimensions other than the one of the axis.
  unsigned dim1 = (axis_dim + 1) % 3;
  unsigned dim2 = (axis_dim + 2) % 3;

  // Mid radius.
  const unsigned mid_radius = (ext_radius + int_radius) / 2;
  // Radius of the section of the torus.
  const unsigned torus_radius = (ext_radius - int_radius) / 2 ;

  point3d bb_min;
  bb_min[axis_dim] = -torus_radius;
  bb_min[dim1]     = -ext_radius;
  bb_min[dim2]     = -ext_radius;

  point3d bb_max;
  bb_max[axis_dim] = torus_radius;
  bb_max[dim1]     = ext_radius;
  bb_max[dim2]     = ext_radius;

  // Ensure the box of IMA is large enough.
  mln_assertion(ima.has(bb_min));
  mln_assertion(ima.has(bb_max));

  box3d bb(bb_min, bb_max);
  mln_piter_(box3d) p(bb);
  for_all(p)
  {
    unsigned x = math::sqrt((math::sqr(torus_radius) - math::sqr(p[axis_dim])));
    unsigned lo_radius = mid_radius - x;
    unsigned hi_radius = mid_radius + x;
    unsigned r2 = math::sqr(p[dim1]) + math::sqr(p[dim2]);
    if (math::sqr(lo_radius) < r2 && r2 < math::sqr(hi_radius))
      ima(p) = true;
  }
}


int main()
{
  using namespace mln;

  typedef image3d<bool> I;
  typedef neighb3d N;

  // Add a border of (at least) 1 pixel, to a guarantee a meaningful
  // result of the computation/retrieval of connectivity numbers
  // (performed within is_simple_3d).  Indeed the LUT-based (fast)
  // version of this step expects each pixel to have 26 neighboring
  // sites.
  border::thickness = 1;

//   I whole_input = load_pgm_3d(MLN_TESTS_IMG_DIR "/bunny.pgm");
//   // FIXME: Work on a small image in a first time, then process the
//   // whole image.
//   I input = subsampling_3d(whole_input, 4);

  // Input image.
  def::coord half_len = 100;
  box3d b = make::box3d(-half_len, -half_len, -half_len,
			+half_len, +half_len, +half_len);
  I input(b);
  data::fill(input, false);
  border::fill(input, false);
  def::coord ext_radius = half_len;
  def::coord int_radius = ext_radius * 0.7;
  draw_torus(input, 0, int_radius, ext_radius);
  draw_torus(input, 1, int_radius, ext_radius);
  draw_torus(input, 2, int_radius, ext_radius);

  std::cerr << input.domain() << std::endl;
  save_raw_3d(input, "image3d-skel-with-end-points-input.raw");

  // FIXME: Debug.
  unsigned n_fg_comps;
  labeling::blobs(input, c26(), n_fg_comps);
  unsigned n_bg_comps;
  labeling::blobs((fun::v2b::lnot<bool>() << input), c6(), n_bg_comps);
  std::cerr << "n_fg_comps = " << n_fg_comps << std::endl;
  std::cerr << "n_bg_comps = " << n_bg_comps << std::endl;

  // FIXME: Use a dual neighborhood instead?

  // Foreground neighborhood.
  N nbh_fg = c26();
  // Background neighborhood.
  N nbh_bg = c6();

  // Simplicity criterion functor.
  //
  // Note that when it is available, the topo::is_simple_point3d<I, N>
  // functor based on a precomputed look-up table (LUT) is much faster
  // than topo::is_simple_point3d<I, N>, though it increases compiling
  // times and executable size (because of the 8MiB LUT).
  topo::is_simple_point3d<I, N> is_simple(nbh_fg, nbh_bg);
  // Simple point detach procedure.
  topo::detach_point<I> detach;
  // Constraint: do not collapse end points.
  /* FIXME: This criterion is static, e.g. it is attached to INPUT
     (fixed), not OUTPUT (changing during the thinning).  This is not
     as good as a dynamic critertion (attached to OUTPUT).  Maybe it
     is time to introduce/use `breadth_first_thinning_inplace'.  */
  topo::is_not_end_point<I, N> constraint(c6(), input);

  I output = topo::skeleton::breadth_first_thinning(input, nbh_fg,
						    is_simple,
						    detach,
						    constraint);
  save_raw_3d(output, "image3d-skel-with-end-points-skel.raw");
}
