#ifndef MLN_DRAW_MESH_HH
# define MLN_DRAW_MESH_HH

# include <mln/pw/image.hh>
#include <mln/core/image2d_b.hh>
# include <mln/core/point2d.hh>
//# include <mln/pw/cst.hh>
# include <mln/level/fill.hh>
//# include <mln/metal/is_a.hh>
# include "mesh_p.hh"

namespace mln
{
  namespace draw
  {
    template <typename I>
    void
    bresenham(Image<I>& ima,
	     point2d p1,
	     point2d p2,
	     mln_value(I) link_v)
    {

    }

    template <typename I, typename P>
    void
    mesh(Image<I>& ima, const Mesh_p<P>& m,
	 mln_value(I) node_v,
	 mln_value(I) link_v)
    {
      level::fill(ima, 0);

      for (unsigned i = 0; i < m.gr_.nb_link_; ++i)
	bresenham (exact(ima),
		  m.loc_[m.gr_.links_[i]->node1],
		  m.loc_[m.gr_.links_[i]->node2],
		  link_v);

      for (unsigned i = 0; i < m.gr_.nb_node_; ++i)
 	exact(ima)(m.loc_[i]) = node_v;
    }

  } // end of draw

} // end of mln

#endif // MLN_MESH_PSITE_HH
