#include "filetype.hh"

#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/elementary/gradient_internal.hh>
#include <mln/math/diff_abs.hh>

#include <mln/accu/image/init.hh>
#include <mln/accu/image/take.hh>
#include <mln/accu/image/to_result.hh>

#include <mln/accu/stat/max.hh>



namespace mln
{

  value::int_u8
  dist(const value::rgb8& c1,
       const value::rgb8& c2)
  {
    unsigned d = 0;
    d += math::diff_abs(c1.red(), c2.red());
    d += math::diff_abs(c1.green(), c2.green());
    d += math::diff_abs(c1.blue(), c2.blue());
    if (d > 255)
      d = 255;
    return d;
  }

  template <typename N>
  image2d<value::int_u8>
  color_gradient(const image2d<value::rgb8>& input,
		 const N& nbh)
  {
    using value::int_u8;
    image2d<int_u8> output(input.domain());

    mln_piter(box2d) p(input.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
    {
      int_u8 d = 0u;
      for_all(n) if (input.domain().has(n))
	{
	  int_u8 d_ = dist(input(p), input(n));
	  if (d_ > d)
	    d = d_;
	}
      output(p) = d;
    }

    return output;
  }



  template <typename I>
  image2d<value::int_u8>
  get_red(const I& input)
  {
    typedef image2d<value::int_u8> J;
    J output(input.domain());

    mln_pixter(const I) p_in(input);
    mln_pixter(J) p_out(output);
    for_all_2(p_in, p_out)
      p_out.val() = p_in.val().red();
    return output;
  }

  template <typename I>
  image2d<value::int_u8>
  get_green(const I& input)
  {
    typedef image2d<value::int_u8> J;
    J output(input.domain());

    mln_pixter(const I) p_in(input);
    mln_pixter(J) p_out(output);
    for_all_2(p_in, p_out)
      p_out.val() = p_in.val().green();
    return output;
  }

  template <typename I>
  image2d<value::int_u8>
  get_blue(const I& input)
  {
    typedef image2d<value::int_u8> J;
    J output(input.domain());

    mln_pixter(const I) p_in(input);
    mln_pixter(J) p_out(output);
    for_all_2(p_in, p_out)
      p_out.val() = p_in.val().blue();
    return output;
  }


} // mln


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.ppm output.pgm" << std::endl
	    << "  Color gradient." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::rgb8;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  trace::entering("main");

  image2d<rgb8> ima;
  io::ppm::load(ima, argv[1]);

  typedef accu::stat::max<int_u8> A;
  image2d<A> grad(ima.domain());

  using morpho::elementary::gradient_internal;

  accu::image::init(grad);

  accu::image::take(grad,
		    gradient_internal(get_red(ima),
				      c4()));
  accu::image::take(grad,
		    gradient_internal(get_green(ima),
				      c4()));
  accu::image::take(grad,
		    gradient_internal(get_blue(ima),
				      c4()));

  io::pgm::save(accu::image::to_result(grad),
		argv[2]);

  trace::exiting("main");
}
