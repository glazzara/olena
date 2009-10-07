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

#ifndef COMPUTE_ATTRIBUTE_IMAGE_HH
# define COMPUTE_ATTRIBUTE_IMAGE_HH
///
///
/// \brief  Compute an attribute on a morphological tree.
///
/// \todo Specialize for fast image.
///


# include <mln/core/image/attribute_image.hh>
# include <mln/core/concept/tree.hh>
# include <mln/core/concept/accumulator.hh>

# include <mln/data/fill.hh>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      template <typename T, typename A>
      attribute_image<T, mln_result(A)>
      compute_attribute_image(const Tree<T>& tree,
			      const Accumulator<A>& accu);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename T, typename A>
	inline
	void
	take_as_init(trait::accumulator::when_pix::use_none,
		     const T&, A& accu,
		     const mln_psite(T)&)
	{
	  accu.take();
	}

	template <typename T, typename A>
	inline
	void
	take_as_init(trait::accumulator::when_pix::use_pix,
		     const T& tree, A& accu,
		     const mln_psite(T)& p)
	{
	  accu.take(make::pix(p, tree.f(p)));
	}


	template <typename T, typename A>
	inline
	void
	take_as_init(trait::accumulator::when_pix::use_v,
		     const T& tree, A& accu,
		     const mln_psite(T)& p)
	{
	  accu.take(tree.f(p));
	}

	template <typename T, typename A>
	inline
	void
	take_as_init(trait::accumulator::when_pix::use_p,
		     const T&, A& accu,
		     const mln_psite(T)& p)
	{
	  accu.take(p);
	}

	template <typename T, typename A>
	void
	take_as_init(const T& tree, A& a, const mln_psite(T)& p)
	{
	  take_as_init(mln_trait_accumulator_when_pix(A)(), tree, a, p);
	}

      } // end of namespace namespace mln::morpho::tree::internal

      namespace impl
      {

	namespace generic
	{

	  template <typename T, typename A>
	  attribute_image<T, mln_result(A)>
	  compute_attribute_image(const T& tree, const A& accu, attribute_image<T, A>* accu_img = 0)
	  {
	    attribute_image<T, A> a(tree);
	    (void)  accu_img;

	    // Transmit dynamic data of accumulator (ex: rank filter).
	    data::fill(a, accu);

	    // Initialize. Each node takes its content.
	    {
	      mln_fwd_piter(T::domain_t) p(tree.domain());
	      for_all(p)
		internal::take_as_init(tree, a.element(tree.node_at_(p)), p);
	    }

	    // Transmit to parent.
	    for (unsigned i = tree.n_nodes() - 1; i > 0; --i)
	      {
		unsigned q = tree.parent_at_(i);
		if (q != i) // The current node is not a root.
		  a.element(q).take(a.element(i));
	      }

	    attribute_image<T, mln_result(A)> out(tree);
	    for (unsigned i = 0; i < tree.n_nodes(); i++)
	      out.element(i) = a.element(i).to_result();

	    return out;
	  }

	} // end of namespace mln::morpho::tree::impl::generic

      } // end of namespace mln::morpho::tree::impl


      // Facades
      template <typename T, typename A>
      attribute_image<T, mln_result(A)>
      compute_attribute_image(const Tree<T>& tree_, const Accumulator<A>& acc)
      {
	trace::entering("mln::morpho::tree::compute_attribute_image");
	const T& tree = exact(tree_);
	const A& accu = exact(acc);

	mln_precondition(tree.is_valid());

	return impl::generic::compute_attribute_image(tree, accu);
	trace::exiting("mln::morpho::tree::compute_attribute_image");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !COMPUTE_ATTRIBUTE_IMAGE_HH
