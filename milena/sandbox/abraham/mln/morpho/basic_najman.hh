#include <mln/level/sort_psites.hh>
#include <mln/level/fill.hh>
#include <mln/core/image2d.hh>
#include <mln/core/p_set.hh>
#include <mln/util/greater_psite.hh>
#include <mln/estim/min_max.hh>
#include <mln/math/sqr.hh>

#include <queue>
#include <set>

namespace mln
{

  // Here we define some custome priority functions

  namespace util {

    template <typename I>
    class najman_distance
    {
    public:
      typedef mln_psite(I) psite;

      najman_distance(const Image<I>& ima);

      /// Is \a x greater than \a y?
      bool operator()(const psite& x, const psite& y);

      // mln_value(I)
      int
      D(const mln_psite(I)& x);

    private:
      const I& ima_;
    };


    template <typename I>
    najman_distance<I>
    make_najman_distance(const Image<I>& ima);

# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    najman_distance<I>::najman_distance(const Image<I>& ima)
      : ima_ (exact(ima))
    {
    }

    template <typename I>
    // mln_value(I)
    int
    najman_distance<I>::D(const mln_psite(I)& x)
    {
      const I& ima = exact(ima_);

      mln_piter(I) it(ima.domain());

      it.start();
      mln_psite(I) min = it.to_point();

      for_all(it)
      {
	// FIXME : REALLY not optimized
	// FIXME : REALLY not efficient

	if (ima(it.to_point()) < ima(x))
	// using norm2
	  if ( (// math::sqr(int(ima(it.to_point())) - ima(x)) +
		math::sqr(int(it.to_point().row()) - x.row()) +
		math::sqr(int(it.to_point().col()) - x.col())) <
	       (// math::sqr(int(ima(min)) - ima(x)) +
		math::sqr(int(min.row()) - x.row()) +
		math::sqr(int(min.col()) - x.col())) )
	    min = it.to_point();
      }

      return /* math::sqr(int(ima(min)) - ima(x)) +*/ math::sqr(int(min.row()) - x.row()) + int(math::sqr(min.col()) - x.col());
    }

    template <typename I>
    bool
    najman_distance<I>::operator()(const mln_psite(I)& x, const mln_psite(I)& y)
    {
      if (ima_(x) < ima_(y))
	return false;
      else
	if (ima_(x) > ima_(y))
	  return true;

      // ima(x) = ima(y)
      return !(D(x) <= D(y));

    }


    template <typename I>
    najman_distance<I>
    make_najman_distance(const Image<I>& ima)
    {
      return najman_distance<I>(ima);
    }

# endif // ! MLN_INCLUDE_ONLY






    // Priority function for the second wateshed

    template <typename I>
    class lesser_psite
    {
    public:
      typedef mln_psite(I) psite;

      lesser_psite(const Image<I>& ima);

      /// Is \a x lesser than \a y?
      bool operator()(const psite& x, const psite& y);

    private:
      const I& ima_;
    };

    template <typename I>
    lesser_psite<I>
    make_lesser_psite(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    lesser_psite<I>::lesser_psite(const Image<I>& ima)
      : ima_ (exact(ima))
    {
    }

    template <typename I>
    bool
    lesser_psite<I>::operator()(const mln_psite(I)& x, const mln_psite(I)& y)
    {
      return ima_(x) < ima_(y);
    }


    template <typename I>
    lesser_psite<I>
    make_lesser_psite(const Image<I>& ima)
    {
      return lesser_psite<I>(ima);
    }

# endif // ! MLN_INCLUDE_ONLY



    template <typename I, typename N>
    class my_lesser_psite
    {
    public:
      typedef mln_psite(I) psite;

      my_lesser_psite(const Image<I>& ima,
		      const Neighborhood<N>& nbh);

      /// Is \a x lesser than \a y?
      bool operator()(const psite& x, const psite& y);

    private:
      const I& ima_;
      const N& nbh_;
    };


    template <typename I, typename N>
    my_lesser_psite<I, N>
    make_my_lesser_psite(const Image<I>& ima,
			 const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N>
    my_lesser_psite<I, N>::my_lesser_psite(const Image<I>& ima,
					   const Neighborhood<N>& nbh)
      : ima_ (exact(ima)), nbh_(exact(nbh))
    {
    }

    template <typename I, typename N>
    bool
    my_lesser_psite<I, N>::operator()(const mln_psite(I)& x, const mln_psite(I)& y)
    {

#ifdef V1

      if (ima_(x) == ima_(y))
	{
	  int mx = 0, my = 0;

	  mln_niter(N) a(nbh_, x);
	  for_all(a)
	    if (ima_.has(a) and ima_(a) > ima_(x))
	      ++mx;

	  mln_niter(N) b(nbh_, y);
	  for_all(b)
	    if (ima_.has(b) and ima_(b) > ima_(y))
	      ++my;

	  return mx > my;
	}

#else


      if (ima_(x) == ima_(y))
	{
	  mln_value(I) mx = ima_(x), my = ima_(y);

	  mln_niter(N) a(nbh_, x);
	  for_all(a)
	    if (ima_.has(a) and ima_(a) > ima_(mx))
	      mx = a;

	  mln_niter(N) b(nbh_, y);
	  for_all(b)
	    if (ima_.has(b) and ima_(b) > ima_(my))
	      my = b;

	  return ima_(x) < ima(y);
	}


#endif


      return ima_(x) < ima_(y);
    }


    template <typename I, typename N>
    my_lesser_psite<I, N>
    make_my_lesser_psite(const Image<I>& ima,
			 const Neighborhood<N>& nbh)
    {
      return my_lesser_psite<I, N>(ima);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util













  namespace morpho
  {

    template <class I, class N>
    struct basic_najman
    {

      typedef mln_psite(I) psite;

      struct node {
	mln_value(I) level;
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

      // Ctor
      basic_najman(const Image<I>& i,
		   const Neighborhood<N>& n)
	: pima(exact(i)),
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
	mln_piter(I) p (pima.domain());
	for_all(p)
	  pima(p) = pima(p) + 1;

	init();

	BuildComponentTree();

	for_all(p)
	  pima(p) = pima(p) - 1;

      }


      void init()
      {
	// Sort the points in increasing order
	S = level::sort_psites_increasing(pima);

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
	    nodes(p) = MakeNode(pima(p));
	  }
      }


      void BuildComponentTree()
      {
	typename p_array<mln_psite(I)>::fwd_piter ip(S);
	for_all(ip)
	  {
	    psite p = ip.to_point();

	    psite curCanonicalElt = Find_tree(p);
	    psite curNode = Find_node(subtreeRoot(curCanonicalElt));

	    mln_niter(N) q(nbh, p);
	    for_all(q)
	      if (pima.has(q) and isproc(q) and pima(q) <= pima(p))
		{
		  psite adjCanonicalElt = Find_tree(q);
		  psite adjNode = Find_node(subtreeRoot(adjCanonicalElt));
		  if (curNode != adjNode)
		    if (nodes(curNode).level == nodes(adjNode).level)
		      curNode = MergeNode(adjNode, curNode);
		    else
		      {
			nodes(curNode).addChild(adjNode);
			nodes(curNode).area += nodes(adjNode).area;
			nodes(curNode).highest += nodes(adjNode).highest;
		      }

		  curCanonicalElt = Link_tree(adjCanonicalElt, curCanonicalElt);
		  subtreeRoot(curCanonicalElt) = curNode;
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
	psite r = lca_rec(Root, Par_node(a), Par_node(b));

	// These two conditions make the lca become a plca
	// if (r == Par_node(a))
	//   return psite(-1, -1);

	// if (r == Par_node(b))
	//   return psite(-1, -1);

	return r;
      }

      psite lca_rec (psite cur, psite a, psite b)
      {
	// FIXME : naive implementation, make something better

	// If we found one of the point, we return it so it can be caught in
	// the loop

	if (cur == a)
	  return a;

	if (cur == b)
	  return b;

	// We're on a leaf, the point has not been found

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

	      // On of the point has been encountered in a child branch
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

	      // If the lca has already been found, it would be returned
	      // so we just have to return it
	      return tmp;
	    }
	}

	// The two points had been spotted in the children, cur is the LCA
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
	  if (pima(it.to_point()) < pima(min))
	    min = it.to_point();

	return min;
      }

      psite max (p_set<psite>& components)
      {
	if (components.npoints() == 0)
	  return psite(-1, -1);

	typename p_set<psite>::fwd_piter it(components);
	psite max = components[0];

	for_all(it)
	  if (pima(it.to_point()) > pima(max))
	    max = it.to_point();

	return max;
      }


      psite highest_fork (p_set<psite>& components)
      {
	if (components.npoints() == 0)
	  {
	    std::cerr << "highest fork : empty set" << std::endl;
	    return psite(-1, -1);
	  }

	// if (components.npoints() == 1)
	//  return components[0];

	psite
	  m = min(components),
	  hfork = m;

	typename p_set<psite>::fwd_piter it(components);
	for_all(it)
	{
	  // Can't remove the point from the set
	  if (it.to_point() == m)
	    continue;

	  psite c = lca(hfork, it.to_point());
	  if (c != it.to_point())
	  // hfork = it.to_point();
	     hfork = c;
	}

	if (nodes(m).level == nodes(hfork).level)
	  return psite(-1, -1);

	return hfork;
      }

      psite w_destructible(psite p)
      {
	mln_niter(N) q(nbh, p);
	p_set<psite> v;

	for_all(q)
	  if (pima.has(q) && pima(q) < pima(p))
	    v.insert(Par_node(q));

	if (v.npoints() == 0)
	  return psite(-1, -1);

	psite hf = highest_fork(v);

	if (hf == psite(-1, -1))
	  return min(v);

	if (nodes(hf).level <= pima(p))
	  return hf;

	return psite(-1, -1);
      }

      psite m_destructible(psite p)
      {
	mln_niter(N) q(nbh, p);
	p_set<psite> v = p_set<psite>();

	for_all(q)
	  if (pima.has(q) && pima(q) < pima(p))
	    v.insert(Par_node(q));

	if (v.npoints() == 0)
	  return psite(-1, -1);

	if (nodes(min(v)).children.npoints() != 0)
	  return (psite(-1, -1));

	//std::cout << "hf of " << p << ":" << v;
	psite hf = highest_fork(v);
	//std::cout << " is " << hf << std::endl;

	if (hf == psite(-1, -1))
	  return min(v);

	return psite(-1, -1);
      }

      psite w_constructible(psite p)
      {
	mln_niter(N) q(nbh, p);
	p_set<psite> v;

	psite pmax = p;

	for_all(q)
	  if (pima.has(q) && pima(q) > pima(p))
	    v.insert(Par_node(q));

	if (v.npoints() == 0)
	  return psite(-1, -1);

	if (v.npoints() == 1)
	  return v[0];

	psite
	  c = max(v);

	typename p_set<psite>::fwd_piter it(v);
	for_all(it)
	{
	  // Can't remove the point from the set
	  if (it.to_point() == c)
	    continue;

	  psite c1 = lca(c, it.to_point());
	  if (c1 != it.to_point())
	    c = c1;
	}

	if (nodes(c).level <= pima(p))
	  return psite(-1, -1);

	return c;
      }


      void m_watershed ()
      {

	// FIXME : make a better priority function
	typedef
	  std::priority_queue< psite, std::vector<psite>, util::greater_psite<I> >
	  //	  std::priority_queue< psite, std::vector<psite>, util::najman_distance<I> >
	  ordered_queue_type;


	ordered_queue_type l(util::make_greater_psite(pima));
	//	ordered_queue_type l(util::make_najman_distance(pima));

	// Clear the marker map
	level::fill(isproc, false);
	mln_piter(I) it(pima.domain());

	for_all(it)
	  if (m_destructible(it.to_point()) != psite(-1, -1))
	    {
	      //std::cout << "init: push " << it.to_point() << std::endl;
	      l.push(it.to_point());
	      isproc(it.to_point()) = true;
	    }

	psite p, i;
	while (!l.empty())
	  {
	    // Extract priority queue
	    p = l.top();
	    l.pop();

	    // unmark p
	    isproc(p) = false;

	    i = m_destructible(p);
	    //std::cout << "point " << p << "is m-destructible " << i << std::endl;

	    if (i != psite(-1, -1))
	      {
		pima(p) = nodes(i).level - 1 ;
		Par_node(p) = i;

		mln_niter(N) q(nbh, p);
		for_all(q)
		  if (pima.has(q) && !isproc(q))
		    if (m_destructible(q) != psite(-1, -1))
		      {
			//			if (q == psite(1, 1))
			// std::cout << "psite(1, 1) ajoute dans la boucle" << std::endl;

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
	std::map< mln_value(I), std::set<psite> > L;

	// Setting the min and the max of the image
	mln_value(I) k, kmin, kmax;
	mln::estim::min_max(pima, kmin, kmax);

	// For k From kmin to kmax - 1 Do Lk <- <empty set>
	for (k = kmin; k < kmax; k++)
	  L[k] = std::set<psite>();

	// I K(pima.domain(), pima.border());
	mln_ch_value(I, int) K(pima.domain(), pima.border());
	level::fill(K, 73320);
	mln_ch_value(I, psite) H(pima.domain(), pima.border());

	// For All p of E do
	mln_piter(I) it(pima.domain());
	for_all(it)
	{
	  psite p = it.to_point();

	  // i <- W-Destructible(p)
	  psite i = w_destructible(p);

	  // If i != infinity then
	  if (i != psite(-1, -1))
	    {
	      L[nodes(i).level - 1].insert(p);
	      K(p) = nodes(i).level - 1;
	      H(p) = i;
	    }
	}

	for (k = kmin; k < kmax; k++)
	  {
	    std::set<psite>& Lk = L[k];

	    while (!Lk.empty())
	      {
		psite p = *(Lk.begin());
		Lk.erase(p);

		if (K(p) == 73320)
		  std::cerr << "Comparison with an uninitilized number" << std::endl;

		if (K(p) == (int) k)
		  {
		    pima(p) = k;
		    Par_node(p) = H(p);

		    mln_niter(N) q(nbh, p);
		    for_all(q)
		      if (pima.has(q) && k < pima(q))
			{
			  psite i = w_destructible(q);
			  if (i == psite(-1, -1))
			    K(q) = 10000;
			  else
			    if (K(q) != nodes(i).level - 1)
			      {
				L[nodes(i).level - 1].insert(q);
				K(q) = nodes(i).level - 1;
				H(q) = i;
			      }
			}



		      if (pima.has(p) && k < pima(p))
			{

			  std::cout << "cas improbable" << std::endl;

			  psite i = w_destructible(p);
			  if (i == psite(-1, -1))
			    K(p) = 10000;
			  else
			    if (K(p) != nodes(i).level - 1)
			      {
				L[nodes(i).level - 1].insert(p);
				K(p) = nodes(i).level - 1;
				H(p) = i;
			      }
			}



		  }
	      }
	  }
      }


      void revert_tree (psite p)
      {
	node& n = nodes(p);

	n.level = 255 - n.level;

	typename p_array<mln_psite(I)>::fwd_piter it(n.children);

	for_all(it)
	  revert_tree(it.to_psite());
      }

      void gotopo()
      {
	init();

	BuildComponentTree();

	mln_piter(I) p (pima.domain());
	for_all(p)
	  pima(p) = 255 - pima(p);

	revert_tree(Root);

	topo_watershed();

	for_all(p)
	  pima(p) = 255 - pima(p);
      }


      void topo_watershed()
      {
	// Maxima components
	mln_ch_value(I, bool) cmax(pima.domain(), pima.border());

	// Mark enqueued points
	mln_ch_value(I, bool) enqueued(pima.domain(), pima.border());

	// Not used
	// level::fill(isproc, false);


	typedef
	  std::priority_queue< psite, std::vector<psite>, util::lesser_psite<I> >
	  ordered_queue_type;

	ordered_queue_type l(util::make_lesser_psite(pima));




	// Flag C-maxima
	level::fill(cmax, false);
	mln_piter(I) it(Par_node.domain());
	for_all(it)
	  if (nodes(Par_node(it.to_point())).children.npoints() == 0)
	    cmax(it.to_point()) = true;

#ifdef SLOW

	// Enqueue all
	for_all(it)
	{
	  enqueued(it.to_point()) = true;
	  l.push(it.to_point());
	}
#else // !SLOW

	// Optimisation : enqueue minima's neighbours
	level::fill(enqueued, false);
	for_all(it)
	{
	  mln_niter(N) q(nbh, it.to_point());
	  for_all(q)
	    if (cmax.has(q) && cmax(q))
	      {
		enqueued(it.to_point()) = true;
		l.push(it.to_point());
		break;
	      }
	}

#endif // SLOW


	// Main loop
	while(!l.empty())
	  {
	    psite x = l.top();
	    l.pop();
	    enqueued(x) = false;

	    psite c = w_constructible(x);

	    if (x == psite(3, 3))
	      {
		std::cout << "w-c : " << c << std::endl;
		io::pgm::save(pima, "int.pgm");
	      }

	    if (c != psite(-1, -1))
	      {
		pima(x) = nodes(c).level;
		Par_node(x) = c;
		// isproc(x) = true;

		if (nodes(c).children.npoints() == 0)
		  cmax(x) = true;
		else
		  if (nodes(c).children.npoints() > 1)
		    {
		    }
		  else
		    std::cerr << "ERREUR COMPOSANTE BRANCHE" << std::endl;

		mln_niter(N) q(nbh, x);
		for_all(q)
		  if (pima.has(q) && !cmax(q) && !enqueued(q))
		    {
		      enqueued(q) = true;
		      l.push(q);
		    }
	      } // if (c != psite(-1, -1))
	  } // while(!l.empty())

	for_all(it)
	  pima(it.to_point()) = nodes(Par_node(it.to_point())).level;
      }
    }; // struct basic_najman

  }; // namespace morpho

}; // namespace mln

