
#include <iterator>
#include <iostream>
#include <algorithm>

#include <mln/core/image2d.hh>

#include <mln/core/neighb2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/image_if.hh>
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
//  using value::int_u8;
  template <typename N>
  void segm_to_pregraph(const image2d<unsigned>& lbl, 
                        const N& nbh, 
                        int nbasins)
  {
    mln_piter(image2d<unsigned>) p(lbl.domain());
    mln_niter(N) n(nbh, p);
    
//    typedef metal::vec<3,mln_value(I)> V;
    
//    typedef Image<V> J;
    
    image2d<unsigned> retour;
    initialize(retour, lbl);
    
    int n_seuil = ((nbasins/10)+1)*10;
    
    std::cout << "Nbasins = " << nbasins << std::endl
              << "n_seuil = " << n_seuil << std::endl;
     
    for_all(p)
    {
      if(lbl(p) == 0)
      {
        std::set<unsigned> s;  //Set of neighbouring values of p
        for_all(n) if (lbl.has(n))
        {
          if (lbl(n) == 0)
            continue;
          s.insert(lbl(n));
        }
        if (s.size() < 2 )
          std::cout << "#" << std::endl;
        if (s.size() == 2 ) 
        {
        	std::set<unsigned>::const_iterator l1, l2;
          l1 = s.begin();
          l2 = s.begin();
          l2++;
          
          retour(p) = *l1 * n_seuil + *l2;

        }
        if (s.size() > 2 )
          retour(p) = 0;        
      }
      else
      {
       // retour(p) = make::vec(0,0,lbl(p));
        retour(p) = lbl(p);
      }
    }  
    //io::pgm::save(retour, "tmp_pregraphe.pgm");
    io::pgm::save( level::transform(retour, convert::to_fun(foo)),
		 "tmp_pregraph.pgm" );
    //return retour;
  }               
                  
   
  
  template <typename I>
  void puzzle(const I& irm,
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
    
    I carte;
    initialize(carte, irm);
    
    for_all(p)
    {
      carte(p) = m[lbl(p)].to_result();
    }
   
     
    io::pgm::save(carte, "tmp_puzzle.pgm");

//    accu::mean_<unsigned> mm;
//    mm.take(m[70]);
//    mm.take(m[77]);
//    mm.take(m[80]);
//    mm.take(m[82]);
//    mm.take(m[99]);
//    std::cout << mm.to_result() << std::endl;

  }


  template <typename I, typename J, typename N>
  void mk_graph(const I& lbl,
  		const J& irm,
		N nbh,
		unsigned nlabels)
  {
    std::vector< std::vector<bool> > adja(nlabels + 1);
    for (unsigned l = 0; l <= nlabels; ++l)
      adja[l].resize(nlabels + 1, false);

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
		adja[*l1][*l2] = true;
	    }
	}

    unsigned count = 0;
    for (unsigned l1 = 1; l1 <= nlabels; ++l1)
      for (unsigned l2 = 1; l2 <= nlabels; ++l2)
	if (adja[l1][l2])
	  ++count;
    std::cout << "link count = " << count << std::endl;

    // Graph.
    util::graph<float> g;
    // Nodes.
    for (unsigned i = 0; i <= nlabels; ++i)
    {
      accu::mean_<unsigned> m;
      level::take(irm | (pw::value(lbl) == pw::cst(i)), m);
      g.add_node(m.to_result());
    }
    // Edges.
    for (unsigned l1 = 1; l1 <= nlabels; ++l1)
      for (unsigned l2 = l1 + 1; l2 <= nlabels; ++l2)
	if (adja[l1][l2])
	  g.add_edge(l1, l2);
    //g.print_debug (std::cout);
    for( int i = 0 ; i < g.nnodes() ; ++i)
      std::cout << "[" << i << " : " << g.node_data(i) << " ] ";
    std::cout << std::endl;
  }

} // end of namespace mln


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> irm;
  io::pgm::load(irm, "./+irm6.pgm");

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

   puzzle(irm, wshed, nbasins);
   
//   mk_graph(wshed, irm, c4(), nbasins);

//   std::vector< accu::mean_<unsigned> >
  segm_to_pregraph(wshed, c8(), nbasins);
}
