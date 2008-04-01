#include <mln/level/sort_psites.hh>
#include <mln/level/fill.hh>
#include <mln/core/image2d.hh>

namespace mln
{

  namespace morpho
  {

    template <class I, class N>
    struct basic_najman
    {

      typedef mln_psite(I) psite;

      struct node {
	int level;
	int area;
	int highest;
	p_array<mln_psite(I)> children;

	void addChildren(const node& n)
	{
	  typename p_array<mln_psite(I)>::fwd_piter it(n.children);
	  for (it.start();
	       it.is_valid();
	       it.next())
	    children.append(it.to_psite());
	}
	void addChild(const psite n)
	{
	  children.append(n);
	}
      }; // struct node


      const Image<I>& ima;
      const Neighborhood<N>& nbh;
      mln_ch_value(I, psite) par;

      mln_ch_value(I, psite) Par_tree;
      // Par_node is handled by par (from ap_maxtree)
      mln_ch_value(I, int) Rnk_tree;
      mln_ch_value(I, int) Rnk_node;
      mln_ch_value(I, psite) lowestNode;
      mln_ch_value(I, node) nodes;
      mln_ch_value(I, bool) isproc;
      psite Root;
      p_array<mln_psite(I)> S;
      // std::map<psite,psite> M; // to keep the root of any point.

      basic_najman(const Image<I>& i,
		   const Neighborhood<N>& n)
	: ima(i),
	  nbh(n),
	  par(exact(i).domain(), exact(i).border()),
	  Par_tree(exact(i).domain(), exact(i).border()),
	  Rnk_tree(exact(i).domain(), exact(i).border()),
	  Rnk_node(exact(i).domain(), exact(i).border()),
	  lowestNode(exact(i).domain(), exact(i).border()),
	  nodes(exact(i).domain(), exact(i).border()),
	  isproc(exact(i).domain(), exact(i).border())
      {
      }


      void MakeSet_tree(psite x)
      {
	Par_tree(x) = x;
	Rnk_tree(x) = 0;
      }

      void MakeSet_node(psite x)
      {
	par(x) = x; // was Par_node(x) = x;
	Rnk_node(x) = 0;
      }

      psite Find_tree(psite x)
      {
	if (Par_tree(x) != x)
	  Par_tree(x) = Find_tree(Par_tree(x));
	return Par_tree(x);
      }

      void swap(psite& x, psite& y)
      {
	psite memo = x;
	x = y;
	y = memo;
      }

      bool is_root(const psite& x) const
      {
	return par(x) == x;
      }

      bool is_level_root(const psite& x) const
      {
	return is_root(x) || exact(ima)(par(x)) < exact(ima)(x);
      }

      psite find_level_root(const psite& x)
      {
	if (is_level_root(x))
	  return x;
	else
	  return par(x) = find_level_root(par(x));
      }

      psite Find_node(psite x)
      {
	if (par(x) != x)
	  par(x) = Find_node(par(x));
	return par(x);
      }

      void go()
      {
	init();
	BuildComponentTree();
      }


      void init()
      {
	// Sort the points in increasing order
	S = level::sort_psites_increasing(ima);

	// Clear the marker map
	level::fill(isproc, false);
	for (int ip = 0; ip < int(S.npoints()); ++ip)
	  {
	    psite p = S[ip];
	    MakeSet_node(p);
	    MakeSet_tree(p);
	    // 	      if (lowestNode.hold(p))
	    lowestNode(p) = p;
	    // 	      if (nodes.hold(p))
	    nodes(p) = MakeNode(exact(ima)(p));
	  }
      }


      void BuildComponentTree()
      {
	for (int ip = 0; ip < int(S.npoints()); ++ip)
	  {
	    psite p = S[ip];
	    isproc(p) = true;

	    psite curTree = Find_tree(p);
	    psite curNode = Find_node(lowestNode(curTree));

	    mln_niter(N) q(nbh, p);

	    for_all(q)
	      if (exact(ima).has(q) and isproc(q) and exact(ima)(q) >= exact(ima)(p)) // f[q] >= f[p] parce qu'on prend l'ordre de l'histogramme
		{
		  psite adjTree = Find_tree(q);
		  psite adjNode = Find_node(lowestNode(adjTree));
		  if (curNode != adjNode)
		    {
		      if (nodes(curNode).level == nodes(adjNode).level)
			curNode = MergeNode(adjNode, curNode);
		      else
			{
			  // we have nodes[curNode].level < nodes[adjNode].level
			  nodes(curNode).addChild(adjNode);
			  //apparemment NON :Par_node[adjNode] = curNode; // car adjNode devient fils de curNode
			  nodes(curNode).area += nodes(adjNode).area;
			  nodes(curNode).highest += nodes(adjNode).highest;
			}
		    }
		  curTree = Link_tree(adjTree, curTree);
		  lowestNode(curTree) = curNode;
		}
	  }
	Root = lowestNode(Find_tree(Find_node(psite(0, 0))));
	// Pour garder une map de correspondance point <-> local_root
	//            for (int ip = 0; ip < int(S.size()); ++ip)
	//              {
	//                psite p = S[ip];
	// 	       M(p) = Find_node(p);
	// 	     }

	mln_piter(I) r(par.domain());
	for_all(r)
	  par(r) = Find_node(r);
      }


      psite MergeNode(psite& node1, psite& node2)
      {
	psite tmpNode = Link_node(node1, node2);
	psite tmpNode2;
	if (tmpNode == node2)
	  {
	    nodes(node2).addChildren(nodes(node1));
	    tmpNode2 = node1;
	  }
	else
	  {
	    nodes(node1).addChildren(nodes(node2));
	    tmpNode2 = node2;
	  }
	nodes(tmpNode).area += nodes(tmpNode2).area;
	nodes(tmpNode).highest += nodes(tmpNode2).highest;
	return tmpNode;
      }

      psite Link_tree(psite x, psite y)
      {
	if (Rnk_tree(x) > Rnk_tree(y))
	  swap(x, y);
	else
	  if (Rnk_tree(x) == Rnk_tree(y))
	    Rnk_tree(y) += 1;
	Par_tree(x) = y;
	return y;
      }

      psite Link_node(psite x, psite y)
      {
	if (Rnk_node(x) > Rnk_node(y))
	  swap(x, y);
	else
	  if (Rnk_node(x) == Rnk_node(y))
	    Rnk_node(y) += 1;
	par(x) = y; // was Par_node(x) = y;
	return y;
      }

      node MakeNode(int level)
      {
	node n;
	n.level = level;
	n.area = 1;
	n.highest = level;
	return n;
      }

      template <typename C>
      void image_output(image2d<C>& img)
      {
	for(unsigned int i = 0; i < img.domain().len(0); ++i)
	  {
	    for(unsigned int j = 0; j < img.domain().len(1); ++j)
	      {
		C l = (img(psite(i, j)));//.row() * img.domain().len(1) + (img(psite(i, j))).col();
		std::cout << " " << l << " ";
	      }
	    std::cout << std::endl;
	  }
      }



      void print_tree(psite p)
      {
	node& n = nodes(p);
	std::cout << "psite " << p << "=" << (p.row() * exact(lowestNode).domain().len(1) + p.col()) << " : ";

	typename p_array<mln_psite(I)>::fwd_piter it(n.children);
	for (it.start();
	     it.is_valid();
	     it.next())
	  {
	    psite q = it.to_psite();
	    std::cout << q << "=" << (q.row() * lowestNode.domain().len(2) + q.col()) << " ";
	  }
	std::cout << std::endl;

	for (it.start();
	     it.is_valid();
	     it.next())
	  {
	    print_tree(it.to_psite());
	  }
      }

    }; // struct basic_najman

  }; // namespace morpho

}; // namespace mln
