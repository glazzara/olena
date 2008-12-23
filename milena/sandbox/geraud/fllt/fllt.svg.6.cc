// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
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

#include <iomanip>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/core/routine/clone.hh>
#include <mln/core/image_if_value.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/core/site_set/p_queue_fast.hh>
#include <mln/core/image/cast_image.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/data/fill.hh>
#include <mln/level/compare.hh>
#include <mln/debug/println.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/accu/bbox.hh>
#include <mln/geom/bbox.hh>
#include <mln/pw/all.hh>

#include <mln/literal/black.hh>
#include <mln/literal/white.hh>
#include <mln/literal/colors.hh>

#include <mln/util/tree.hh>
#include <mln/util/branch_iter_ind.hh>
#include <mln/util/branch_iter.hh>

#include <sstream>


namespace mln
{

  namespace my
  {

# define fllt_tree(P, V)  mln::util::tree< fllt_node_elt<P, V> >
# define fllt_node(P, V)  mln::util::tree_node< fllt_node_elt<P, V> >
# define fllt_tree_ptr(P, V)  mln::util::tree< fllt_node_elt<P, V>* >
# define fllt_node_ptr(P, V)  mln::util::tree_node< fllt_node_elt<P, V>* >
# define fllt_branch(P, V)  mln::util::branch< fllt_node_elt<P, V> >
# define fllt_branch_iter_ind(P, V)  mln::util::branch_iter_ind< fllt_node_elt<P, V> >
# define fllt_branch_iter(P, V)  mln::util::branch_iter< fllt_node_elt<P, V> >

# define stl_to_mln_iter(T) stl_iterator< T >

    //Fwd declarations.
    template <typename V> struct lower;
    template <typename V> struct upper;

    template <typename P, typename V>
    struct fllt_node_elt
    {
      V	value;
      p_array<P> points;
      p_array<P> holes;
      std::vector<fllt_node(P, V)*> hole_shapes;
      /// Tell if his parent if brighter or not.  Nb : if the parent
      /// if brighter, the node come from the lower level set
      bool brighter;
      unsigned nsites;
      bool tagged;
      bool set_id;

      fllt_node_elt(bool set_id) : nsites(0), tagged(false), set_id(set_id) {}
    };


    template <typename C>
    class stl_iterator
    {
    public:
      stl_iterator(C& c) : container_(c) {}
      void start(){ it_ = container_.begin(); }
      void next() { it_++; }
      bool is_valid() const{ return it_ != container_.end(); }
      typename C::value_type& operator*() { return *it_; }

    private:
      C& container_;
      typename C::iterator it_;
    };

    template <typename N_t, typename G, typename Set>
    void update_gN(const N_t& N, G& gN);

    template <typename N_t, typename G, typename V>
    void update_gN(const N_t& N, G& gN, lower<V>)
    {
      for (unsigned g = 0; g < 256; ++g)
	if (N[g]->nsites() != 0)
	{
	  gN = g;
	  return;
	}
      // if N is empty, gN is the max value.
      gN = 255;
    }

    template <typename N_t, typename G, typename V>
    void update_gN(const N_t& N, G& gN, upper<V>)
    {
      for (int g = 255; g >= 0; --g)
      {
	if (N[g]->nsites() != 0)
	{
	  gN = g;
	  return;
	}
      }
      // if N is empty, gN is the min value.
      gN = 0;
    }


    template <typename N_t>
    void print_N(const N_t& N)
    {
      for (unsigned i = 0; i < 256; ++i)
      {
	if (N[i]->nsites() == 0)
	  continue;
	std::cout << i << ": " << *N[i] << std::endl;
      }
    }

    template <typename N_t>
    void clear_N(N_t& N)
    {
      for (unsigned i = 0; i < 256; ++i)
	N[i]->clear();
    }



    template <typename T>
    image2d<T> enlarge(const image2d<T>& input, unsigned coef)
    {
      unsigned
	nrows_ = coef * geom::nrows(input),
	ncols_ = coef * geom::ncols(input);
      image2d<T> output(nrows_, ncols_);
      for (int row = 0; row < nrows_; ++row)
	for (int col = 0; col < ncols_; ++col)
          opt::at(output, row, col) = opt::at(input, row / coef, col / coef);
      return output;
    }


    void save_u(const image2d<value::int_u8>& u,
		const image2d<int>& is,
		box2d R_box,
		int in_R,
		int in_N)
    {
      static int id = 0;
      std::stringstream filename;
      filename << "fllt_trace_" << std::setw(5) << std::setfill('0')
	       << std::right << id++ << ".ppm";

      image2d<value::int_u8> out = clone(cast_image<value::int_u8>(is));

      mln_assertion(R_box.nsites() > 0);
      mln_piter_(box2d) p(R_box);
      for_all(p)
	if (is(p) == in_R)
	  out(p) = 255;
	else if (is(p) == in_N)
	  out(p) = 127;
      // 	else if (is(p) == in_N)
      // 	  out(p) = literal::green;

      io::pgm::save(out, filename.str());
      //io::pgm::save(out, filename.str());
    }

    template <typename I>
    void save(const I& is, unsigned in_N, unsigned in_R, const std::string& name = "")
    {
      static unsigned counter = 0;
      using value::rgb8;

      image2d<rgb8> temp(is.domain());
      data::fill(temp, literal::black);

      mln_piter(I) p(is.domain());
      for_all(p)
	if (is(p) == in_R)
	  temp(p) = literal::red;
	else if (is(p) == in_N) // N
	  temp(p) = literal::green;
	else if (is(p) < in_N)
	  temp(p) = literal::blue;
	else
	  temp(p) = literal::white;

      if (name == "")
      {
	std::stringstream filename;
	filename << "./temp_" << ++counter << ".ppm";
	io::ppm::save(my::enlarge(temp, 10), filename.str());
      }
      else
	io::ppm::save(temp, name);
    }



    template <typename I, typename P, typename V, typename Set>
    void blob(const Set&,
	      const I& is,
	      p_array<mln_point(I)>* N[256],
	      unsigned in_N,
	      const box2d& N_box,
	      fllt_node(P, V)* current_cc)
    {
      typedef p_array<mln_point(I)> arr_t;

//       std::cout << ">>>>>>>enter blob. " << current_cc <<  std::endl;
      bool flower = true;
      unsigned ncc = 0;
      static image2d<unsigned> is_labeled(is.domain());
      static unsigned label = 0;

      if (label == 0)
      {
	data::fill(is_labeled, 0);
	label++;
      }

      P cur;
      mln_niter(neighb2d) n(Set::bdr_nbh()/*c8()*/, cur);
      p_queue_fast<P> qu;
      p_array<P>& holes = current_cc->elt().holes;

      mln_piter(I) p(N_box);
      for_all(p)
	if (is(p) == in_N)
	  break;

      mln_assertion(is(p) == in_N);
      if (is_labeled(p) != label)
      {
	if (flower == false)
	  holes.append(p);
	else
	  flower = false;
	qu.push(p);
	is_labeled(p) = label;
	do
	{
	  cur = qu.front();
	  qu.pop();
	  for_all(n) if (is.has(n))
	    if (is(n) == in_N && is_labeled(n) != label)
	    {
	      qu.push(n);
	      is_labeled(n) = label;
	    }
	}
	while (! qu.is_empty());
      }



      for (unsigned i = 0; i < 256; ++i)
      //for (int i = 255; i >= 0; --i)
      {
	mln_piter(arr_t) p(*N[i]);
	for_all(p)
	{
	  mln_assertion(is(p) == in_N);
	  if (is_labeled(p) != label)
	  {
	    if (flower == false)
	      holes.append(p);
	    else
	      flower = false;
	    qu.push(p);
	    is_labeled(p) = label;
	    do
	    {
	      cur = qu.front();
	      qu.pop();
	      for_all(n) if (is.has(n))
		if (is(n) == in_N && is_labeled(n) != label)
		{
		  qu.push(n);
		  is_labeled(n) = label;
		}
	    }
	    while (! qu.is_empty());
	  }
	}
      }

      ++label;

      //       if (holes.size() == 2)
      // 	std::cout << holes[0] << holes[1] << std::endl;
      //       std::cout << " <<<<<<<exiting blob." << std::endl;
    }

//     template <typename I, typename V>
//     void blob(const I& is,
// 	      p_array<mln_point(I)>* N[256],
// 	      unsigned in_N,
// 	      const box2d& N_box,
// 	      fllt_node(mln_point(I), V)* current_cc)
//     {
//       typedef p_array<mln_point(I)> arr_t;

// //       std::cout << ">>>>>>>enter blob. " << current_cc <<  std::endl;
//       bool flower = true;
//       unsigned ncc = 0;
//       static image2d<unsigned> is_labeled(is.domain());
//       static unsigned label = 0;

//       if (label == 0)
//       {
// 	data::fill(is_labeled, 0);
// 	label++;
//       }

//       typedef mln_psite(I) P;
//       P cur;
//       mln_niter(neighb2d) n(c8(), cur);
//       p_queue_fast<P> qu;
//       p_array<P>& holes = current_cc->elt().holes;

//       mln_piter(I) p(N_box);
//       for_all(p)
// 	if (is(p) == in_N)
// 	  break;

//       mln_assertion(is(p) == in_N);
//       if (is_labeled(p) != label)
//       {
// 	if (flower == false)
// 	  holes.append(p);
// 	else
// 	  flower = false;
// 	qu.push(p);
// 	is_labeled(p) = label;
// 	do
// 	{
// 	  cur = qu.front();
// 	  qu.pop();
// 	  for_all(n) if (is.has(n))
// 	    if (is(n) == in_N && is_labeled(n) != label)
// 	    {
// 	      qu.push(n);
// 	      is_labeled(n) = label;
// 	    }
// 	}
// 	while (! qu.is_empty());
//       }



//       for (unsigned i = 0; i < 256; ++i)
//       //for (int i = 255; i >= 0; --i)
//       {
// 	mln_piter(arr_t) p(*N[i]);
// 	for_all(p)
// 	{
// 	  mln_assertion(is(p) == in_N);
// 	  if (is_labeled(p) != label)
// 	  {
// 	    if (flower == false)
// 	      holes.append(p);
// 	    else
// 	      flower = false;
// 	    qu.push(p);
// 	    is_labeled(p) = label;
// 	    do
// 	    {
// 	      cur = qu.front();
// 	      qu.pop();
// 	      for_all(n) if (is.has(n))
// 		if (is(n) == in_N && is_labeled(n) != label)
// 		{
// 		  qu.push(n);
// 		  is_labeled(n) = label;
// 		}
// 	    }
// 	    while (! qu.is_empty());
// 	  }
// 	}
//       }

//       ++label;

//       //       if (holes.size() == 2)
//       // 	std::cout << holes[0] << holes[1] << std::endl;
//       //       std::cout << " <<<<<<<exiting blob." << std::endl;
//     }

    template <typename P, typename V>
    void
    move_A_to_R(p_array<P>& A,
		image2d<int>& deja_vu,
		fllt_node(P, V)* current_cc,
		image2d< fllt_node(P, V)* >& smallest_shapes,
		int in_R,
		int in_N,
		const V& g,
		unsigned& n_comps)
    {
      typedef p_array<P> arr_t;

      mln_piter(arr_t) a(A);
      for_all(a)
      {
	mln_invariant(deja_vu(a) == in_N);
	mln_invariant(smallest_shapes(a) != current_cc);

	deja_vu(a) = in_R;
	current_cc->elt().nsites++;
	if (!smallest_shapes(a))
	{
	  smallest_shapes(a) = current_cc;
	  current_cc->elt().points.append(a);
	}
	else
	  if (!smallest_shapes(a)->parent())
	    if (smallest_shapes(a)->elt().value == g)
	    {
	      fllt_node(P, V)* to_delete = smallest_shapes(a);

	      mln_piter(arr_t) p(smallest_shapes(a)->elt().points);
	      // Todo optimization here.
	      for_all(p)
		smallest_shapes(p) = 0;

	      while(!to_delete->children().empty())
		current_cc->add_child(*to_delete->children().begin());
	      delete to_delete;
	      n_comps--;
	      smallest_shapes(a) = current_cc;
	      current_cc->elt().points.append(a);
	    }
	    else
	      smallest_shapes(a)->set_parent(current_cc);
	// N_box is not re-computed so that we save time;
	// N_box is always growing while looping from step 3.
      }
    }

    //   LOWER LEVEL SET : region = c4, border = c8
    template <typename V>
    struct lower
    {
      typedef upper<V> opposite;
      // If compare(u,v) u root <- v <- u
      // else root <- u <- v
      static bool
      compare(const V& u, const V& v)
      {
	return u < v;
      }

      template <typename I, typename N, typename L>
      static mln_ch_value(I, L)
	regional_extremum(const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels)
      {
	return labeling::regional_minima(input, nbh, nlabels);
      }

      static const bool parent_is_brighter = true;
      static const bool id = false;

      static const neighb2d& bdr_nbh() { return c8(); }
      static const neighb2d& reg_nbh() { return c4(); }
    };


    //   UPPER LEVEL SET : region = c8, border = c4
    template <typename V>
    struct upper
    {
      typedef lower<V> opposite;

      // If compare(u,v) u root <- v <- u
      // else root <- u <- v
      static bool
      compare(const V& u, const V& v)
      { return u > v; }

      template <typename I, typename N, typename L>
      static mln_ch_value(I, L)
	regional_extremum(const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels)
      {	return labeling::regional_maxima(input, nbh, nlabels); }

      static const bool parent_is_brighter = false;
      static const bool id = true;

      static const neighb2d& bdr_nbh() { return c4(); }
      static const neighb2d& reg_nbh() { return c8(); }
    };

    template <typename I, typename Set>
    fllt_tree(mln_point(I), mln_value(I))&
      level_set(const Image<I>& input_,
		image2d< fllt_node(mln_point(I), mln_value(I))* >& smallest_shapes)
    {

      typedef mln_point(I) P;
      typedef mln_value(I) V;
      typedef fllt_node(P, V) node_type;
      typedef fllt_tree(P, V) tree_type;

      const I& input = exact(input_);

      mln_assertion(input.domain() == smallest_shapes.domain());

      unsigned l = 0, l_max = 0;
      mln_ch_value(I, unsigned) reg_min = Set::regional_extremum(input, Set::reg_nbh(), l_max);
      std::vector<bool> tag(l_max + 1, false);
      tag[0] = true;

      // Variables.
      I u = mln::clone(input);
      P x0;
      V g, gN;
      mln_fwd_piter(I) p(input.domain());
      p.start();
      data::fill(smallest_shapes, 0);
      node_type* current_cc;

      unsigned in_N = 1, in_R = 2;

      image2d<int> deja_vu(input.domain().to_larger(1));
      data::fill(deja_vu, 0);

      typedef p_array<P> arr_t;
      arr_t* A = new arr_t();
      arr_t* N[256];
      for (unsigned i = 0; i < 256; ++i)
	N[i] = new arr_t();
      accu::bbox<P> N_box;

      bool touch_border_of_image = false;
      unsigned n_step_1 = 0, n_step_3 = 0, n_comps = 0, n_holes = 0;

      // Step 1.
    step_1:
      {
	while (tag[reg_min(p)] && p.is_valid())
	  p.next();
	if (p.is_valid())
	  tag[reg_min(p)] = true; // To be processed.
	else
	  goto the_end;

	++n_step_1;
	x0 = p;
	g = input(x0);
	++n_comps;
	current_cc = new node_type(Set::id);
	current_cc->elt().value = g;
	touch_border_of_image = false;

      }

      // Step 2.
    step_2:
      {
	in_N += 2;
	in_R = in_N + 1;
	// R <- 0 and N <- 0
	clear_N(N);
	N_box.init();

	// A <- { x0 }
	A->clear();
	A->append(x0);
	N_box.take(x0);

	deja_vu(x0) = in_R;
	smallest_shapes(x0) = current_cc;
	current_cc->elt().points.append(x0);
	current_cc->elt().nsites++;

      }

      // Step 3.
    step_3:
      {
//  	save_u(u, deja_vu, N_box, in_R, in_N);
	++n_step_3;

	mln_piter(arr_t) a(*A);
	mln_niter(neighb2d) x(Set::reg_nbh(), a);


	// R <- R U A
	if (A->nsites() == 0)
	  goto the_end;

	// N <- N U { x in nbh of A and not in R }
	for_all(a)
	  for_all(x)
	{
	  if (deja_vu.has(x) && deja_vu(x) < in_N)
	  {
	    if (u.has(x))
	    {
	      N[u(x)]->append(x);
	      N_box.take(x);
	    }
	    else
	      touch_border_of_image = true;
	    deja_vu(x) = in_N;
	  }
	}
	// gN = min u(x) for all x in N
	update_gN(N, gN, Set());

	// FIXME: update the number of CC of the border of R
      }

      // Step 4.
    step_4:
      {
	// a)
	if (Set::compare(g, gN))
	{
	  g = gN;

	  ++n_comps;

	  if (touch_border_of_image)
	    blob(Set(), deja_vu, N, in_N, N_box.to_result().to_larger(1), current_cc);
	  else
	    blob(Set(), deja_vu, N, in_N, N_box, current_cc);

	  n_holes += current_cc->elt().holes.nsites();

	  node_type* child = current_cc;
	  current_cc = new node_type(Set::id);
	  current_cc->elt().value = g;
	  child->set_parent(current_cc);


	  arr_t* tmp = A;
	  A = N[g];
	  N[g] = tmp;
	  N[g]->clear();
	  move_A_to_R(*A, deja_vu, current_cc, smallest_shapes, in_R, in_N, g, n_comps);
	  goto step_3;
	}
	// b)
	else if (g == gN)
	{
	  arr_t* tmp = A;
	  A = N[g];
	  N[g] = tmp;
	  N[g]->clear();
	  move_A_to_R(*A, deja_vu, current_cc, smallest_shapes, in_R, in_N, g, n_comps);
	  goto step_3;
	}
	// c)
	else
	{
	  // FIXME: IDEA: this change might be performed while R is constructed(?)
	  mln_piter(I) r(N_box);
	  for_all(r)
	    if (deja_vu(r) == in_R)
	      u(r) = g;

	  goto step_1;
	}
      }

    the_end:
      std::cout << "l_max " << l_max << " " << n_step_1 << ' ' << n_step_3 << std::endl;
      std::cout << "n comps = " << n_comps << "  n holes = " << n_holes << std::endl;

      return *new tree_type(current_cc);
    }

    // F is the set in which we get the node.
    template <typename P, typename V, typename F>
    fllt_node(P, V)*
      find_hole(fllt_node(P, V)& node,
		const P p,
		const image2d<fllt_node(P, V)*>& other_reg)
    {
      fllt_node(P, V)* s = other_reg(p);
      mln_assertion(s);
      while (s->parent() && F::compare(s->parent()->elt().value, node.elt().value))
      {
	mln_assertion(s);
	s = s->parent();
	mln_assertion(s);
      }
//       std::cout << "   [Find the hole] of " << p
// 		<< " from " << &node
// 		<< " return " << s
// 		<< std::endl;
      return s;
    }

    template <typename P, typename V>
    bool shape_is_included(fllt_node(P, V)* A,
			   fllt_node(P, V)* B)
    {
      return A->parent() == B || A == B;
    }

    template <typename P, typename V>
    void find_all_holes(fllt_tree(P, V)& lower_tree,
			fllt_tree(P, V)& upper_tree,
			const image2d<fllt_node(P, V)*>& low_reg,
			const image2d<fllt_node(P, V)*>& upp_reg)
    {
      typedef p_array<P> arr_t;
      typedef fllt_node(P, V) node_type;

      {
	fllt_branch_iter_ind(P, V) node_(lower_tree.main_branch());
	for_all(node_)
	{
	  node_type& node = *node_;
	  mln_piter(arr_t) hole(node.elt().holes);
	  for_all(hole)
	    node.elt().hole_shapes.push_back(find_hole<P,V,upper<V> >(node, P(hole), upp_reg));
	}
      }

      {
	fllt_branch_iter_ind(P, V) node_(upper_tree.main_branch());
	for_all(node_)
	{
	  node_type& node = *node_;
	  mln_piter(arr_t) hole(node.elt().holes);
	  for_all(hole)
	    node.elt().hole_shapes.push_back(find_hole<P,V,lower<V> >(node, P(hole), low_reg));
	}
      }
    }

    template <typename I>
    fllt_tree(mln_point(I), mln_value(I))
      merge_trees(fllt_tree(mln_point(I), mln_value(I))& lower_tree,
		  fllt_tree(mln_point(I), mln_value(I))& upper_tree,
		  const image2d<fllt_node(mln_point(I), mln_value(I))*>& low_reg,
		  const image2d<fllt_node(mln_point(I), mln_value(I))*>& upp_reg,
		  const Image<I>& input_)
    {

      const I& input = exact(input_);
      typedef mln_point(I) P;
      typedef mln_value(I) V;

      typedef fllt_node(P, V) node_type;
      typedef fllt_tree(P, V) tree_type;
      typedef p_array<P> arr_t;


      find_all_holes(lower_tree, upper_tree, low_reg, upp_reg);
      std::vector<node_type*> to_fill;

      fllt_branch_iter_ind(P, V) node_(lower_tree.main_branch());
      for_all(node_)
      {
	node_type& node = *node_;
	if (node.elt().set_id != lower<V>::id)
	  continue;

	// 	std::cout << "Fill " << &node << std::endl;
	typename std::vector<fllt_node(P, V)*>::iterator hole_;
	for (hole_ = node.elt().hole_shapes.begin();
	     hole_ != node.elt().hole_shapes.end();
	     hole_++)
	{
	  fllt_node(P, V)* hole = *hole_;

 	  bool child_has_bigger_hole = false;
	  typename fllt_node(P, V)::children_t::iterator it;
	  for (it = node.children().begin(); it != node.children().end() && !child_has_bigger_hole; it++)
	  {
	    // Browse the holes of each child.
	    typename std::vector<fllt_node(P, V)*>::iterator child_hole_;
	    for (child_hole_ = (*it)->elt().hole_shapes.begin();
		 child_hole_ != (*it)->elt().hole_shapes.end();
		 child_hole_++)
	    {
	      fllt_node(P, V)* child_hole = *child_hole_;
	      // 	      std::cout << "hole : " << hole << " " << hole->elt().points << " " << std::endl;
	      // 	      std::cout << "child hole : " << child_hole << " " << child_hole->elt().points << std::endl;
	      if (shape_is_included(hole, child_hole))
	      {
		child_has_bigger_hole = true;
		break;
	      }
	    } // end of browsing child's holes.
	  } // end of browsing childs.
 	  if (!child_has_bigger_hole)
 	  {
	    // // 	    std::cout << "move " << hole << " as child of " << &node << std::endl;
	    node.add_child(hole);
	    to_fill.push_back(hole);
 	  }
 	} // end of browsing holes of node.
	node.elt().holes.clear();
      } // end of browsing lower_tree.

      for(typename std::vector<node_type*>::iterator node_ = to_fill.begin();
      	  node_ != to_fill.end();
      	  node_++)
      {
      	node_type& node = **node_;

      	fllt_branch_iter_ind(P, V) node_(fllt_branch(P, V)(upper_tree, node));
      	for_all(node_)
      	{
      	  node_type& node = *node_;
      	  if (node.elt().set_id != upper<V>::id)
      	    continue;

	  typename std::vector<fllt_node(P, V)*>::iterator hole_;
	  for (hole_ = node.elt().hole_shapes.begin();
	       hole_ != node.elt().hole_shapes.end();
	       hole_++)
	  {
	    fllt_node(P, V)* hole = *hole_;

      	    bool child_has_bigger_hole = false;
      	    typename fllt_node(P, V)::children_t::iterator it;
      	    for (it = node.children().begin(); it != node.children().end() && !child_has_bigger_hole; it++)
      	    {
      	      // Browse the holes of each child.
	      typename std::vector<fllt_node(P, V)*>::iterator child_hole_;
	      for (child_hole_ = (*it)->elt().hole_shapes.begin();
		   child_hole_ != (*it)->elt().hole_shapes.end();
		   child_hole_++)
	      {
		fllt_node(P, V)* child_hole = *child_hole_;
      		//if (hole->elt().points <= child_hole->elt().points)
		if (shape_is_included(hole, child_hole))
      		{
      		  child_has_bigger_hole = true;
      		  break;
      		}
      	      } // end of browsing child's holes.
      	    } // end of browsing childs.

      	    if (!child_has_bigger_hole)
      	      node.add_child(hole);

      	  } // end of browsing holes of node.
      	  node.elt().holes.clear();
      	} // end of browsing lower_tree.

      }

      return lower_tree;
    }

    template <typename I>
    fllt_tree(mln_point(I), mln_value(I))
      fllt(const Image<I>& input_)
    {
      typedef mln_point(I) P;
      typedef mln_value(I) V;

      const I& input = exact(input_);

      fllt_tree(P, V) upper_tree;
      fllt_tree(P, V) lower_tree;
      image2d<fllt_node(P, V)*> low_reg(input.domain());
      image2d<fllt_node(P, V)*> upp_reg(input.domain());

      std::cout << "1/ Compute the lower level set.----------------------------------------" << std::endl;
      lower_tree = level_set<I, lower<V> >(input, low_reg);
      //draw_tree(input, lower_tree);

      std::cout << "2/ Compute the upper level set.----------------------------------------" << std::endl;
      upper_tree = level_set<I, upper<V> >(input, upp_reg);
      //draw_tree(input, upper_tree);

      std::cout << "3/ Merge.---------------------------------------------------------------" << std::endl;
      fllt_tree(P, V) result_tree = merge_trees(lower_tree, upper_tree, low_reg, upp_reg, input);

      return result_tree;
    }

    template <typename P, typename V>
    void
    draw_tree(const image2d<V>& ima,
	      fllt_tree(P, V)& tree)
    {
      p_array<P> tmp;

      fllt_branch_iter_ind(P, V) p(tree.main_branch());
      for_all(p)
      {
	std::cout << "region mere : " << (*p).parent() << std::endl;
	std::cout << "               ^" << std::endl;
	std::cout << "               |" << std::endl;
	std::cout << "region : " << &*p
		  << " value = " << (*p).elt().value << std::endl
		  << " from " << ((*p).elt().set_id == lower<V>::id ? "lower" : "upper") << " level set." << std::endl
		  << " nsites = " << (*p).elt().nsites << std::endl
		  << " holes = " << (*p).elt().holes << std::endl;

	std::cout << std::endl;

	tmp.append((*p).elt().points);

	fllt_branch_iter_ind(P, V) n(fllt_branch(P, V)(tree, *p));
	for_all(n)
	  tmp.append((*n).elt().points);

	if ((*p).elt().points.nsites() > 0)
	  debug::println(ima | tmp);
	tmp.clear();

	std::cout << std::endl;
      }
    }


    template <typename P, typename V, typename I>
    unsigned
    compute_area_rec(fllt_node(P, V)* node, I& ima)
    {

      if (!node)
	return 0;

      int area = 0;

      for (int i = 0; i < node->children().size();i++)
	area += compute_area_rec(node->children()[i], ima);

      mln_piter(p_array<P>) p(node->elt().points);
      for_all(p)
	if (!ima(P(p)))
	{
	  ++area;
	  ima(p) = true;
	}

      node->elt().nsites = area;
      return area;
    }

    template <typename P, typename V, typename I>
    void
    compute_area(const Image<I>& input_, fllt_tree(P, V)& tree)
    {
      const I& input = exact(input_);

      image2d<bool> ima(input.domain());
      data::fill(ima, false);
      compute_area_rec(tree.root(), ima);
    }

    void draw_shape(image2d<value::int_u8>& output,
		    fllt_node(point2d, value::int_u8)* node)
    {
      typedef point2d P ;
      typedef value::int_u8 V;

      fllt_tree(P, V) subtree(node);
      fllt_branch_iter_ind(P, V) s(fllt_branch(P, V)(subtree, *node));
      for_all(s)
	data::fill(output | (*s).elt().points), (*s).elt().value);
    }

    void area_filter(image2d<value::int_u8>& output,
		     fllt_node(point2d, value::int_u8)* node,
		     unsigned min_area,
		     unsigned max_area,
		     value::int_u8 bg)
    {
      typedef point2d P ;
      typedef value::int_u8 V;

      data::fill(output, bg);
      fllt_tree(P, V) subtree(node);
      fllt_branch_iter_ind(P, V) s(fllt_branch(P, V)(subtree, *node));
      for_all(s)
	if ((*s).elt().nsites > min_area && (*s).elt().nsites < max_area)
	  draw_shape(output, &*s);
    }

    void area_filter_min(image2d<value::int_u8>& output,
			 fllt_node(point2d, value::int_u8)* node,
			 unsigned min_area,
			 value::int_u8 g,
			 unsigned accu)
    {
//       if ((*node).elt().nsites >= min_area)
      if (accu > min_area)
      {
	accu = 0;
	g = (*node).elt().value;
      }

      accu += (*node).elt().nsites;
      data::fill(output | (*node).elt().points), g);

      for (int i = 0; i < node->children().size();i++)
	area_filter_min(output, node->children()[i], min_area, g, accu);
    }

  } // end of namespace mln::my

} // end of namespace mln


int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace mln::my;
  using value::int_u8;

 typedef fllt_tree(point2d, int_u8) tree_type;



//   if (argc != 2)
//     {
//       std::cerr << "usage: " << argv[0] << " filename" << std::endl;
//       return 1;
//     }

  image2d<int_u8> lena;
  io::pgm::load(lena, argv[1]);


//   int vs[8][9] = { {0,0,0,0,0,0,0,0,0},
// 		   {0,0,0,0,0,0,0,0,0},
// 		   {0,1,1,1,1,1,1,1,0},
// 		   {0,1,0,0,1,3,3,1,0},
// 		   {0,1,0,0,1,3,4,1,0},
// 		   {0,1,0,0,1,3,3,1,0},
// 		   {0,1,1,1,1,1,1,1,0},
// 		   {0,0,0,0,0,0,0,0,0} };



//   int vs[8][9] = { {6,6,6,6,6,6,6,6,6},
// 		   {6,6,6,6,6,6,6,6,6},
// 		   {6,5,5,5,5,5,5,5,6},
// 		   {6,5,6,6,5,3,3,5,6},
// 		   {6,5,6,6,5,3,0,5,6},
// 		   {6,5,6,6,5,3,3,5,6},
// 		   {6,5,5,5,5,5,5,5,6},
// 		   {6,6,6,6,6,6,6,6,6} };

//   int vs[8][9] = { {2,2,2,2,2,2,2,2,2},
// 		   {2,2,2,2,2,2,2,2,2},
// 		   {2,1,1,1,1,1,1,1,2},
// 		   {2,1,2,2,1,0,0,1,2},
// 		   {2,1,2,2,1,0,0,1,2},
// 		   {2,1,2,2,1,0,0,1,2},
// 		   {2,1,1,1,1,1,1,1,2},
// 		   {2,2,2,2,2,2,2,2,2} };


  int vs[8][9] = { {2,2,2,2,2,2,2,2,2},
		   {2,2,2,2,2,2,2,2,2},
		   {2,1,1,1,1,1,1,1,2},
		   {2,1,2,2,1,0,0,1,2},
		   {2,1,2,2,1,0,4,1,2},
		   {2,1,2,2,1,0,0,1,2},
		   {2,1,1,1,1,1,1,1,2},
		   {2,2,2,2,2,2,2,2,2} };

//  int vs[10][13] = { {1,1,1,1, 1,1,1,1, 1,1,1,1,1},
// 		    {1,2,2,2,2,2,2,2,2,2,2,2,1},
// 		    {1,2,2,2,2,2,2,2,2,2,2,2,1},
// 		    {1,2,2,2,3,3,3,3,3,3,3,2,1},
// 		    {1,2,2,2,3,3,3,2,2,2,3,2,1},
// 		    {1,2,2,2,3,4,3,2,4,4,3,2,1},

// 		    {1,2,2,2,3,3,3,2,2,2,3,2,1},
// 		    {1,2,2,2,3,3,3,3,3,3,3,2,1},
// 		    {1,2,2,2,2,2,2,2,2,2,2,2,1},
// 		    {1,1,1,1,1,1,1,1,1,1,1,1,1}};


//   image2d<int> lena_(make::image2d(vs));
//   image2d<int_u8> lena(lena_.domain());
//   data::fill(lena, lena_);

  tree_type tree = my::fllt(lena);
  compute_area(lena, tree);

  image2d<value::int_u8> output (lena.domain ());
//   area_filter_min(output, tree.root(), atoi(argv[2]), 0, 0);
  area_filter(output, tree.root(), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
  io::pgm::save(output, "out.pgm");

  //  draw_tree(lena, tree);
}
