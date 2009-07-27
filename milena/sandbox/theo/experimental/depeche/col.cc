#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/alias/neighb1d.hh>

#include <mln/fun/v2v/projection.hh>
#include <mln/core/image/dmorph/unproject_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>

#include <mln/accu/stat/mean.hh>
#include <mln/accu/image/init.hh>
#include <mln/accu/image/take.hh>
#include <mln/accu/image/to_result.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/plot/save.hh>

#include <mln/linear/gaussian_1d.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/data/convert.hh>
#include <mln/io/ppm/save.hh>

#include <mln/data/fill.hh>
#include <mln/debug/println.hh>
#include <mln/accu/stat/median_h.hh>

#include <mln/morpho/closing/volume.hh>



namespace mln
{

  
  float get_approx_gap(const util::array<unsigned>& line, unsigned delta)
  {
    accu::stat::median_h< value::int_u<12> > med;
    const unsigned n = line.nelements() - delta;
    for (unsigned i = 0; i < n; ++i)
      med.take(line[i + delta] - line[i]);

     std::cout << "delta = " << delta << std::endl;
     std::cout << med.histo() << std::endl;
     std::cout << "med = " << med.to_result() << std::endl;

    return float(med.to_result()) / delta;
  }

  
  void refine_gap(const util::array<unsigned>& line, unsigned delta, float& gap)
  {
     std::cout << "gap = " << gap << std::endl;
    unsigned count = 0;

    accu::stat::mean<float> mean;
    const unsigned n = line.nelements() - delta;
    for (unsigned i = 0; i < n; ++i)
      {
	float g = line[i + delta] - line[i];
	if (g / delta > gap - .2 && g / delta < gap + .2)
	  {
	    mean.take(g);
	    ++count;
	  }
      }
     std::cout << "count = " << count << "  total = " << n << std::endl;

    gap = mean.to_result() / delta;
     std::cout << "gap = " << gap << std::endl;
  }


  util::array<unsigned>
  select_lines(const util::array<unsigned>& line, float gap)
  {
    std::set<unsigned> s;
    const unsigned n = line.nelements() - 1;
    for (unsigned i = 0; i < n; ++i)
      {
	float g = line[i + 1] - line[i];
	if (g > gap - 2 && g < gap + 2)
	  {
	    s.insert(line[i]);
	    s.insert(line[i + 1]);
	  }
      }

    std::vector<unsigned> v(s.begin(), s.end());
    util::array<unsigned> out;
    out.hook_std_vector_() = v;

    return out;
  }


  util::array<unsigned>
  image_2_line(const image1d<double>& input)
  {
    image1d<bool> is_max(input.domain());
    unsigned n;
    data::fill(is_max,
	       pw::value(labeling::regional_maxima(input, c2(), n)) | input.domain());
    std::cout << "n = " << n << std::endl;

    util::array<unsigned> line;
    {
      mln_piter_(box1d) p(input.domain());
      for_all(p)
	if (is_max(p))
	  line.append(p.ind());
    }

    return line;
  }


  util::array<unsigned>
  analyze(const util::array<unsigned>& line, float& gap, bool& ok)
  {
    // FIXME: The value of delta in 'get_approx_gap' and in
    // 'refine_gap' should change w.r.t. to the nature of input data.
    gap = get_approx_gap(line, 10); // FIXME: 10 / 3
    float rough_gap = gap; 
    refine_gap(line, 20, gap);      // FIXME: 20 / 4
    float finer_gap = gap;

    ok = std::abs(finer_gap - rough_gap) < 1;

    return select_lines(line, gap);
  }


  void draw(const image2d<value::int_u8>& input,
	    const util::array<unsigned>& line,
	    value::rgb8 color,
	    const std::string& filename)
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);
    const int nrows = input.nrows();

    for (unsigned i = 0; i < line.nelements(); ++i)
      {
	int col = line[i];
	for (int row = 0; row < nrows; ++row)
	  output.at_(row, col) = color;
      }
  
    io::ppm::save(output, filename);
}


} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda h.txt output.ppm" << std::endl;
  std::cerr << "hint: lambda = 50; delta = 10 / 20 otherwise 3 / 4" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 5)
    usage(argv);

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);
  box2d b = input.domain();


  // h computing

  typedef accu::stat::mean<int_u8, unsigned, double> A;
  
  image1d<A> hmean(b.ncols());
  accu::image::init(hmean);

  fun::v2v::projection<point2d, 0> hproj;
  accu::image::take(unproject(hmean, b, hproj).rw(), input);

  image1d<double> h = accu::image::to_result(hmean);
  io::plot::save(h, argv[3]);


  // h cleaning

  h = linear::gaussian_1d(h, 1.0, 255);
  io::plot::save(h, "h_g.txt");

  int lambda = std::atoi(argv[2]);
  h = morpho::closing::volume(h, c2(), lambda);
  io::plot::save(h, "h_gc.txt");



  // analyzing

  util::array<unsigned> line = image_2_line(h);
  draw(input, line, value::rgb8(255,0,0), "temp_h.ppm");

  float gap;
  bool ok;
  line = analyze(line, gap, ok);

  draw(input, line, value::rgb8(255,0,0), "temp_h_sel.ppm");

  if (! ok)
    {
      std::cerr << "analyze failed: abort!" << std::endl;
      std::abort();
    }



  // getting a math model
  float o;

  {
    accu::stat::mean<float> me;
    // col = r * gap  where  r = o + i (with offset o and integer i)
    // col = gap * i + orig (orig = o * gap)
    for (unsigned l = 0; l < line.nelements(); ++l)
      {
	float r = float(line[l]) / gap;
	int i = int(r + 0.49999);
	float o = r - float(i);
 	me.take(o);
	std::cout << line[l] << " = " << (gap * o) << " + " << i << " * " << gap << std::endl;
      }
    o = me.to_result();

    std::cout << "gap = " << gap << "  offset = " << (gap * o) << std::endl;

    // verif
    util::array<unsigned> line_;
    for (unsigned l = 0; l < line.nelements(); ++l)
      {
 	int i = int(float(line[l]) / gap - o + 0.49999);
 	line_.append(int((o + i) * gap + 0.49999));
      }

    std::cout << std::endl;

    // over-display
    draw(input, line, value::rgb8(255,0,0), "temp_h_found.ppm");

  }


  // drawing matrix

  {
    image2d<rgb8> output = data::convert(rgb8(), input);
    const int nrows = input.nrows();
    int ncols = input.ncols();

    int n_max = int(float(ncols) / gap - o + 3);
    for (int i = 0; i < n_max; ++i)
      {
	float col_ = (o + i) * gap;
	int col = int(col_ + 0.49999);
	if (col < 0)
	  continue;
	if (col > ncols - 1)
	  break;
	for (int row = 0; row < nrows; ++row)
	  output.at_(row, col) = rgb8(0,255,0);
      }

    io::ppm::save(output, argv[4]);
  }



}
