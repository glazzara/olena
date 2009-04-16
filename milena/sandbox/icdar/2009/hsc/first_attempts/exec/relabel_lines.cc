#include "../exec/filetype.hh"

#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/dpoint2d.hh>

#include <mln/value/label_8.hh>
#include <mln/core/image/image_if.hh>

#include <mln/data/fill.hh>
#include <mln/pw/all.hh>
#include <mln/labeling/blobs.hh>
// #include <mln/labeling/compute.hh>
#include <mln/accu/maj_h.hh>

#include <mln/morpho/elementary/dilation.hh>
#include <mln/debug/colorize.hh>

#include <mln/transform/influence_zone_geodesic.hh>
#include <mln/level/transform.hh>
#include <mln/fun/l2l/wrap.hh>


namespace mln
{

  typedef value::int_u8 L;

  enum { max_L = 256 - 1 };
  
  typedef L              par_t[max_L];
  typedef std::vector<L> labels_t[max_L];

  
  L find_root(par_t& par, L l)
  {
    if (par[l] == l)
      return l;
    return par[l] = find_root(par, par[l]);
  }

  
  image2d<L> doit(const image2d<L>& ws_txt,
		  const image2d<L>& ws_spc)
  {
    labels_t left_, right_;

//     image2d<bool> cross(ws_txt.domain());
//     data::fill(cross, false);

    box2d b = ws_txt.domain();
    mln_piter_(box2d) p(b);
    for_all(p)
    {
      if (ws_txt(p) != 0 || ws_spc(p) != 0)
	continue;
      if (ws_txt(p + left) == 0 && ws_txt(p + right) == 0
	  // text = locally horizontal:
	  // o o o
	  &&
	  ws_spc(p + left) != 0 && ws_spc(p + right) != 0
	  // region frontiere = locally roughly vertical:
	  //   x .
	  //   o
	  // . x
	  )
	{
	  L l1 = ws_spc(p + left),
	    l2 = ws_spc(p + right);
	  if (l1 == 0 || l2 == 0)
	    continue;
	  if (l2 == l1)
	    continue;
	  right_[l1].push_back(l2);
	  left_[l2].push_back(l1);
// 	  cross(p) = true;
	}
    }

//     io::pbm::save(cross, "tmp_cross.pbm");


    // debug code:

//     for (L l = 0; l < max_L; ++l)
//       if (right_[l].size())
// 	{
// 	  std::cout << l << " -> ";
// 	  const std::vector<L>& lab = right_[l];
// 	  for (L i = 0; i < lab.size(); ++i)
// 	    {
// 	      std::cout << lab[i] << ' ';
// 	      const std::vector<L>& rev_lab = left_[ lab[i] ];
// 	      if (rev_lab.size())
// 		{
// 		  std::cout << '(';
// 		  for (L j = 0; j < rev_lab.size(); ++j)
// 		    std::cout << rev_lab[j] << ',';
// 		  std::cout << ')';
// 		}
// 	    }
// 	  std::cout << std::endl;
// 	}


    par_t par;
    for (L l = 0; l < max_L; ++l)
      par[l] = l;

    for (L l = 1; l < max_L; ++l)
      if (right_[l].size() == 1)
	{
	  L l2 = right_[l][0];
	  mln_invariant(l2 != 0);
	  if (left_[l2].size() != 0 && left_[l2][0] == l)
	    {
	      mln_invariant(l != 0 && l2 != l);
	      // Union.
	      par[l] = l2; // l --right--> l2
	    }
	}

    for (L l = 1; l < max_L; ++l)
      par[l] = find_root(par, l);

//     // debug code:

//     for (L l = 1; l < max_L; ++l)
//       if (par[l] != l)
// 	std::cout << l << " -> " << par[l] << "   ";
//     std::cout << std::endl;


    image2d<L> out(b);
    for_all(p)
      out(p) = par[ ws_spc(p) ];

    return out;
  }


  void left_iz(const image2d<bool>& ws)
  {
    using value::int_u8;
    image2d<int_u8> ima(ws.domain());
    data::fill(ima, 0);
    unsigned nrows = ima.nrows();
    int_u8 l = 0;
    for (unsigned row = 0; row < nrows; ++row)
      if (ws.at_(row, 0) == true)
	ima.at_(row, 0) = ++l;
    io::pgm::save(ima, "tmp_iz_start.pgm");

    data::paste( transform::influence_zone_geodesic(ima | pw::value(ws),
						    c8(),
						    mln_max(unsigned)),
		 ima );

    io::pgm::save(ima, "tmp_iz.pgm");
  }



  image2d<L> doit_cut(const image2d<L>& ws_txt,
		      const image2d<L>& ws_spc)
  {

    box2d b = ws_txt.domain();

    image2d<bool> lines(b);
    data::fill(lines, (pw::value(ws_txt) == pw::cst(0)) | b);

    left_iz(lines); // THIS IS A TEST!


    //debug:
    io::pbm::save(lines, "tmp_ws_line.pbm");

    mln_piter_(box2d) p(b);
    for_all(p)
      if (ws_txt(p)        == 0 &&
	  ws_txt(p + up)   == 0 &&
	  ws_txt(p + down) == 0)
	lines(p) = false;

    //debug:
    io::pbm::save(lines, "tmp_lines.pbm");

    typedef value::int_u<12> L2;
    L2 n_lines; 
    image2d<L2> line_lab = labeling::blobs(lines, c8(), n_lines);

    {
      io::pgm::save(level::transform(line_lab,
				     fun::l2l::wrap<value::int_u8>()),
		    "tmp_lines.pgm");
    }

    // Cannot be used with int_u* (only with label_*):
    //     util::array<L2> arr = labeling::compute(accu::maj_h<L2>(),
    // 					    line_lab | (pw::value(line_lab) != pw::cst(0)),
    // 					    ws_spc,
    // 					    max_L);
    
    util::array< accu::maj_h<L2> > arr(max_L + 1); // L -> L2
    for_all(p)
    {
      if (line_lab(p) == 0) // not on a line
	continue;
      L l = ws_spc(p); // l = label of a region
      if (l == 0)
	continue;
      arr[l].take(line_lab(p));
    }

    util::array<L2> arr_(max_L); // L -> L2
    for (L l = 1; l < max_L; ++l)
      arr_[l] = arr[l].to_result();

//     // debug:
//     for (L l = 1; l < max_L; ++l)
//       std::cout << l << " -> " << arr_[l] << "   ";
//     std::cout << std::endl;

    L2 l2_max = 0;
    for (L l = 1; l < max_L; ++l)
      if (arr_[l] > l2_max)
	l2_max = arr_[l];

    for (L l = 1; l < max_L; ++l)
      if (arr_[l] == 0)
	arr_[l] = ++l2_max;

//     // debug:
//     std::cout << std::endl << std::endl;
//     for (L l = 1; l < max_L; ++l)
//       std::cout << l << " -> " << arr_[l] << "   ";
//     std::cout << std::endl;

    util::array<L> newl(l2_max + 1);
    for (unsigned i = 0; i <= l2_max; ++i)
      newl[i] = 0;

    L cur = 1;
    for (L l = 1; l < max_L; ++l)
      {
	L2 l2 = arr_[l];
	if (newl[l2] == 0)
	  newl[l2] = cur++;
      }
    
    image2d<L> out(b);
    for_all(p)
      if (ws_spc(p) == 0)
	out(p) = 0;
      else
	out(p) = newl[ arr_[ws_spc(p)] ];

    out = morpho::elementary::dilation(out, c4()); // c2_row

    return out;
  }


} // ! mln


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " ws_txt.pgm ws_spc.pgm output.ppm" << std::endl
	    << "  Label lines." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  trace::entering("main");

  image2d<L> ws_txt, ws_spc;
  io::pgm::load(ws_txt, argv[1]);
  io::pgm::load(ws_spc, argv[2]);

  image2d<L> out = doit_cut(ws_txt, ws_spc);

  io::ppm::save(debug::colorize(value::rgb8(), out, 254),
		argv[3]);

  trace::exiting("main");
}
