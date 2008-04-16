#include <mln/level/sort_psites.hh>
#include <mln/level/fill.hh>
#include <mln/core/image2d.hh>
#include <mln/core/p_set.hh>
//#include <mln/util/greater_psite.hh>
#include <queue>
#include <set>

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

      I pima;
      const Image<I>& ima;
      const Neighborhood<N>& nbh;
      mln_ch_value(I, psite) Par_node;
      mln_ch_value(I, psite) Par_tree;

      mln_ch_value(I, int) Rnk_tree;
      mln_ch_value(I, int) Rnk_node;
      mln_ch_value(I, psite) subtreeRoot;
      mln_ch_value(I, node) nodes;
      mln_ch_value(I, bool) isproc;
      psite Root;
      p_array<mln_psite(I)> S;
      // std::map<psite,psite> M; // to keep the root of any point.

      basic_najman(const Image<I>& i,
		   const Neighborhood<N>& n)
	: pima(exact(i)),
	  ima(i),
	  nbh(n),
	  Par_node(exact(i).domain(), exact(i).border()),
	  Par_tree(exact(i).domain(), exact(i).border()),
	  Rnk_tree(exact(i).domain(), exact(i).border()),
	  Rnk_node(exact(i).domain(), exact(i).border()),
	  subtreeRoot(exact(i).domain(), exact(i).border()),
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
	Par_node(x) = x;
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

      psite Find_node(psite x)
      {
	if (Par_node(x) != x)
	  Par_node(x) = Find_node(Par_node(x));
	return Par_node(x);
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
	    // 	      if (subtreeRoot.hold(p))
	    subtreeRoot(p) = p;
	    // 	      if (nodes.hold(p))
	    nodes(p) = MakeNode(exact(ima)(p));
	  }
      }


      void BuildComponentTree()
      {
	for (int ip = 0; ip < int(S.npoints()); ++ip)
	  {
	    psite p = S[ip];

	    psite curTree = Find_tree(p);
	    psite curNode = Find_node(subtreeRoot(curTree));

	    mln_niter(N) q(nbh, p);

	    for_all(q)
	      if (exact(ima).has(q) and isproc(q) and exact(ima)(q) <= exact(ima)(p))
		{
		  psite adjTree = Find_tree(q);
		  psite adjNode = Find_node(subtreeRoot(adjTree));
		  if (curNode != adjNode)
		    {
		      if (nodes(curNode).level == nodes(adjNode).level)
			{
			  curNode = MergeNode(adjNode, curNode);
			}
		      else
			{
			  nodes(curNode).addChild(adjNode);
			  nodes(curNode).area += nodes(adjNode).area;
			  nodes(curNode).highest += nodes(adjNode).highest;
			}
		    }
		  curTree = Link_tree(adjTree, curTree);
		  subtreeRoot(curTree) = curNode;
		}
	    isproc(p) = true;
	  }
	// Pour garder une map de correspondance point <-> local_root
	//            for (int ip = 0; ip < int(S.size()); ++ip)
	//              {
	//                psite p = S[ip];
	// 	       M(p) = Find_node(p);
	// 	     }

	mln_piter(I) r(Par_node.domain());
	for_all(r)
	  Par_node(r) = Find_node(r);

	Root = subtreeRoot(Find_tree(Find_node(psite(0, 0))));
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
	Par_node(x) = y;
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
	std::cout << "psite " << p << "(" << n.level << ")=" << (p.row() * exact(subtreeRoot).domain().len(1) + p.col()) << " : ";

	typename p_array<mln_psite(I)>::fwd_piter it(n.children);
	for_all(it)
	  {
	    psite q = it.to_psite();
	    std::cout << q << "=" << (q.row() * subtreeRoot.domain().len(1) + q.col()) << " ";
	  }
	std::cout << std::endl;

	for_all(it)
	  {
	    print_tree(it.to_psite());
	  }
      }

      psite lca (psite a, psite b)
      {
	return lca_rec(Root, Par_node(a), Par_node(b));
      }

      psite lca_rec (psite cur, psite a, psite b)
      {
	// FIXME : naive implementation, make something better

	if (cur == a)
	  return a;

	if (cur == b)
	  return b;

	if (nodes(cur).children.npoints() == 0)
	  return psite (-1, -1);

	psite tmp, acc = psite(-1, -1);
	int n = 0;
	typename p_array<mln_psite(I)>::fwd_piter it(nodes(cur).children);
	for_all(it)
	{
	  tmp = lca_rec(it.to_psite(), a, b);
	  if (tmp != psite(-1, -1))
	    {
	      if (tmp == a)
		{
		  acc = tmp;
		  n++;
		  continue;
		}
	      if (tmp == b)
		{
		  acc = tmp;
		  n++;
		  continue;
		}
	      return tmp;
	    }
	}
	if (n == 2)
	  return cur;

	return acc;
      }

      psite min (p_set<psite>& components)
      {
	if (components.npoints() == 0)
	  return psite(-1, -1);

	typename p_set<psite>::fwd_piter it(components);
	psite min = components[0];

	for_all(it)
	  if (nodes(it.to_point()).level > nodes(min).level)
	    min = it.to_point();

	return min;
      }

      psite highest_fork (p_set<psite>& components)
      {
	if (components.npoints() == 0)
	  return psite(-1, -1);

	psite hfork = components[0], min = hfork;
	typename p_set<psite>::fwd_piter it(components);

	for_all(it)
	  hfork = lca(hfork, it.to_point());

	if (nodes(min).level == nodes(hfork).level)
	 return psite(-1, -1);

	return hfork;
      }

      psite w_destructible(psite p)
      {
	mln_niter(N) q(nbh, p);
	p_set<psite> v;

	for_all(q)
	  if (exact(ima)(q) < exact(ima)(p))
	    v.append(Par_node(q));

	if (v.npoints() == 0)
	  return psite(-1, -1);

	psite hf = highest_fork(v);

	if (hf == psite(-1, -1))
	  return min(v);

	if (nodes(hf).level <= exact(ima)(p))
	  return hf;

	return psite(-1, -1);
      }

      psite m_destructible(psite p)
      {
	mln_niter(N) q(nbh, p);
	p_set<psite> v;

	for_all(q)
	  if (exact(ima)(q) < exact(ima)(p))
	    v.insert(Par_node(q));

	if (v.npoints() == 0)
	  return psite(-1, -1);

	if (nodes(min(v)).children.npoints() != 0)
	  return (psite(-1, -1));

	psite hf = highest_fork(v);

	if (hf == psite(-1, -1))
	  return min(v);

	return psite(-1, -1);
      }

      void m_watershed ()
      {

	// FIXME : make a better priority function
	//	typedef
	//	  std::priority_queue< psite, std::vector<psite>, util::greater_psite<I> >
	//	  ordered_queue_type;


	std::priority_queue<psite> l;

	// Clear the marker map
	level::fill(isproc, false);
	mln_piter(I) it(exact(ima).domain());

	for_all(it)
	  if (m_destructible(it.to_point()) != psite(-1, -1))
	    {
	      l.push(it.to_point());
	      isproc(it.to_point()) = true;
	    }

	psite p, i;
	while (!l.empty())
	  {
	    p = l.top();
	    l.pop();
	    isproc(p) = false;

	    i = m_destructible(p);

	    if (i != psite(-1, -1))
	      {
		pima(p) = nodes(i).level ;
		Par_node(p) = i;
		mln_niter(N) q(nbh, p);

		for_all(q)
		  if (!isproc(q))
		    if (m_destructible(q) != psite(-1, -1))
		      {
			l.push(q);
			isproc(q) = true;
		      }
	      }
	  }
      }

      void w_watershed()
      {
	p_array< p_set<psite> > L;
	// TODO : replace int with the type of value
	mln_ch_value(I,  int) K;
	mln_ch_value(I,  psite) H;

	mln_piter(I) it(exact(ima).domain());

	psite i;
	for_all(it)
	{
	  psite p = it.to_point();

	  i = w_destructible(p);
	  if (i != psite(-1, -1))
	    {
	      L[nodes(i).level].insert(p);
	      K(p) = nodes(i).level;
	      H(p) = i;

	      typename p_array< p_set<psite> >::fwd_piter n(L);
	      for_all(n)
	      {
		while (!n.empty())
		  {
		    psite p = *n.begin();
		    n.erase(p);
		    // TODO : replace int with the type of value
		    int k = nodes(p).level + 1;
		    if (K(p) == k)
		      {
			pima(p) = k;
			Par_node(p) = H(p);

			mln_niter(N) q(nbh, p);
			for_all(q)
			  if (k < pima(q))
			    {
			      psite j = w_destructible(q);
			      if (j != psite(-1, -1))
				K(q) = -1;
			      else
				if (K(q) != nodes(j).level)
				  {
				    n.insert(q);
				    K(q) = i-1;
				    H(q) = j;
				  }
			    }
		      }
		  }
	      }
	    }
	}

      }


    }; // struct basic_najman

  }; // namespace morpho

}; // namespace mln
