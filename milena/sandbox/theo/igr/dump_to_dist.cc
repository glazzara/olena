#include <cstdio>

#include <mln/core/var.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/concept/function.hh>

#include <mln/value/int_u12.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/util/array.hh>
#include <mln/data/transform.hh>
#include <mln/data/paste.hh>

#include <mln/morpho/elementary/closing.hh>

#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/is_separator.hh>
#include <mln/world/inter_pixel/separator_to_pixels.hh>
#include <mln/world/inter_pixel/display_edge.hh>
#include <mln/world/inter_pixel/neighb2d.hh>




namespace mln
{

  const float saturation = 0.5f;

  struct int_u8_from_float : Function_v2v< int_u8_from_float >
  {
    typedef value::int_u8 result;
    result operator()(float f) const
    {
      f /= saturation;
      if (f < 0.f)
	return 0u;
      if (f > 1.f)
	return 255u;
      return f * 255;
    }
  };


  // Distance function.
  struct fun_dist : Function_vv2v< fun_dist >
  {
    typedef float result;
    
    template <typename V>
    float operator()(const util::array<V>& v1,
		     const util::array<V>& v2) const
    {
      const unsigned n = v1.nelements();
      float
	sum_min = 0.f,
	sum_v1 = 0.f, sum_v2 = 0.f;

      for (unsigned i = 0; i < n; ++i)
	{
	  sum_min += std::min(v1[i], v2[i]);
	  sum_v1 += v1[i];
	  sum_v2 += v2[i];
	}

      float res = 1.f - sum_min / std::max(sum_v1, sum_v2);
      if (res < 0.f || res > 1.f)
	{
	  std::cerr << "oops!" << std::endl;
	  std::abort();
	}
      return res;
    }

  };

} // mln


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u12;

  if (argc != 3)
    std::abort();

  border::thickness = 0;

  image3d<int_u12> input;
  io::dump::load(input, argv[1]);

  unsigned n = input.nslices();
  std::cout << "n = " << n << std::endl;


  typedef image2d<util::array<int_u12> > I;
  I ima;

  mln_VAR(sli_0, slice(input, 0));
  initialize(ima, sli_0);

  box2d b = sli_0.domain();
  std::cout << "b = " << b << std::endl;

  // Initialization: resizing of arrays.
  mln_piter_(box2d) p(b);
  for_all(p)
    ima(p).resize(n);

  // Copying.
  for (unsigned t = 0; t < n; ++t)
    {
      image2d<int_u12> sli_t = duplicate(slice(input, t));
      for_all(p)
	ima(p)[t] = sli_t(p);
    }

  if (input.nelements() != n * b.nsites())
    std::cerr << "oops!" << std::endl;

//   FILE* file = std::fopen(argv[2], "w");
//   std::fwrite(ima.buffer(), input.nelements() * sizeof(int_u12), 1, file);
//   std::fclose(file);

  using namespace world::inter_pixel;
  mln_VAR(ima_, immerse(ima));

  image2d<float> d_(ima_.unmorph_().domain());
  mln_VAR(d, d_ | is_separator());

  {
    fun_dist f;
    mln_piter_(d_t) e(d.domain());
    for_all(e)
      {
	point2d p1, p2;
	separator_to_pixels(e, p1, p2);
	d(e) = f(ima_(p1), ima_(p2));
      }

  }

//   data::paste(morpho::elementary::closing(d, e2e()),
// 	      d);

  io::dump::save(d_, argv[2]);

//   mln_VAR(d_8, data::transform(d, int_u8_from_float()));

//   io::pgm::save(display_edge(d_8.unmorph_(), 0, 3),
// 		argv[2]);

}
