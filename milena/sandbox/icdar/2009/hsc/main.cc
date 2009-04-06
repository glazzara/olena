#include <set>
#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/pw/all.hh>
#include <mln/data/fill.hh>
#include <mln/level/saturate.hh>
#include <mln/level/convert.hh>
#include <mln/arith/revert.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>
#include <mln/debug/colorize.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/world/binary_2d/subsample.hh>

#define MLN_FLOAT double
#include <sandbox/theo/exec/gaussian_directional_2d.hh>

#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/accu/stat/variance.hh>



namespace mln
{


  template <typename L>
  inline
  L find_root_(std::vector<L>& parent, L l)
  {
    if (parent[l] == l)
      return l;
    return parent[l] = find_root_(parent, parent[l]);
  }



  template <typename L>
  image2d<L>
  merge(const image2d<value::int_u8>& ima,
	const image2d<L>& ws,
	L n,
	unsigned n_min_stats)
  {
    typedef accu::stat::variance<float> A;
    std::vector<A>
      vvar(n + 1),
      hvar(n + 1);

    mln_piter(box2d) p(ima.domain());
    for_all(p)
      if (ima(p) != 255 && ws(p) != 0)
	vvar[ws(p)].take(255 - ima(p), p.row());


    image2d<bool>
      radj(n + 1, n + 1),
      ladj(n + 1, n + 1);
    data::fill(radj, false);
    data::fill(ladj, false);

    for_all(p) if (ws(p) == 0)
      {
	if (ws(p + left) == 0 || ws(p + right) == 0)
	  // ws is horizontal => forget it
	  continue;
	L l1 = ws(p + left),
	  l2 = ws(p + right);
	if (l1 == 0 || l2 == 0 || l2 == l1)
	  continue;
	radj.at_(l1, l2) = true;
	ladj.at_(l2, l1) = true;
      }

    std::vector<L>
      best_right(n + 1, 0),
      best_left(n + 1, 0);
    L best_l2;
    float best_score;

    for (L l1 = 1; l1 <= n; ++l1)
      {
 	if (! vvar[l1].is_valid() || vvar[l1].n_items() < n_min_stats)
	  // Non significative stats.
	  continue;


	// Look right.

# ifdef LOG
	std::cout << l1 << ": ";
# endif

	best_l2 = 0;
	best_score;
	
	for (L l2 = 1; l2 <= n; ++l2)
	  {
	    if (! radj.at_(l1, l2)) // l1 -> l2
	      continue;

	    if (! vvar[l2].is_valid() || vvar[l2].n_items() < n_min_stats)
	      // Non significative stats.
	      continue;

# ifdef LOG
	    std::cout << l2 << ' ';
# endif

	    float vm, vM, sm, sM;
	    if (vvar[l1].mean() < vvar[l2].mean())
	      {
		vm = vvar[l1].mean();
		sm = vvar[l1].standard_deviation();
		vM = vvar[l2].mean();
		sM = vvar[l2].standard_deviation();
	      }
	    else
	      {
		vm = vvar[l2].mean();
		sm = vvar[l2].standard_deviation();
		vM = vvar[l1].mean();
		sM = vvar[l1].standard_deviation();
	      }

	    float score = (vm - sm) - (vM - sM);
# ifdef LOG
	    std::cout << '(' << score << "), ";
# endif
	    if (best_l2 == 0 || score > best_score)
	      {
		best_score = score;
		best_l2 = l2;
	      }
	  }

	if (best_l2 != 0)
	  {
	    best_right[l1] = best_l2;
# ifdef LOG
	    std::cout << "   ...   " << l1 << " -> " << best_l2 << std::endl;
# endif
	  }

	// Look left.

# ifdef LOG
	std::cout << l1 << ": ";
# endif

	best_l2 = 0;
	best_score;
	
	for (L l2 = 1; l2 <= n; ++l2)
	  {
	    if (! ladj.at_(l1, l2)) // l2 <- l1
	      continue;

	    if (! vvar[l2].is_valid() || vvar[l2].n_items() < n_min_stats)
	      // Non significative stats.
	      continue;

# ifdef LOG
	    std::cout << l2 << ' ';
# endif

	    float vm, vM, sm, sM;
	    if (vvar[l1].mean() < vvar[l2].mean())
	      {
		vm = vvar[l1].mean();
		sm = vvar[l1].standard_deviation();
		vM = vvar[l2].mean();
		sM = vvar[l2].standard_deviation();
	      }
	    else
	      {
		vm = vvar[l2].mean();
		sm = vvar[l2].standard_deviation();
		vM = vvar[l1].mean();
		sM = vvar[l1].standard_deviation();
	      }

	    float score = (vm - sm) - (vM - sM);
# ifdef LOG
	    std::cout << '(' << score << "), ";
# endif

	    if (best_l2 == 0 || score > best_score)
	      {
		best_score = score;
		best_l2 = l2;
	      }
	  }

	if (best_l2 != 0)
	  {
	    best_left[l1] = best_l2;
# ifdef LOG
	    std::cout << "   ...   " << best_l2 << " <- " << l1 << std::endl;
# endif
	  }

      }

	
    std::vector<L> parent(n + 1);
    for (L l = 0; l <= n; ++l)
      parent[l] = l;

    for (L l1 = 1; l1 <= n; ++l1)
      {
	L l2 = best_right[l1];
	if (l2 == 0)
	  continue;
	if (best_left[l2] == l1)
	  {
	    // cross-validation
	    L l1r = find_root_(parent, l1),
	      l2r = find_root_(parent, l2);
	    if (l2r == l1r)
	      continue; // already merged
	    if (l1r < l2r)
	      parent[l1r] = l2r;
	    else
	      parent[l2r] = l1r;
	  }
      }


    for (L l = 1; l <= n; ++l)
      parent[l] = find_root_(parent, l);


    image2d<L> out(ima.domain());
    for_all(p)
      out(p) = parent[ws(p)];

    // io::pgm::save(out, "tmp_out.pgm");

    return out;

  } // end of 'merge'



} // ! mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.pgm" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:   input 2D binary image (text is black; background is white)" << std::endl
	    << "  output.dump: output image where line components are labeled (int_u8)" << std::endl
	    << "               0 is the background label." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 3)
    usage(argv);


  // Parameters.

  const unsigned
    subsampling_factor = 4,
    height = 5,
    width = 25,
    n_min_stats = 1000;
  const float
    h_sigma = 31,
    v_sigma = 1.3;

  // end of Parameters.


  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  // Sub-sampling.
  image2d<int_u8>
    small = world::binary_2d::subsample(input, subsampling_factor),
    fuzzy, clo,
    ws,
    spc;


# ifdef LOG
  io::pgm::save(small, "tmp_small.pgm");
# endif


  // Fuzzifying.
  {
    image2d<MLN_FLOAT> temp(small.domain()), out;
    data::fill(temp, small);

    out = linear::gaussian_directional_2d(temp, 1, h_sigma, 0);
    out = linear::gaussian_directional_2d(out,  0, v_sigma, 0);

    fuzzy = level::saturate(int_u8(), out);

# ifdef LOG
    io::pgm::save(fuzzy, "tmp_fuzzy.pgm");
# endif
  }


  clo = morpho::closing::structural(fuzzy, win::rectangle2d(height, width));

# ifdef LOG
    io::pgm::save(clo, "tmp_clo.pgm");
# endif

  int_u8 n_basins;
  ws = morpho::watershed::flooding(clo, c4(), n_basins);


  spc = merge(small, ws, n_basins, n_min_stats); // Merging!


# ifdef LOG

  {
    io::ppm::save(debug::colorize(rgb8(), spc, n_basins),
		  "tmp_spc.ppm");

    io::pgm::save(ws, "tmp_ws.pgm");

    image2d<rgb8> cool = level::convert(rgb8(), small);
    data::fill((cool | (pw::value(ws) == pw::cst(0))).rw(),
    	       literal::red);
    io::ppm::save(cool, "tmp_ws.ppm");
  }
# endif


  // Outputing.
  {
    image2d<int_u8> output(input.domain());

    mln_piter_(box2d) p(input.domain());
    for_all(p)
      if (input(p))
	output(p) = 0;
      else
	output(p) = spc.at_(p.row() / subsampling_factor, p.col() / subsampling_factor);

    io::pgm::save(output, argv[2]);
  }


  trace::exiting("main");
}
