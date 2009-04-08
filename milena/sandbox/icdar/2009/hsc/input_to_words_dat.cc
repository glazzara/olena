#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/debug/colorize.hh>
#include <mln/estim/min_max.hh>

#include <sandbox/icdar/2009/hsc/input_to_lines.hh>
#include <sandbox/icdar/2009/hsc/line_to_words.hh>
#include <sandbox/icdar/2009/hsc/get_line_images.hh>
#include <sandbox/icdar/2009/hsc/io/icdar/save.hh>


namespace mln
{

  template <typename L>
  void
  paste_words(image2d<L>& output,
	      const image2d<L>& words,
	      L l_1st)
  {
    mln_piter(box2d) p(words.domain());
    for_all(p)
    {
      if (words(p) == 0)
	continue;
      output(p) = l_1st.to_enc() + words(p).to_enc();
    }
  }
  
} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.dat [output.ppm]" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:  a raw binary 2D image (either a line or a page)" << std::endl
	    << "  output.dat: output image where words are labeled (int_u8)" << std::endl
	    << "              0 is the background label." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 3 && argc != 4)
    usage(argv);


  trace::entering("main");

  typedef image2d<bool> I_bw;

  I_bw input;
  io::pbm::load(input, argv[1]);


  typedef value::label<12> Ll;
  Ll n_lines;
  image2d<Ll> lines = input_to_lines(input, n_lines, 0.501); // with majoritary

  // std::cout << "n lines = " << n_lines << std::endl;

  typedef value::int_u16 L;

  image2d<L> output(input.domain());
  data::fill(output, 0);

  util::array<I_bw> line_ = get_line_images(lines, n_lines, 51);

  L n_words = 0;
  for (unsigned l = 1; l <= n_lines; ++l)
    {
      I_bw line = line_[l]; // A line image.

      L n_line_words = 0;
      image2d<L> words = line_to_words(line, n_line_words, false);
      paste_words(output, words, /* 1st label is */ n_words);

      // std::cout << "line = " << l
      //	<< "  n_words = " << n_line_words << std::endl;

      n_words += n_line_words;
    }

  {
    // L l_min, l_max;
    // estim::min_max(output, l_min, l_max);
    // if (l_max != n_words)
    //  std::cout << "oops: " << l_max << ' ' << n_words << std::endl;
  }

  io::icdar::save(output, argv[2]);

  if (argc == 4)
    {
      io::ppm::save(debug::colorize(value::rgb8(),
				    output,
				    n_words),
		    argv[3]);
    }

  trace::exiting("main");
}
