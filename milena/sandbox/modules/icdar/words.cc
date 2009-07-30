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


#include <mln/core/image/image2d.hh>
#include <mln/core/image/imorph/tr_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/concept/function.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/var.hh>

#include <mln/fun/v2b/threshold.hh>
#include <mln/data/transform.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/labeling/wrap.hh>
#include <mln/labeling/colorize.hh>
#include <mln/morpho/watershed/superpose.hh>
#include <mln/logical/not.hh>
#include <mln/world/binary_2d/subsample.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/closing/structural.hh>

#include <mln/transform/distance_front.hh>

#include <mln/make/w_window2d_int.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/all.hh>

#include <mln/value/int_u8.hh>



namespace mln
{

  namespace fun
  {

    namespace p2p
    {

      // Forward declaration.
      template <typename P> struct enlarge;


      template <typename P>
      struct shrink : Function_v2v < shrink<P> >
      {
	typedef P result;
	typedef enlarge<P> invert;

	shrink(unsigned factor = 2)
	{
	  mln_precondition(factor != 0);
	  this->factor = factor;
	}

	P operator()(const P& p) const
	{
	  mln_precondition(factor != 0);
	  P tmp(p);
	  for (unsigned i = 0; i < P::dim; ++i)
	    tmp[i] /= factor;
	  return tmp;
	}

	invert inv() const
	{
	  return enlarge<P>(factor);
	}

	unsigned factor;
      };


      template <typename P>
      struct enlarge : Function_v2v < enlarge<P> >
      {
	typedef P result;
	typedef shrink<P> invert;

	enlarge(unsigned factor = 2)
	{
	  mln_precondition(factor != 0);
	  this->factor = factor;
	}

	P operator()(const P& p) const
	{
	  mln_precondition(factor != 0);
	  P tmp(p);
	  for (unsigned i = 0; i < P::dim; ++i)
	    tmp[i] *= factor;
	  return tmp;
	}

	invert inv() const
	{
	  return shrink<P>(factor);
	}

	unsigned factor;
      };

    } // end of namespace mln::fun::p2p

  } // end of namespace mln::fun


  template <typename S, typename I, typename F>
  tr_image<S,I,F>
  transform_p2p(const Site_Set<S>& s, const Image<I>& ima, const Function_v2v<F>& t)
  {
    tr_image<S,I,F> tmp(exact(s), exact(ima), exact(t));
    return tmp;
  }

} // end of namespace mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.ppm" << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);

  using namespace mln;

  typedef value::int_u8 L;
  typedef image2d<bool> I;


  // Parameters.

  // Related to the down-sizing.
  const unsigned factor = 2;

  // Related to the structural closing.
  const unsigned height = 5, width  = 11;

  // Related to the distance map filtering.
  const unsigned l_area = 500;

  // end of Parameters.



  I input;
  io::pbm::load(input, argv[1]);


  /// Reduction and invertion.
  image2d<L>
    small_gl = world::binary_2d::subsample(input, factor);
  I small = data::transform(small_gl, fun::v2b::threshold<L>(191));
  logical::not_inplace(small);


  /// Structural closing.
  I clo = morpho::closing::structural(small,
				      win::rectangle2d(height,
						       width));


  /// Distance map computation.
  int ww[] = { 00, 11,  0, 11,  0,
	       11,  7,  5,  7, 11,
	       00,  5,  0,  5,  0,
	       11,  7,  5,  7, 11,
	       00, 11,  0, 11,  0 };

  typedef value::int_u<12> int_u12;
  image2d<int_u12>
    dmap = transform::distance_front(clo,
				     c4(), make::w_window2d_int(ww),
				     mln_max(int_u12)),
    dmap_ = morpho::closing::area(dmap, c4(), l_area);


  /// Watershed.
  L n_words;
  image2d<L> ws_ = morpho::watershed::flooding(dmap_,
					       c4(),
					       n_words);

  /// Remove non-significant regional minima.
  image2d<L> ws  = morpho::elementary::dilation(ws_, c4());

  /// Adjust the domain of the watershed image.
  mln_VAR(ws_large, transform_p2p(input.domain(), ws,
				  fun::p2p::enlarge<point2d>(factor)));

  image2d<L> output(input.domain());
  data::fill(output, literal::zero);

  data::paste(ws_large | (pw::value(input) == pw::cst(false)), output);
  io::ppm::save(labeling::colorize(output, n_words), argv[2]);


#ifdef LOG
  io::pbm::save(small, "temp_small.pbm");
  io::pbm::save(clo, "temp_clo.pbm");
  io::pgm::save(labeling::wrap(dmap),  "temp_dmap.pgm");
  io::pgm::save(labeling::wrap(dmap_), "temp_dmap_.pgm");

  io::pgm::save(labeling::wrap(ws),  "temp_ws.pgm");
  io::pgm::save(labeling::wrap(ws_), "temp_ws_.pgm");

  /// Adjust the domain of the watershed image.
  mln_VAR(ws_large_, transform_p2p(input.domain(), ws_,
				   fun::p2p::enlarge<point2d>(factor)));
  io::ppm::save(morpho::watershed::superpose(input, ws_large_),
		"temp_ws_on_input.ppm");
#endif // ! LOG

}
