//Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)

// This file is part of Olena.

// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.

// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/tree/components.hh>
#include <mln/morpho/tree/propagate.hh>
#include <mln/morpho/tree/propagate_representative.hh>

#include <mln/accu/stat/mean.hh>

#include <mln/data/sort_psites.hh>
#include <mln/data/convert.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/math/diff_abs.hh>

#include "color_distance.hh"
#include <iostream>
#include <cstring>

using namespace mln;
using value::int_u8;


void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " in:input.pgm in:source(.pgm|.ppm) [out:attribute.pbm]"
	    << std::endl
	    << "Compute the mean attribute image from 'source' values using 'input'"
    " min tree structure. Then, compute the delta image d (parent(n) - n) and retrieve"
    " components that maximizes the delta. The retrieved components are stored through a mask"
    " where objects are set to 1 and background to 0 in 'attribute.pbm'."
	    << std::endl;

    abort();
}

const char* get_extension(const char* filename)
{
  return (filename + strlen(filename) - 3);
}


template <typename T, typename S>
inline
mln_ch_value(typename T::function, int_u8)
compute_delta_mean(const value::int_u8&, const T& tree, const Image<S>& source)
{

  typedef mln_ch_value(typename T::function, mln_sum(mln_value(S))) O;
  O out = morpho::tree::compute_attribute_image_from(accu::stat::mean<mln_value(S)>(),
						     tree, source);

  io::pgm::save(data::convert(value::int_u8(), out), "mean.pgm");

  // Compute delta image.
  mln_ch_value(typename T::function, int_u8) dist;
  initialize(dist, tree.f());

  mln_up_node_piter(T) n(tree);
  for_all(n)
  {
    mln_assertion(tree.f(tree.parent(n)) > tree.f(n));
    dist(n) = math::diff_abs(out(tree.parent(n)), out(n));
  }

  return dist;
}

template <typename T, typename S>
inline
mln_ch_value(typename T::function, int_u8)
compute_delta_mean(const value::rgb8&, const T& tree, const Image<S>& source)
{

  typedef mln_ch_value(typename T::function, mln_sum(mln_value(S))) O;
  O out = morpho::tree::compute_attribute_image_from(accu::stat::mean<mln_value(S)>(),
						     tree, source);

  io::ppm::save(data::convert(value::rgb8(), out), "mean.ppm");

  // Compute delta image.
  mln_ch_value(typename T::function, int_u8) dist;
  initialize(dist, tree.f());

  mln_up_node_piter(T) n(tree);
  for_all(n)
  {
    // Fixme: rewrite distance routines with vector instead of convert ?
    dist(n) = dist_mean(convert::to<value::rgb8>(out(tree.parent(n))), convert::to<value::rgb8>(out(n)));
  }

  return dist;
}

template <typename T, typename S>
inline
mln_ch_value(typename T::function, int_u8)
compute_delta_mean(const T& tree, const Image<S>& source)
{
  return compute_delta_mean(mln_value(S) (), tree, source);
}



int main(int argc, char** argv)
{


  if (argc < 3)
    usage(argv);

  const char* finput = argv[1];
  const char* fsource = argv[2];
  const char* foutput = argc > 3 ? argv[3] : "mask.pbm";

  // Image loadin'.
  typedef image2d<value::int_u8> I;
  I input;
  io::pgm::load(input, finput);


  // Tree construction.
  typedef p_array<mln_psite_(I)> S;
  typedef morpho::tree::data<I, S> T;

  S s = data::sort_psites_decreasing(input);
  T tree(input, s, c4());

  // Attribute computation.
  mln_ch_value_(I, int_u8) delta;

  const char* extension = get_extension(fsource);
  if (strcmp(extension, "pgm") == 0)
    {
      image2d<value::int_u8> src;
      io::pgm::load(src, fsource);
      delta = compute_delta_mean(tree, src);
    }
  else if (strcmp(extension, "ppm") == 0)
    {
      image2d<value::rgb8> src;
      io::ppm::load(src, fsource);
      delta = compute_delta_mean(tree, src);
    }
  else
    usage(argv); // Type not handled.

  morpho::tree::propagate_representative(tree, delta);
  io::pgm::save(delta, "distance.pgm");

  // Get the max components of the delta image.
  p_array<mln_psite_(I)> obj = morpho::tree::get_components(tree, delta);

  typedef mln_ch_value_(I, bool) O;
  O out = morpho::tree::set_value_to_components(tree, obj, true, false);

  io::pbm::save(out, foutput);
}
