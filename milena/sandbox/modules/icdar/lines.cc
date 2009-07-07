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


// For local::superimpose_save.
#include <string>
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label.hh>
#include <mln/data/convert.hh>
#include <mln/data/fill.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/literal/colors.hh>
#include <mln/io/ppm/save.hh>

// For local::accu_compute_on_weighted_sets.
#include <mln/util/array.hh>

// For main.
#include <mln/io/pbm/load.hh>
#include <mln/world/binary_2d/subsample.hh>
#include <mln/io/pgm/save.hh>

#include <mln/data/fill.hh>
#define MLN_FLOAT double
#include <sandbox/theo/exec/gaussian_directional_2d.hh>
#include <mln/data/saturate.hh>

#include <mln/win/rectangle2d.hh>
#include <mln/morpho/closing/structural.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/algebra/vec.hh>
#include <mln/accu/stat/var.hh>
#include <mln/make/region_adjacency_graph.hh>
#include <mln/make/vertex_image.hh>
#include <mln/make/edge_image.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.ppm" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:  input 2D binary image (text is black; background is white)" << std::endl
	    << "  output.ppm: result of line segmentation." << std::endl;
  std::abort();
}



namespace local
{

  using namespace mln;


  // Superpose the watershed line (red) over an input image.
  void superimpose_save(const image2d<value::int_u8>& input,
			const image2d<value::label<12> >& ws,
			const std::string& filename)
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);
    data::fill((output | (pw::value(ws) == pw::cst(0))).rw(),
	       literal::red);
    io::ppm::save(output, filename);
  }

  
  template <typename A, typename L, typename I>
  util::array<A>
  accu_compute_on_weighted_sets(A,
				const L& label,
				mln_value(L) nlabels,
				const I& w)
  {
    util::array<A> arr(nlabels.next());
    mln_piter(L) p(label.domain());
    for_all(p)
      arr[label(p)].take_n_times(w(p), p.to_site());
    return arr;
  }


  struct bloup : Function_vv2v<bloup>
  {
    typedef float result;

    template <typename Var, typename V>
    float dist(const Var& var, const V& v) const
    {
      mln_precondition(var.is_valid());
      V v_ = v - var.mean();
      return v_.t() * var.variance()._1() * v_;
    }

    template <typename Var>
    float operator()(const Var& var1, const Var& var2) const
    {
      return std::min(dist(var1, var2.mean()), dist(var2, var1.mean()));
    }
  };


} // local




int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;


  if (argc != 3)
    usage(argv);


  // Parameters.

  const unsigned
    height = 5,
    width = 25;

  const float
    h_sigma = 31,
    v_sigma = 1.3;

  // end of Parameters.


  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  // Sub-sample.
  image2d<value::int_u8> small = world::binary_2d::subsample(input, 4);
//   io::pgm::save(small, "temp_small.pgm");


  
  // Fuzzifying.

  image2d<double> temp(small.domain());
  data::fill(temp, small);

  temp = linear::gaussian_directional_2d(temp, 1, h_sigma, 255);
  temp = linear::gaussian_directional_2d(temp, 0, v_sigma, 255);
  
  image2d<int_u8> fuzzy = data::saturate(value::int_u8(), temp);
//   io::pgm::save(fuzzy, "temp_fuzzy.pgm");

  // Closing.
  image2d<int_u8> clo = morpho::closing::structural(fuzzy, win::rectangle2d(height, width));

  // Watershed transform.
  typedef value::label<12> L;
  L n_basins;
  image2d<L> ws = morpho::watershed::flooding(clo, c4(), n_basins);

  std::cout << "n basins = " << n_basins << std::endl;

  local::superimpose_save(small, ws, "temp_small_ws.ppm");
  local::superimpose_save(fuzzy, ws, "temp_fuzzy_ws.ppm");


  typedef accu::stat::var< algebra::vec<2,float> > A;
  util::array<A> arr;
  arr = local::accu_compute_on_weighted_sets(A(),
					     ws,
					     n_basins,
					     (pw::cst(255) - pw::value(small)) | small.domain());

  for (unsigned l = 1; l <= n_basins; ++l)
    std::cout << l << ' ' << arr[l].n_items() << ' ' << arr[l].mean() << std::endl;

  typedef util::graph G;
  G gr = make::region_adjacency_graph(ws, c8(), n_basins);

  vertex_image<void,A,G> v_ima = make::vertex_image(gr, arr);

  edge_image<void,float,G> e_ima = make::edge_image(v_ima,
						    local::bloup());

  {
    typedef edge_image<void,float,G> I;
    mln_piter_(I) e(e_ima.domain());
    for_all(e)
    {
      std::cout << e.element().v1() << '-' << e.element().v2() << " : " << e_ima(e) << std::endl;
    }
  }

  trace::exiting("main");
}
