
#include <iterator>
#include <iostream>
#include <algorithm>

#include <mln/core/image/image2d.hh>

#include <mln/core/neighb2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/level/transform.hh>

#include <mln/convert/to_window.hh>
#include <mln/convert/to_image.hh>
#include <mln/convert/to_fun.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/accu/mean.hh>
#include <mln/level/take.hh>

#include <mln/util/graph.hh>

#include <mln/debug/println.hh>


mln::value::int_u8 foo(unsigned u)
{
  return u == 0 ?
    0 : // wshed line
    1 + (u - 1) % 255; // basin
}


namespace mln
{

  template <typename I>
  void doit(const I& irm,
	    const image2d<unsigned>& lbl,
	    unsigned nlabels)
  {
    {
//     accu::mean_<unsigned> m;

//     mln_piter(I) p(irm.domain());
//     for_all(p)
//       if (lbl(p) == 82)
// 	m.take(irm(p));

//     level::take(irm | (pw::value(lbl) == pw::cst(82)), m);

//     std::cout << "reg 82 has mean = " << m << std::endl;
    }

    std::vector< accu::mean_<unsigned> > m(nlabels + 1);
    mln_piter(I) p(irm.domain());
    for_all(p)
      m[lbl(p)].take(irm(p));
    
//     for (unsigned l = 1; l <= nlabels; ++l)
//       std::cout << l << ":" << m[l] << "  ";
//     std::cout << std::endl;

    accu::mean_<unsigned> mm;
    mm.take(m[70]);
    mm.take(m[77]);
    mm.take(m[80]);
    mm.take(m[82]);
    mm.take(m[99]);
    std::cout << mm.to_result() << std::endl;
  }


  /*template <typename I, typename N>
  void mk_graph(const I& lbl,
		N nbh,
		unsigned nlabels)
  {
    std::vector< std::vector<bool> > adja(nlabels + 1);
    for (unsigned l = 1; l <= nlabels; ++l)
      adja[l].resize(l, false);

    mln_piter(I) p(lbl.domain());
    mln_niter(N) n(nbh, p);

    // We compute the adjacency matrix of the RAG.
    for_all(p)
      if (lbl(p) == 0) // wshed
	{
	  std::set<unsigned> s;
	  for_all(n) if (lbl.has(n))
	    {
	      if (lbl(n) == 0)
		continue;
	      s.insert(lbl(n));
	    }
	  if (s.size() < 2)
	    {
	      std::cout << "#" << std::endl;
	      continue;
	    }
	  std::set<unsigned>::const_iterator l1, l2;
	  for (l1 = s.begin(); l1 != s.end(); ++l1)
	    {
	      l2 = l1;
	      for (++l2; l2 != s.end(); ++l2)
		adja[*l2][*l1] = true;
	    }
	}

    unsigned count = 0;
    for (unsigned l1 = 2; l1 <= nlabels; ++l1)
      for (unsigned l2 = 1; l2 < l1; ++l2)
	if (adja[l1][l2])
	  ++count;
    std::cout << "link count = " << count << std::endl;

    // Graph.
    util::graph<> g;
    // Nodes.
    for (unsigned i = 0; i <= nlabels; ++i)
      g.add_node();
    // Edges.
    for (unsigned l1 = 1; l1 <= nlabels; ++l1)
      for (unsigned l2 = l1 + 1; l2 <= nlabels; ++l2)
	if (adja[l1][l2])
	  g.add_edge(l1, l2);
    g.print_debug (std::cout);

  }*/

} // end of namespace mln


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> irm;
  io::pgm::load(irm, "./+irm6cut.pgm");

//   io::pgm::save( morpho::gradient(irm, win::rectangle2d(3,3)),
//                  "tmp_grad_3x3.pgm" );

  window2d c4p = convert::to_window(c4());
  c4p.insert(0,0);
//   debug::println(convert::to_image(c4p));
  image2d<int_u8> grad = morpho::gradient(irm, c4p);
  io::pgm::save( grad, "tmp_grad_c4p.pgm" );

  image2d<int_u8> clo;
  initialize(clo, irm);

  morpho::closing_area(grad, c4(), 100, clo);
  io::pgm::save( clo, "tmp_clo_a100.pgm" );

  unsigned nbasins;
  image2d<unsigned> wshed = morpho::meyer_wst(clo, c4(), nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;
  io::pgm::save( level::transform(wshed, convert::to_fun(foo)),
		 "tmp_wshed.pgm" );

//   doit(irm, wshed, nbasins);
//   mk_graph(wshed, c4(), nbasins);

//   std::vector< accu::mean_<unsigned> >

}
