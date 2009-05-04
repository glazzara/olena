// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/* core */
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

/* io */
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

/* Gradient */
#include "color_internal_gradient.hh"

/* Labeling */
#include <mln/level/convert.hh>
#include "color_labeling_mean.hh"

/* WST */
#include <mln/morpho/watershed/all.hh>
#include <mln/morpho/elementary/dilation.hh>

/* Tree computation */
#include <mln/level/sort_psites.hh>
#include <mln/morpho/tree/data.hh>
#include "../../tree/propagate.hh"
#include "../../tree/components.hh"

#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/attribute/sharpness.hh>

/* pw */
#include <mln/core/concept/function.hh>
#include <mln/fun/p2v/ternary.hh>
#include <mln/pw/all.hh>

//std
#include <iostream>
#include <string>

#include <mln/level/stretch.hh>
#include <mln/io/pgm/save.hh>

namespace mln
{
  // Sharpness Attribute -> Height Attribute
  template <typename P2V>
  struct height_wrapper_ : Function_p2v< height_wrapper_<P2V> >
  {
    typedef unsigned result;

    height_wrapper_(const Function_p2v<P2V>& f) :
      f_ (exact(f))
    {
      mlc_is_a(mln_result(P2V), Accumulator)::check();
    }

    template <typename P>
    unsigned operator() (const P& p) const
    {
      return f_(p).height();
    }

  private:
    const P2V& f_;
  };

  // meta
  template <typename P2V>
  inline
  height_wrapper_<P2V>
  height_wrapper(const Function_p2v<P2V>& f)
  {
    return height_wrapper_<P2V>(f);
  }
}





void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " input.ppm height [-o1 | -o2 | -o3]"
	    << std::endl
	    << "-o1\tSuperpose WS on input image (out: superpose.ppm)" << std::endl
	    << "-o2\tCount number of basins" << std::endl
  	    << "-o3\tReplace basin by its mean color on the input (out: mean.ppm)"
	    << std::endl;
  abort();
}


int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;
  using value::rgb8;

  if (argc < 3)
    usage(argv);

  int output_ = 0;
  int lambda_h = std::atoi(argv[2]);

  if (argc == 4)
    if (std::string(argv[3]) == "-o1")
      output_ = 1;
    else if (std::string(argv[3]) == "-o2")
      output_ = 2;
    else if (std::string(argv[3]) == "-o3")
      output_ = 3;
    else
      usage(argv);



  // Load color image.
  typedef image2d<rgb8> I_;

  I_ input_;
  io::ppm::load(input_, argv[1]);

  /***************************/
  /* Preprocessing (gradient)*/
  /***************************/
  typedef image2d<int_u8> I;

  I input = color_internal_gradient(input_);
  io::pgm::save(input, "gradient.pgm");

  /***************************/
  /* Component tree creation */
  /***************************/
  typedef p_array<point2d> S;
  typedef morpho::tree::data<I,S> tree_t;

  S s = level::sort_psites_decreasing(input);
  tree_t t(input, s, c4());


  /******************************/
  /* Compute Attribute On Image */
  /******************************/
  typedef mln_ch_value_(I, double) A;
  A a;
  {
    typedef morpho::attribute::sharpness<I> sharp_t;
    typedef mln_ch_value_(I, sharp_t) B;

    // Attribute Pruning
    B a_img;
    a = morpho::tree::compute_attribute_image(sharp_t (), t, &a_img);

    // Component filtering
    a = duplicate((fun::p2v::ternary(height_wrapper(pw::value(a_img)) > pw::cst(lambda_h),
				   pw::value(a),
				   pw::cst(0.0))) | a.domain());
  }

  /************************************************/
  /* Retrieve Components (Maximising the criteria)*/
  /************************************************/
  A component_img;
  {
    p_array< mln_psite_(A) > obj_array;
    I output = level::stretch(int_u8(), a); //adapt to 0-255
    io::pgm::save(output, "components.pgm");

    obj_array = morpho::tree::get_components(t, a);
    std::cout << obj_array.nsites() << std::endl;

    component_img = morpho::tree::propagate_components(a, t, obj_array, 1.1);

    // debug
    //I output = level::stretch(int_u8(),  component_img); //adapt to 0-255
    //io::pgm::save(output, "components.pgm");

 }


  /************************************************/
  /* WATERSHED                                    */
  /************************************************/
  typedef image2d<int_u16> WST;
  int_u16 n_basins = 0;
  WST wst = morpho::watershed::flooding(component_img, c4(), n_basins);

  if (!output_ || output_ == 1)
    {
      I_ out = morpho::watershed::superpose(input_, wst);
      io::ppm::save(out, "superpose.pgm");
    }
  if (!output_ || output_ == 2)
    std::cout << "Number of basins: " << n_basins << std::endl;

  if (!output_ || output_ == 3)
    {
      typedef image2d<value::label_16> L;
      L lab = level::convert(value::label_16(),
			     morpho::elementary::dilation(wst, c8()));
      I_ out = color_labeling_mean(input_, lab, n_basins);
      io::ppm::save(out, "mean.pgm");
    }
}
