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

#include <mln/math/max.hh>

#include <mln/math/diff_abs.hh>

#include "color_distance.hh"

#include <iostream>
#include <cstring>

// debug
#include <mln/morpho/tree/debug.hh>
#include <mln/morpho/tree/components_debug.hh>

using namespace mln;
using value::int_u8;

//template <typename T>
//struct mymean : public mln::accu::internal



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
  typedef accu::stat::mean<mln_value(S)> ACC;
  typedef mln_ch_value(typename T::function, ACC) ACC_IMG;

  ACC_IMG accus;
  O means = morpho::tree::compute_attribute_image_from(ACC (),
						       tree,
						       source,
						       &accus);

  io::ppm::save(data::convert(value::rgb8(), means), "mean.ppm");

  // Compute delta image.
  mln_ch_value(typename T::function, int_u8) dist, idist_dad, idist_bro;
  initialize(dist, tree.f());
  initialize(idist_dad, tree.f());
  initialize(idist_bro, tree.f());

  mln_up_node_piter(T) n(tree);
  for_all(n)
  {
    int_u8 dist_dad, dist_bro;
    value::rgb8 current, cur_mean, bro_mean, dad_mean;
    ACC aux;
    ACC aux2 = accus(tree.parent(n));

    // FIXME: add untake_n_times to accumlator concept
    // FIXME: to optimize
    {
      mln_piter(T::nodes_t) child(tree.children(tree.parent(n)));
      for_all(child)
	if (child != n)
	  aux.take(accus(child));
    }

    cur_mean = convert::to<value::rgb8>(means(n));

    bro_mean = convert::to<value::rgb8>(aux.to_result());
    dist_bro = aux.is_valid() ? dist_mean(bro_mean, cur_mean) : (int_u8) 0;
    idist_bro(n) = dist_bro;

    aux.take(accus(n));
    aux2.untake(aux);

    dad_mean = convert::to<value::rgb8>(aux2.to_result());
    dist_dad = dist_mean(dad_mean, cur_mean);
    idist_dad(n) = dist_dad;

    dist(n) = math::max(dist_bro, (int_u8) (dist_dad * 1.5));
  }

  morpho::tree::propagate_representative(tree, idist_bro);
  morpho::tree::propagate_representative(tree, idist_dad);
  io::pgm::save(idist_dad, "distance_dad.pgm");
  io::pgm::save(idist_bro, "distance_bro.pgm");

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

      // init debug
      global::debug_img = duplicate(src);
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
