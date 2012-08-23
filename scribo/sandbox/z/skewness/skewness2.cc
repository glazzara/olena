#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/pw/all.hh>
#include <mln/value/int_u8.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/neighb.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/extension/adjust.hh>
#include <mln/border/mirror.hh>
#include <mln/extension/duplicate.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/debug/println.hh>
#include <mln/data/transform.hh>
#include <mln/io/dump/save.hh>
#include <mln/accu/math/sumpow.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/data/compute_in_window.hh>
#include <mln/core/var.hh>
#include <mln/data/saturate.hh>
#include <mln/fun/v2b/threshold_le.hh>
#include <mln/arith/revert.hh>
#include <mln/util/timer.hh>

mln::image2d<double> skewness;

#include <scribo/binarization/sauvola_ms.hh>

mln::image2d<bool> skewness_pbm;
std::string prefix;


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " <input.pgm> <prefix> <win_size>" << std::endl;
    return 1;
  }

  util::timer tt;
  tt.start();
  util::timer t;

  t.start();
  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);

  prefix = argv[2];

  int win_size = atoi(argv[3]);
  win::rectangle2d wr = win::rectangle2d(win_size,win_size);
  window2d win;
  win.insert(wr);

  accu::pair<accu::stat::mean<value::int_u8,double>,
    accu::pair<accu::math::sumpow<2,value::int_u8,double>,
               accu::math::sumpow<3,value::int_u8,double> > > accu;
  t.stop();
  std::cout << "initialization - " << t << std::endl;


  t.restart();
  mln_VAR(res, data::compute_in_window(accu, input, win));
  t.stop();
  std::cout << "compute in window - " << t << std::endl;

  initialize(skewness, res);
  data::fill(skewness, 0);


  point2d pc = input.domain().pcenter();
  std::cout << pc << " - sum_2 = " << res(pc).second.first << " - sum_3 = " << res(pc).second.second << " - mean = " << res(pc).first << std::endl;

  t.restart();
  {
    int n;
    mln_pixter_(image2d<double>) p(skewness);
    for_all(p)
    {
      n = win.size();
      mln_VAR(d, res.element(p.offset()));
      double m = d.first;
      double sum_3 = d.second.second;
      double sum_2 = d.second.first;
      p.val() = (sum_3 - 3. * m * sum_2) / (double) n + 2. * std::pow(m, 3);
    }
  }
  t.stop();
  std::cout << "Compute unskew - " << t << std::endl;


  // t.restart();
  // skewness_pbm = data::transform(skewness, fun::v2b::threshold_le<double>(0));
  // t.stop();
  // std::cout << "binarize - " << t << std::endl;

  // {
  //   io::dump::save(skewness, prefix + "skewness.dump");
  //   io::pgm::save(data::convert(value::int_u8(), skewness), prefix + "skewness_u8.pgm");
  //   io::pgm::save(data::stretch(value::int_u8(), skewness), prefix + "skewness_stretch.pgm");
  //   io::pbm::save(skewness_pbm, prefix + "skewness.pbm");
  // }

  t.restart();
  {
    border::resize(skewness, input.border());
    mln_pixter_(image2d<value::int_u8>) p(input);
    for_all(p)
    {
      if (skewness.element(p.offset()) > 0)
	p.val() = mln_max(value::int_u8) - p.val();
    }
  }
  t.stop();
  std::cout << "Negate parts of input image - " << t << std::endl;

  t.restart();
  image2d<bool> bin = scribo::binarization::sauvola_ms(input, 101, 2);
  std::cout << "sauvola_ms - " << t << std::endl;


  std::cout << "Total time : " << tt << std::endl;
  // prefix += "i_";

  // image2d<bool> bin_i = scribo::binarization::sauvola(arith::revert(input), 51);

  // prefix = argv[2];

  io::pbm::save(bin, prefix + "bin.pbm");
  // io::pbm::save(bin_i, prefix + "bin_i.pbm");

  // image2d<bool> out;
  // initialize(out, bin);
  // mln_piter_(image2d<bool>) p(out.domain());
  // for_all(p)
  //   if (skewness_pbm(p))
  //     out(p) = bin(p);
  //   else
  //     out(p) = bin_i(p);

  // io::pbm::save(out, prefix + "output.pbm");

  // io::dump::save(scribo::binarization::internal::debug_stddev, prefix + "stddev.dump");
  // io::dump::save(scribo::binarization::internal::debug_mean, prefix + "mean.dump");
  // io::dump::save(scribo::binarization::internal::debug_threshold, prefix + "threshold.dump");

}
