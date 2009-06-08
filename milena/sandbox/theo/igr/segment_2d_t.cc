#include <string>

#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/world/inter_pixel/is_separator.hh>
#include <mln/world/inter_pixel/neighb2d.hh>
#include <mln/world/inter_pixel/display_edge.hh>

#include <mln/debug/println.hh>
#include <mln/estim/min_max.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label.hh>
#include <mln/data/transform.hh>

#include <mln/morpho/closing/height.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/labeling/compute.hh>
#include <mln/arith/div.hh>


const float saturation = 0.5;


namespace mln
{

  struct int_u12_from_float : Function_v2v< int_u12_from_float >
  {
    typedef value::int_u12 result;
    result operator()(float f) const
    {
      mln_precondition(f >= 0.f && f <= 1.f);
      unsigned i = f / saturation * 4095;
      return i > 4095 ? 4095 : i;
    }
  };

  template <typename I>
  void debug_println(const std::string& name, const I& input)
  {
    box2d crop( point2d(100, 180),
		point2d(109, 189) );
    debug::println(name,
		   (input.unmorph_() | crop)
		   | world::inter_pixel::is_separator());
  }

  template <typename I>
  void io_save_edges_int_u12(const I& input,
		     value::int_u8 bg,
		     const std::string& filename)
  {
    mlc_equal(mln_value(I), value::int_u12)::check();
    mln_ch_value(I, value::int_u8) output;
    initialize(output, input);
    arith::div_cst(input, 16, output);
    io::pgm::save(world::inter_pixel::display_edge(output.unmorph_(),
						   bg,
						   3),
		  filename);
  }

} // mln



int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace world::inter_pixel;
  using value::int_u12;
  using value::int_u8;

  if (argc != 3)
    std::abort();

  unsigned lambda = atoi(argv[2]);

  image2d<float> df_;
  io::dump::load(df_, argv[1]);
  mln_VAR(df, df_ | is_separator());

  {
    std::cout << df_.domain() << std::endl;
    float min, max;
    estim::min_max(df, min, max);
    std::cout << "min = " << min << "  max = " << max << std::endl;
  }  

  mln_VAR(d, data::transform(df, int_u12_from_float()));
  // debug_println("d", d);
  io_save_edges_int_u12(d, 0, "d.pgm");

  mln_VAR(d_clo, morpho::closing::height(d, e2e(), lambda));
  // debug_println("d_clo", d_clo);
  io_save_edges_int_u12(d_clo, 0, "d_clo.pgm");

  typedef value::label<12> L;
  L n_basins;
  mln_VAR(w, morpho::watershed::flooding(d_clo, e2e(), n_basins));
  std::cout << "n basins = " << n_basins << std::endl;
  // debug_println("w", w);
  // io_save_edges_int_u12(w, 0, "w.pgm");

  typedef accu::stat::mean<int_u12,float,int_u12> A;
  util::array<int_u12> m = labeling::compute(A(), d, w, n_basins);

  {
    util::array<int_u8> m_(n_basins.next());
    m_[0] = 1; // watershed line <=> 1
    for (unsigned l = 1; l <= n_basins; ++l)
      {
	m_[l] = m[l] / 16;
	if (m_[l] < 2) m_[l] == 2;
	// basin <=> 2..255
      }
    mln_VAR(d_m, data::transform(w, m_));
    mln_VAR(out, world::inter_pixel::display_edge(d_m.unmorph_(),
						  0, // background <=> 0
						  3));
    io::pgm::save(out, "d_m.pgm");
  }
}
