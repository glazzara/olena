// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.


#include <mln/data/sort_psites.hh>
#include <mln/data/fill.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/estim/min_max.hh>
#include <mln/math/sqr.hh>
#include <mln/util/greater_psite.hh>
#include <mln/util/ord.hh>

#include <mln/core/site_set/p_priority.hh>
#include <mln/core/site_set/p_queue_fast.hh>

#include <queue>
#include <vector>
#include <map>

namespace mln
{
  namespace morpho
  {

    template <class I, class N>
    struct basic_najman
    {

      typedef mln_site(I) site;

      struct node {
	mln_value(I) level;
	int area;
	int highest;
	typedef mln_site(I) site;
	// Can't modify the sites in a p_array
	// p_array<mln_site(I)> children;
	std::vector<site> children;

	void addChildren(const node& n)
	{
	  // typename p_array<mln_site(I)>::fwd_piter it(n.children);
	  // for (it.start();
	  //      it.is_valid();
	  //      it.next())
	  //  children.append(it.to_site());
	  for (unsigned i=0; i < n.children.size(); ++i)
	    children.push_back(n.children[i]);
	}

	void addChild(const site p)
	{
	  // children.append(n);
	  children.push_back(p);
	}
      }; // struct node

      mln_exact(I) pima;
      const mln_exact(N)& nbh;
      mln_ch_value(I, site) Par_node;
      mln_ch_value(I, site) Par_tree;

      mln_ch_value(I, int) Rnk_tree;
      mln_ch_value(I, int) Rnk_node;
      mln_ch_value(I, site) subtreeRoot;
      mln_ch_value(I, node) nodes;
      mln_ch_value(I, bool) isproc;

      site Root;
      p_array<mln_site(I)> S;
      // std::map<site,site> M; // to keep the root of any site.

      // Ctor
      basic_najman(const Image<I>& i,
		   const Neighborhood<N>& n)
	: pima(exact(i)),
	  nbh(exact(n)),
	  Par_node(exact(i).domain(), exact(i).border()),
	  Par_tree(exact(i).domain(), exact(i).border()),
	  Rnk_tree(exact(i).domain(), exact(i).border()),
	  Rnk_node(exact(i).domain(), exact(i).border()),
	  subtreeRoot(exact(i).domain(), exact(i).border()),
	  nodes(exact(i).domain(), exact(i).border()),
	  isproc(exact(i).domain(), exact(i).border())
      {
      }

      void MakeSet_tree(site x)
      {
	Par_tree(x) = x;
	Rnk_tree(x) = 0;
      }

      void MakeSet_node(site x)
      {
	Par_node(x) = x;
	Rnk_node(x) = 0;
      }

      site Find_tree(site x)
      {
	if (Par_tree(x) != x)
	  Par_tree(x) = Find_tree(Par_tree(x));
	return Par_tree(x);
      }

      void swap(site& x, site& y)
      {
	site memo = x;
	x = y;
	y = memo;
      }

      site Find_node(site x)
      {
	if (Par_node(x) != x)
	  Par_node(x) = Find_node(Par_node(x));
	return Par_node(x);
      }

      void go()
      {
	init();
	BuildComponentTree();
	compressTree();
	build_euler_tour();
	build_minim();
	m_watershed();
	w_watershed();
      }


      void init()
      {
	// Sort the sites in increasing order
	S = data::sort_psites_increasing(pima);

	// Clear the marker map
	data::fill(isproc, false);
	for (int ip = 0; ip < int(S.nsites()); ++ip)
	  {
	    site p = S[ip];
	    MakeSet_node(p);
	    MakeSet_tree(p);
	    // 	      if (subtreeRoot.hold(p))
	    subtreeRoot(p) = p;
	    // 	      if (nodes.hold(p))
	    nodes(p) = MakeNode(pima(p));
	  }
      }


      void BuildComponentTree()
      {
	typename p_array<mln_site(I)>::fwd_piter ip(S);
	for_all(ip)
	{
	  site p = ip.to_site();

	  site curCanonicalElt = Find_tree(p);
	  site curNode = Find_node(subtreeRoot(curCanonicalElt));

	  mln_niter(N) q(nbh, ip);
	  for_all(q)
	    if (pima.has(q) and isproc(q) and pima(q) <= pima(p))
	      {
		site adjCanonicalElt = Find_tree(q);
		site adjNode = Find_node(subtreeRoot(adjCanonicalElt));
		if (curNode != adjNode)
		  {
		    if (nodes(curNode).level == nodes(adjNode).level)
		      curNode = MergeNode(adjNode, curNode);
		    else
		      {
			nodes(curNode).addChild(adjNode);
			nodes(curNode).area += nodes(adjNode).area;
			nodes(curNode).highest += nodes(adjNode).highest;
		      }
		  }

		curCanonicalElt = Link_tree(adjCanonicalElt, curCanonicalElt);
		subtreeRoot(curCanonicalElt) = curNode;
	      }
	  isproc(p) = true;
	}
	// Pour garder une map de correspondance site <-> local_root
	//            for (int ip = 0; ip < int(S.size()); ++ip)
	//              {
	//                site p = S[ip];
	// 	       M(p) = Find_node(p);
	// 	     }

	mln_piter(I) r(Par_node.domain());
	for_all(r)
	  Par_node(r) = Find_node(r);

	Root = subtreeRoot(Find_tree(Find_node(site(0, 0))));
      }


      site MergeNode(site& node1, site& node2)
      {
	site tmpNode = Link_node(node1, node2);
	site tmpNode2;
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

      site Link_tree(site x, site y)
      {
	if (Rnk_tree(x) > Rnk_tree(y))
	  swap(x, y);
	else
	  if (Rnk_tree(x) == Rnk_tree(y))
	    Rnk_tree(y) += 1;
	Par_tree(x) = y;
	return y;
      }

      site Link_node(site x, site y)
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

      void print_tree(site p)
      {
	node& n = nodes(p);
	std::cout << "site " << p << "(" << n.level << ")=" << (p.row() * exact(subtreeRoot).domain().len(1) + p.col()) << " : ";

	typename p_array<mln_site(I)>::fwd_piter it(n.children);
	for_all(it)
	{
	  site q = it.to_site();
	  std::cout << q << "=" << (q.row() * subtreeRoot.domain().len(1) + q.col()) << " ";
	}
	std::cout << std::endl;

	for_all(it)
	{
	  print_tree(it.to_site());
	}
      }

      site min (p_set<site>& components)
      {
	if (components.nsites() == 0)
	  return site(-1, -1);

	typename p_set<site>::fwd_piter it(components);
	site min = components[0];

	for_all(it)
	  if (pima(it.to_site()) < pima(min))
	    min = it.to_site();

	return min;
      }

      site max (p_set<site>& components)
      {
	if (components.nsites() == 0)
	  return site(-1, -1);

	typename p_set<site>::fwd_piter it(components);
	site max = components[0];

	for_all(it)
	  if (pima(it.to_site()) > pima(max))
	    max = it.to_site();

	return max;
      }


      bool highest_fork (p_set<site>& components, site &r)
      {
	if (components.nsites() == 0)
	  {
	    std::cerr << "highest fork : empty set" << std::endl;
	    return false;
	  }

	site
	  m = min(components),
	  hfork = m;

	typename p_set<site>::fwd_piter it(components);
	for_all(it)
	{
	  // Can't remove the site from the set
	  if (it.to_site() == m)
	    continue;

	  site c = lca(hfork, it.to_site());
	  if (c != it.to_site())
	    hfork = c;
	}

	if (nodes(m).level == nodes(hfork).level)
	  return false;

	r = hfork;
	return true;
      }

      bool w_destructible(site p, site &r)
      {
	mln_niter(N) q(nbh, p);
	p_set<site> v;

	for_all(q)
	  if (pima.has(q) && pima(q) < pima(p))
	    v.insert(Par_node(q));

	if (v.nsites() == 0)
	  return false;

	site hf;
	bool is_hf = highest_fork(v, hf);

	if (!is_hf) {
	  r =  min(v);
	  return true;
	}

	if (nodes(hf).level <= pima(p)) {
	  r = hf;
	  return true;
	}
	return false;
      }

      bool highest_fork (p_set<site>& components) {
	site i;
	return highest_fork(components, i);
      }

      bool m_destructible(site p) {
	site i;
	return m_destructible(p, i);
      }

      bool w_destructible(site p) {
	site i;
	return w_destructible(p, i);
      }

      bool w_constructible(site p) {
	site i;
	return w_constructible(p, i);
      }


      bool m_destructible(site p, site &r)
      {
	mln_niter(N) q(nbh, p);
	p_set<site> v = p_set<site>();

	for_all(q)
	  if (pima.has(q) && pima(q) < pima(p))
	    v.insert(Par_node(q));

	if (v.nsites() == 0)
	  return false;

	// if (nodes(min(v)).children.nsites() != 0)
	if (nodes(min(v)).children.size() != 0)
	  return false;

	bool is_hf = highest_fork(v);

	if (!is_hf) {
	  r = min(v);
	  return true;
	}

	return false;
      }

      bool w_constructible(site p, site &r)
      {
	mln_niter(N) q(nbh, p);
	p_set<site> v;

	for_all(q)
	  if (pima.has(q) && pima(q) > pima(p))
	    v.insert(Par_node(q));

	if (v.nsites() == 0)
	  return false;

	if (v.nsites() == 1) {
	  r = v[0];
	  return true;
	}

	site
	  c = max(v),
	  cmax = c;

	typename p_set<site>::fwd_piter it(v);
	for_all(it)
	{
	  // Can't remove the site from the set
	  if (it.to_site() == cmax)
	    continue;

	  site c1 = lca(c, it.to_site());

	  if (c1 != it.to_site())
	    c = c1;
	}

	if (nodes(c).level <= pima(p))
	  return false;

	r = c;
	return true;
      }

      void m_watershed ()
      {
	typedef
	  // p_priority_queue<site, mln_value(I) >
	  std::priority_queue<site, std::vector<site>, util::greater_psite<I> >
	  ordered_queue_type;


	ordered_queue_type l(util::make_greater_psite(pima));

	// Clear the marker map
	data::fill(isproc, false);
	mln_piter(I) it(pima.domain());

	for_all(it)
	  if (m_destructible(it.to_site()))
	    {
	      l.push(it.to_site());
	      isproc(it.to_site()) = true;
	    }

	site p, i;
	while (!l.empty())
	  {
	    // Extract priority queue
	    p = l.top();
	    l.pop();

	    // unmark p
	    isproc(p) = false;

	    bool is_m = m_destructible(p, i);

	    if (is_m)
	      {
		pima(p) = nodes(i).level;
		Par_node(p) = i;

		mln_niter(N) q(nbh, p);
		for_all(q)
		  if (pima.has(q) && !isproc(q))
		    if (m_destructible(q))
		      {
			// Add priority queue
			l.push(q);

			// mark q
			isproc(q) = true;
		      }
	      }
	  }
      }

      void w_watershed()
      {
	mln::p_priority< mln_value(I), p_queue_fast<site> > L;

	mln_value(I) max = data::compute(accu::meta::stat::max(), pima);

	// I K(pima.domain(), pima.border());
	mln_ch_value(I, unsigned) K(pima.domain(), pima.border());
	mln_ch_value(I, site) H(pima.domain(), pima.border());

	// For All p of E do
	mln_piter(I) it(pima.domain());
	for_all(it)
	{
	  site p = it.to_site();
	  site i;

	  // i <- W-Destructible(p)
	  bool is_w = w_destructible(p, i);

	  // If i != infinity then
	  if (is_w)
	    {
	      L.push(max - nodes(i).level, p);
	      K(p) = nodes(i).level;
	      H(p) = i;
	    }
	}

	while (!L.is_empty())
	  {
	    mln_value(I) k = max - L.highest_priority();

	    site p = L.front();
	    L.pop();

	    if (K(p) == k)
	      {
		pima(p) = k;
		Par_node(p) = H(p);

		mln_niter(N) q(nbh, p);
		for_all(q)
		  if (pima.has(q) && k < pima(q))
		    {
		      site i;
		      bool is_w = w_destructible(q, i);
		      if (!is_w)
			K(q) = 10000; // FIXME : supposed to be infinity...
		      else
			if (K(q) != nodes(i).level)
			  {
			    L.push(max - nodes(i).level, q);
			    K(q) = nodes(i).level;
			    H(q) = i;
			  }
		    }
	      }
	  }
      }

      void revert_tree (site p)
      {
	node& n = nodes(p);

	n.level = 255 - n.level;

	// typename p_array<mln_site(I)>::fwd_piter it(n.children);
	// for_all(it)
	//   revert_tree(it.to_site());
	for (unsigned i=0; i < n.children.size(); ++i)
	  revert_tree(n.children[i]);
      }

      void gotopo()
      {
	init();

	std::cout << "Build component tree..." << std::endl;
	BuildComponentTree();
	std::cout << "                       done" << std::endl;

	std::cout << "Compressing tree..." << std::endl;
	compressTree();
	std::cout << "                       done" << std::endl;

	mln_piter(I) p (pima.domain());
	for_all(p)
	  pima(p) = 255 - pima(p);

	revert_tree(Root);

	std::cout << "Build euler tour..." << std::endl;;
	build_euler_tour();
	std::cout << "                       done" << std::endl;

	std::cout << "Build minim table..." << std::endl;;
	build_minim();
	std::cout << "                       done" << std::endl;

	std::cout << "Watersheding..." << std::endl;;
	topo_watershed();
	std::cout << "                       done" << std::endl;

	for_all(p)
	  pima(p) = 255 - pima(p);
      }


      void topo_watershed()
      {
	// Maxima components
	mln_ch_value(I, bool) cmax(pima.domain(), pima.border());

	// Mark enqueued sites
	mln_ch_value(I, bool) enqueued(pima.domain(), pima.border());

	p_priority< mln_value(I), p_queue_fast<site> > l;
	// p_queue < site > m;
	std::queue<site> m;
	mln_value(I) max = mln_max(mln_value(I));

	std::cout << "Init" << std::endl;

	// Flag C-maxima
	data::fill(cmax, false);
	data::fill(enqueued, false);

	mln_piter(I) it(Par_node.domain());
	for_all(it)
	  // if (nodes(Par_node(it.to_site())).children.nsites() == 0)
	  if (nodes(Par_node(it.to_site())).children.size() == 0)
	    {
	      cmax(it.to_site()) = true;
	      m.push(it.to_site());
	    }

	while (!m.empty())
	  {
	    mln_niter(N) q(nbh, m.front());
	    for_all(q)
	      if (cmax.has(q) && !cmax(q) && !enqueued(q))
		{
		  enqueued(q) = true;
		  l.push(pima(q), q);
		}
	    m.pop();
	  }


	std::cout << "end" << std::endl;

	// Main loop
	while(!l.is_empty())
	  {
	    site x = l.front();
	    l.pop();
	    enqueued(x) = false;

	    site c;
	    bool is_w = w_constructible(x, c);

	    if (is_w)
	      {
		pima(x) = nodes(c).level;
		Par_node(x) = c;

		// if (nodes(c).children.nsites() == 0)
		if (nodes(c).children.size() == 0)
		  cmax(x) = true;
		else
		  // if (nodes(c).children.nsites() > 1)
		  if (nodes(c).children.size() == 1)
		    std::cerr << "ERREUR COMPOSANTE BRANCHE " << nodes(c).children.size() << std::endl;


		mln_niter(N) q(nbh, x);
		for_all(q)
		  if (pima.has(q) && !cmax(q) && !enqueued(q))
		    {
		      enqueued(q) = true;

		      l.push(pima(q), q);
		    }
	      } // if (c != site(-1, -1))
	  } // while(!l.empty())

	for_all(it)
	  pima(it.to_site()) = nodes(Par_node(it.to_site())).level;
      }


      // Optimized LCA Algorithm

      int *euler;
      int *depth;
      int ctree_size;
      std::map<site, int, mln::util::ord<site> > pos;
      site *repr;

      int *minim;
      int **Minim;

      void compute_ctree_size (site p)
      {
	ctree_size += 1;
	node& n = nodes(p);

	// typename p_array<mln_site(I)>::fwd_piter it(n.children);
	// for_all(it)
	//   compute_ctree_size(it.to_site());

	for (unsigned i=0; i < n.children.size(); ++i)
	  compute_ctree_size(n.children[i]);
      }

      void build_euler_tour_rec(site p, int &position, int d)
      {
	if (pos.find(p) == pos.end())
	  pos[p] = position;

	repr[position] = p;
	depth[position] = d;
	euler[position] = pos[p];
	++position;
	node& n = nodes(p);

	// typename p_array<mln_site(I)>::fwd_piter it(n.children);
	// for_all(it)
	// {
	//   build_euler_tour_rec(it.to_site(), position, d+1);
	//   depth[position] = d; // Not optimized
	//   euler[position] = pos[p];
	//   repr[position] = p; // Pas necessaire?
	//   ++position;
	// }

	for(unsigned i=0; i < n.children.size(); ++i)
	  {
	    build_euler_tour_rec(n.children[i], position, d+1);
	    depth[position] = d; // Not optimized
	    euler[position] = pos[p];
	    repr[position] = p; // Pas necessaire?
	    ++position;
	  }
      }

      void build_euler_tour ()
      {
	ctree_size = 0;
	compute_ctree_size(Root);

	int size = 2 * ctree_size - 1;

	// FIXME : free this
	euler = new int[size];
	depth = new int[size];
	repr = new site[size];

	int position = 0;
	build_euler_tour_rec(Root, position, 0);
      }

      void build_minim ()
      {
	// compute_tree_size(); // Already done in build_euler_tour
	int size = 2 * ctree_size - 1;
	int logn = (int)(ceil(log((double)(size))/log(2.0)));
	// minim.reserve(size);
	minim = new int [logn * size]; // FIXME : Think about freeing this
	Minim = new int* [logn];

	Minim[0] = minim;

	// Init
	for (int i = 0; i < size - 1; ++i)
	  if (depth[euler[i]] < depth[euler[i+1]]) {
	    Minim[0][i] = i;
	  } else {
	    Minim[0][i] = i+1;
	  }

	//	Minim[0][size - 1] = size - 1;

	int k;
	for (int j = 1; j < logn; j++) {
	  k = 1 << (j - 1);
	  Minim[j] = &minim[j * size];
	  for (int i = 0; i < size; i++) {
	    if ((i + (k << 1)) >= size) {
	      //Minim[j][i] = size - 1;
	    }
	    else {
	      if (depth[euler[Minim[j - 1][i]]] <= depth[euler[Minim[j - 1][i + k]]])
		Minim[j][i] = Minim[j - 1][i];
	      else
		Minim[j][i] = Minim[j - 1][i + k];
	    }
	  }
	}

      } // void build_minim ()

      site lca (site a, site b)
      {
	int
	  m = pos[a],
	  n = pos[b],
	  k;

	if (m == n)
	  return repr[m];

	if (m > n)
	  {
	    k = n;
	    n = m;
	    m = k;
	  }

	k = (int)(log((double)(n - m))/log(2.));

	if (depth[euler[Minim[k][m]]] < depth[euler[Minim[k][n - (1 << k)]]]) {
	  return repr[euler[Minim[k][m]]];
	} else {
	  return repr[euler[Minim[k][n - (1 << k)]]];
	}
      }


      void removeOneSonNodes(site *p, mln_ch_value(I, site) &newPar_node)
      {
	node &n = nodes(*p);

	if (n.children.size() == 1) // this node has 1 son, delete it
	  {
	    n.area = -1;
	    newPar_node(*p) = n.children[0];
	    *p = n.children[0];
	    removeOneSonNodes(p, newPar_node);
	  }
	else // there is more than one son, recursive call
	  {
	    for (unsigned i = 0; i < n.children.size(); ++i)
	      removeOneSonNodes(&(n.children[i]), newPar_node);
	  }
      }


      void compressTree()
      {
	mln_ch_value(I, site) newPar_node(Par_node.domain(), Par_node.border());

	// Remove the nodes with one son
	removeOneSonNodes(&Root, newPar_node);

	// Update the references on deleted nodes
	mln_piter(I) p(Par_node.domain());
	for_all(p)
	  while (nodes(Par_node(p)).area == -1)
	    Par_node(p) = newPar_node(Par_node(p));
      }

    }; // struct basic_najman

  }; // namespace morpho

}; // namespace mln

