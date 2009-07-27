#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>

#include <mln/transform/distance_geodesic.hh>
#include <mln/accu/shape/bbox.hh>
#include <mln/morpho/closing/algebraic.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/watershed/superpose.hh>

#include <mln/core/routine/duplicate.hh>
#include <mln/data/convert.hh>
#include <mln/pw/all.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/ppm/save.hh>




namespace mln
{

  struct pair_and
  {
    pair_and()
    {}
    pair_and(unsigned first, unsigned second)
      : first(first),
	second(second)
    {}
//     bool operator<(const pair_and& other) const
//     {
//       return first < other.first && second < other.second;
//     }
    unsigned first, second;
  };


  bool operator<(const pair_and& lhs, const pair_and& rhs)
  {
    return lhs.first < rhs.first || lhs.second < rhs.second;
  }


  struct rectangle_and;


  namespace trait
  {

    template <>
    struct accumulator_< rectangle_and >
    {
      typedef accumulator::has_untake::no	has_untake;
      typedef accumulator::has_set_value::no	has_set_value;
      typedef accumulator::has_stop::no		has_stop;
      typedef accumulator::when_pix::use_p	when_pix;
    };

  } // end of namespace mln::trait


  struct rectangle_and : public accu::internal::base< pair_and, rectangle_and >
  {
    typedef point2d argument;

    rectangle_and()
    {
    }

    void init()
    {
      bb_.init();
    }
    void take_as_init_(const point2d& p)
    {
      bb_.take_as_init_(p);
    }
    void take(const point2d& p)
    {
      bb_.take(p);
    }
    void take(const rectangle_and& other)
    {
      bb_.take(other.bb_);
    }

    pair_and to_result() const
    {
      pair_and tmp(bb_.to_result().len(0),
		   bb_.to_result().len(1));
      return tmp;
    }

    bool is_valid() const
    {
      return true;
    }

  protected:
    accu::shape::bbox<point2d> bb_;
  };


} // mln


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm w h output.ppm" << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 5)
    usage(argv);

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  typedef value::int_u8 D;

  image2d<D> dmap;
  dmap = transform::distance_geodesic(input,
				      c8(),
				      mln_max(D));
  io::pgm::save(dmap, "temp_dmap.pgm");

  int w = std::atoi(argv[2]), h = std::atoi(argv[3]);

  pair_and lambda(w, h);
  image2d<D> clo = morpho::closing::algebraic(dmap,
					      c8(),
					      rectangle_and(),
					      lambda);

  io::pgm::save(clo, "temp_clo.pgm");

  typedef unsigned L;
  L n_basins;
  image2d<L> ws = morpho::watershed::flooding(clo, c8(), n_basins);
  std::cout << "n basins = " << n_basins<< std::endl;

  {
    image2d<rgb8> out = data::convert(rgb8(), dmap);
    out = morpho::watershed::superpose(out, ws, rgb8(255,0,0));
    io::ppm::save(out, argv[4]);
  }

//   {
//     image2d<bool> out(input.domain());
//     out = duplicate((pw::value(ws) == pw::cst(0)) | input.domain());
//     io::pbm::save(out, argv[2]);
//   }

}
