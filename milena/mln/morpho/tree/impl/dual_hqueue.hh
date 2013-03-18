// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_MORPHO_TREE_IMPL_COMPUTE_PARENT_DUAL_HQUEUE_HH
# define MLN_MORPHO_TREE_IMPL_COMPUTE_PARENT_DUAL_HQUEUE_HH

///
/// \brief The dual input tree algorithm specialized for low quantized image.
///
/// This implementation is based on P. Salembier algorithm using
/// hierachical queues. This implies a low-quantized input image so
/// that the number of queues is limited.
///
/// TODO: Think about how to extend f domain in a more
/// generic way. The actual implementation doubles the size of the
/// first dimension. It implies a boxed domain.
///
/// TODO: Use the less functor. The actual implementation is for max-tree.
///
/// TODO: During the canonization pass, we build the tree site set
/// from the sorted site set of f, so that we compute twice f
/// histogram (can be avoided).

# include <mln/data/sort_psites.hh>
# include <mln/data/fill.hh>

# include <mln/geom/nsites.hh>
# include <mln/geom/translate.hh>

# include <mln/morpho/tree/data.hh>

# include <mln/util/hqueues.hh>
# include <mln/util/ord.hh>

# include <mln/value/value_array.hh>
# include <mln/value/set.hh>

# include <mln/util/timer.hh>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      namespace impl
      {

	/// Compute a tree using hqueues.
	///
	/// \param f The original image.
	/// \param m The connectivity mask.
	/// \param nbh The neighborhood of the mask.
	///
	/// \return The tree structure.
	///
	template <typename I, typename N>
	inline
	data<I, p_array<mln_psite(I)> >
	dual_hqueue(const Image<I>& f,
		    const Image<I>& m,
		    const Neighborhood<N>& nbh);

      } // end of namespace mln::morpho::tree::impl


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename I, typename N, class E>
	struct shared_flood_args
	{
	  typedef mln_psite(I) P;
	  typedef mln_value(I) V;
	  typedef p_array<P> S;

	  const I& f;
	  const I& m;
	  const N& nbh;
	  mln_ch_value(I, P)& parent;

	  // Aux data
	  util::hqueues<P, V>&		hqueues;
	  const E&			extend; // site -> site functor.
	  value::value_array<V, bool>	is_node_at_level;
	  value::value_array<V, P>	node_at_level;
	  mln_ch_value(I, bool)		deja_vu;
	  const value::set<V>&			vset;

	  shared_flood_args(const I& f_,
			    const I& m_,
			    const N& neigb_,
			    mln_ch_value(I, P)& parent_,
			    util::hqueues<mln_psite(I), V>& hqueues_,
			    const E& extend_)
	    : f (f_),
	      m (m_),
	      nbh (neigb_),
	      parent (parent_),
	      hqueues (hqueues_),
	      extend (extend_),
	      is_node_at_level (false),
	      vset (hqueues.vset())
	  {
	      initialize(deja_vu, f);
	      mln::data::fill(deja_vu, false);
	  }
	};

	template <typename I>
	inline
	histo::array<mln_value(I)>
	compute_histo(const I& f, const I& m, mln_value(I)& hmin, mln_psite(I)& pmin)
	{
	  histo::array<mln_value(I)> hm = histo::compute(m);
	  const histo::array<mln_value(I)> hf = histo::compute(f);

	  { // Retrieve hmin.
	    unsigned i = 0;
	    while (hm[i] == 0)
	      ++i;
	    hmin = hm.vset()[i];
	  }

	  // Merge histograms.
	  for (unsigned i = 0; i < hm.nvalues(); ++i)
	    hm[i] += hf[i];

	  // Retrieve pmin.
	  mln_piter(I) p(m.domain());
	  for (p.start(); m(p) != hmin; p.next())
	    ;
	  mln_assertion(p.is_valid());
	  pmin = p;

	  return hm;
	}

	// Site -> site functor: give for all p in Domain(f), its
	// equivalence in the extended domain.
	// TODO: make it generic. It works only on boxed domain.
	template <typename I>
	struct extend
	{
	  extend(const mln_psite(I)::delta& dp)
	    : dp_ (dp)
	  {
	  }

	  mln_psite(I) operator() (const mln_psite(I)& p) const
	  {
	    return p + dp_;
	  }

	private:
	  const mln_psite(I)::delta dp_;
	};

      } // end of namespace mln::morpho::tree::internal

      namespace impl
      {

	template <typename I, typename N, typename E>
	unsigned
	flood(internal::shared_flood_args<I, N, E>& args, const unsigned h_idx)
	{
	  mln_assertion(args.is_node_at_level[h_idx]);

	  while (!args.hqueues[h_idx].empty())
	    {
	      mln_psite(I) p = args.hqueues[h_idx].pop_front();
	      unsigned p_idx = args.vset.index_of(args.f(p));

	      if (p_idx != h_idx)
		{ // Intensity mismatch: irregular case.
		  mln_psite(I) pext = args.extend(p);
		  args.parent(pext) = args.node_at_level[h_idx];

		  if (p_idx > h_idx) // Singleton with parent at h.
		    args.parent(p) = args.node_at_level[h_idx];
		  else
		    {
		      if (!args.is_node_at_level[p_idx])
			{
			  args.is_node_at_level[p_idx] = true;
			  args.node_at_level[p_idx] = p;
			}
		    }
		}

	      if (p_idx <= h_idx)
		{
		  if (!args.f.domain().has(args.node_at_level[p_idx]) ||
		      util::ord_strict(p, args.node_at_level[p_idx]))
		    { // Regular case but a representative provided by the extension.
		      args.parent(args.node_at_level[p_idx]) = p;
		      args.node_at_level[p_idx] = p;
		      //args.parent(p) = p;
		    }
		  args.parent(p) = args.node_at_level[p_idx];
		}


	      // Process the neighbors
	      mln_niter(N) n(args.nbh, p);
	      for_all(n)
		if (args.f.domain().has(n) && !args.deja_vu(n))
		  {
		    unsigned mn = args.vset.index_of(args.m(n));
		    unsigned fn = args.vset.index_of(args.f(n));
		    args.hqueues[mn].push(n);
		    args.deja_vu(n) = true;

		    mln_psite(I) ext = args.extend(n);
		    // Create a node at c.
		    {
		      mln_psite(I) node = (fn == mn) ? n : ext;
		      if (!args.is_node_at_level[mn])
			{
			  args.is_node_at_level[mn] = true;
			  args.node_at_level[mn] = node;
			}
		    }

		    while (mn > h_idx)
		      mn = flood(args, mn);
		  }
	    }

	  // Retrieve dad.
	  args.is_node_at_level[h_idx] = false;
	  unsigned c = h_idx;
	  while (c > 0 && !args.is_node_at_level[c])
	    --c;

	  mln_psite(I) x = args.node_at_level[h_idx];
	  if (c > 0)
	    args.parent(x) = args.node_at_level[c];
	  else
	    args.parent(x) = x;

	  return c;
	}

	template <typename I, typename N>
	inline
	data< I, p_array<mln_psite(I)> >
	dual_hqueue(const Image<I>& f_,
		    const Image<I>& m_,
		    const Neighborhood<N>& neibh_)
	{
	  mln_trace("mln::morpho::tree::impl::dual_hqueue");

	  const I& f = exact(f_);
	  const I& m = exact(m_);
	  const N& nbh = exact(neibh_);

	  typedef mln_psite(I) P;
	  typedef p_array<mln_psite(I)> S;

	  util::timer tm;
	  tm.start();

	  // Histo.
	  mln_psite(I) pmin;
	  mln_value(I) hmin;
	  const histo::array<mln_value(I)> histo = internal::compute_histo(f, m, hmin, pmin);
	  util::hqueues<P, mln_value(I)> hqueues(histo);

	  mln_psite(I)::delta dp(literal::zero);
	  mln_domain(I) d_ext = f.domain();
	  mln_domain(I) d = f.domain();

	  // Extend the domain.
	  dp[0] = d.pmax()[0] - d.pmin()[0] + 1;
	  d.pmax() += dp;
	  d_ext.pmin() += dp;
	  d_ext.pmax() += dp;

	  // Data.
	  mln_concrete(I)	fext;
	  mln_ch_value(I, P)	parent;
	  p_array<mln_psite(I)> s;

	  // Initialization.
	  fext = geom::translate(m, dp.to_vec(), f, d);
	  initialize(parent, fext);
	  s.reserve(geom::nsites(fext));

	  // Process.
	  internal::extend<I> extend(dp);
	  internal::shared_flood_args< I, N, internal::extend<I> >
	    args(f, m, nbh, parent, hqueues, extend);

	  unsigned r = args.vset.index_of(hmin);
	  args.deja_vu(pmin) = true;
	  args.hqueues[r].push(pmin);
	  args.node_at_level[r] = (f(pmin) == hmin) ? pmin : extend(pmin);
	  args.is_node_at_level[r] = true;
	  flood(args, r);

	  // Attach the nodes under hmin.
	  unsigned i = r;
	  do
	    {
	      if (args.is_node_at_level[i])
		{
		  parent(args.node_at_level[r]) = args.node_at_level[i];
		  r = i;
		}
	    }
	  while (i-- > 0);
	  parent(args.node_at_level[r]) = args.node_at_level[r]; //root

	  // Canonization and make tree site set.
	  {
	    mln_ch_value(I, bool) deja_vu(d_ext);
	    mln::data::fill(deja_vu, false);

	    p_array<mln_psite(I)> s_f = mln::data::sort_psites_increasing(f);
	    mln_fwd_piter(S) p(s_f); // Forward.
	    for_all(p)
	    {
	      P x = p;
	      P q = parent(p);

	      // Case: l: m <---- m <---- f
	      // Or
	      // Case  l1: m <----- f      impossible.
	      //           |
	      //       l2: m
	      mln_assertion(!(d_ext.has(q) && fext(p) == fext(q) && d_ext.has(parent(q)) && q != parent(q)));

	      while (d_ext.has(q) && !deja_vu(q) && (fext(q) != fext(parent(q)) || q == parent(q)))
		{
		  s.append(q);
		  deja_vu(q) = true;
		  x = q;
		  q = parent(q);
		}

	      if (d_ext.has(q) && fext(q) == fext(parent(q)) && q != parent(q))
		{
		  q = (parent(x) = parent(q));
		  mln_assertion(f.domain().has(q));
		}

	      if (fext(q) == fext(parent(q)))
		parent(x) = parent(q);

	      s.append(p);

	      mln_assertion((q = parent(p)) == parent(q) || fext(q) != fext(parent(q)));
	    }

	  }

	  std::cout << "Construction de l'arbre en " << tm << " s." << std::endl;

	  data<I, S> tree(fext, parent, s);


	  return tree;
	}

      }  // end of namespace mln::morpho::tree::impl

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !MLN_MORPHO_TREE_COMPUTE_PARENT_DUAL_HQUEUE_HH



