#ifndef MLN_DRAW_MESH_HH
# define MLN_DRAW_MESH_HH

# include <mln/pw/image.hh>
# include <mln/core/image2d_b.hh>
# include <mln/core/point2d.hh>
# include <mln/level/fill.hh>
# include <mln/draw/line.hh>
# include <mln/core/mesh_p.hh>
# include <mln/core/mesh_image.hh>

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
      mln_dpoint(I) d = p1 - p2;

      std::cout << "dx = "
		<< d[0]
		<< std::endl
		<< "dy = "
		<< d[1]
		<< std::endl;
      if (!d[0])
	{
	  if (d[1])
	    if (d[1] > 0)
	      for (int y = p1[1]; y > p2[1]; --y)
		exact(ima)(make::point2d(p1[0],y)) = link_v;
	    else
	      for (int y = p1[1]; y < p2[1]; ++y)
		exact(ima)(make::point2d(p1[0],y)) = link_v;
	}
      else // d[0] != 0
	{
	  if (!d[1])
	    if (d[0] > 0)
	      for (int x = p1[0]; x > p2[0]; --x)
		exact(ima)(make::point2d(x, p1[1])) = link_v;
	    else
	      for (int x = p1[0]; x < p2[0]; ++x)
		exact(ima)(make::point2d(x, p1[1])) = link_v;
	  else // d[0] != 0 && d[1] != 0
	    {
	      float e = 0;
	      if (d[0] > 0)
		{
		  if (d[1] > 0)
		    {
		      if (d[0] < d[1])
			{
			  // 7eme octant
			  int x = p1[0] - 1;
			  e -= (float)d[1];
			  for (int y = p1[1] - 1; y > p2[1]; --y)
			    {
			      exact(ima)(make::point2d(x,y)) = link_v;
			      e += (float)d[0];
			      if (e >= 0)
				{
				  x--;
				  e -= (float)d[1];
				}
			    }
			}
		      else
			{  
			  // 8eme octant
			  int y = p1[1] - 1;
			  e -= (float)d[0];
			  for (int x = p1[0] - 1; x > p2[0]; --x)
			    {
			      exact(ima)(make::point2d(x,y)) = link_v;
			      e += (float)d[1];
			      if (e > 0)
				{
				  y--;
				  e -= (float)d[0];
				}
			    }
			}
		    }
		  else // d[0] > 0 && d[1] < 0
		    {
		      if (d[0] < -d[1])
			{
			  // 2eme octant
			  int x = p1[0];
			  e += (float)d[1];
			  for (int y = p1[1] + 1; y < p2[1]; ++y)
			    {
			      exact(ima)(make::point2d(x,y)) = link_v;
			      e += (float)d[0];
			      if (e >= 0)
				{
				  x--;
				  e += (float)d[1];
				}
			    }
			}
		      else
			{  
			  // 1er octant
			  int y = p1[1] + 1;
			  e += (float)d[0];
			  for (int x = p1[0] - 1; x > p2[0]; --x)
			    {
			      exact(ima)(make::point2d(x,y)) = link_v;
			      e += (float)d[1];
			      if (e <= 0)
				{
				  y++;
				  e += (float)d[0];
				}
			    }
			}
		    }
		}
	      else // d[0] < 0
		{
		  if (d[1] > 0)
		    {
		      if (-d[0] < d[1])
			{
			  // 6eme octant
			  int x = p1[0] + 1;
			  e += (float)d[1];
			  for (int y = p1[1] - 1; y > p2[1]; --y)
			    {
			      exact(ima)(make::point2d(x,y)) = link_v;
			      e += (float)d[0];
			      if (e < 0)
				{
				  x++;
				  e += (float)d[1];
				}
			    }
			}
		      else
			{
			  // 5eme octant
			  int y = p1[1] - 1;
			  e -= (float)d[0];
			  for (int x = p1[0] + 1; x < p2[0]; ++x)
			    {
			      exact(ima)(make::point2d(x,y)) = link_v;
			      e -= (float)d[1];
			      if (e < 0)
				{
				  y--;
				  e -= (float)d[0];
				}
			    }
			}
		    }
		  else // d[0] < 0 && d[1] < 0
		    {
		      if (-d[0] < -d[1])
			{
			  // 3eme octant
			  int x = p1[0] + 1;
			  e -= (float)d[1];
			  for (int y = p1[1] + 1; y < p2[1]; ++y)
			    {
			      exact(ima)(make::point2d(x,y)) = link_v;
			      e += (float)d[0];
			      if (e < 0)
				{
				  x++;
				  e -= (float)d[1];
				}
			    }
			}
		      else
			{
			  // 4eme octant
			  int y = p1[1] + 1;
			  e -= (float)d[0];
			  for (int x = p1[0] + 1; x < p2[0]; ++x)
			    {
			      exact(ima)(make::point2d(x,y)) = link_v;
			      e += (float)d[1];
			      if (e <= 0)
				{
				  y++;
				  e -= (float)d[0];
				}
			    }
			}
		    }
		}
	    }
	}
    }

    template <typename I, typename P>
    void
    mesh(Image<I>& ima, const mesh_p<P>& m,
	 mln_value(I) node_v,
	 mln_value(I) link_v)
    {
      level::fill(ima, 0);

      for (unsigned i = 0; i < m.gr_.nb_link_; ++i)
	line (exact(ima),
	      m.loc_[m.gr_.links_[i]->node1],
	      m.loc_[m.gr_.links_[i]->node2],
	      link_v);

      for (unsigned i = 0; i < m.gr_.nb_node_; ++i)
 	exact(ima)(m.loc_[i]) = node_v;
    }

    template <typename I,  typename P, typename V>
    void
    mesh(Image<I>& ima, const mesh_image<P, V>& mesh)
    {
      level::fill(ima, 0);

      for (unsigned i = 0; i < mesh.domain().gr_.nb_link_; ++i)
	line (exact(ima),
	      mesh.domain().loc_[mesh.domain().gr_.links_[i]->node1],
	      mesh.domain().loc_[mesh.domain().gr_.links_[i]->node2],
	      1);

      for (unsigned i = 0; i < mesh.domain().gr_.nb_node_; ++i)
 	exact(ima)(mesh.domain().loc_[i]) = mesh.data_values ()[i];
    }
    
   } // end of draw

} // end of mln

#endif // MLN_MESH_PSITE_HH
