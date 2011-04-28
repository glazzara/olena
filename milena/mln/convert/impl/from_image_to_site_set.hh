// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CONVERT_IMPL_FROM_IMAGE_TO_SITE_SET_HH
# define MLN_CONVERT_IMPL_FROM_IMAGE_TO_SITE_SET_HH

/// \file
///
/// General conversion procedure from an image to a site_set.
///
/// \todo Augment code + add checks.
///
/// \todo Add (?) the case p_run-based -> site set.

# include <utility>
# include <mln/core/site_set/p_run.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/metal/converts_to.hh>



namespace mln
{

  // Forward declarations.
  template <typename E> struct Site_Set;
  template <typename E> struct Image;
  template <typename E> struct Gpoint;

  namespace convert
  {

    namespace impl
    {

      /// Conversion of an image \p from towards a site set \p to.
      template <typename I, typename S>
      void
      from_image_to_site_set(const Image<I>& from, Site_Set<S>& to);



# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	// Case: binary image -> set of point runs.

	template <typename I, typename P, typename S>
	void
	from_image_to_site_set(const I& input, const Gpoint<P>&,
			       S& s,           const mln::p_run<P>&)
	{
	  s.clear();
	  mln_fwd_piter(I) p(input.domain());
	  p.start();
	  for (;;)
	  {
	    // Skip background.
	    while (p.is_valid() && input(p) == false)
	      p.next();
	    if (! p.is_valid()) // The end.
	      break;
	    mln_invariant(input(p) == true);
	    P start = p, q;
	    // Go to run end.
	    do
	    {
	      q = p;
	      p.next();
	    }
	    while (p.is_valid() && input(p) == true &&
		// p follows q in a run, i.e., "p == q + right":
		cut_(p.to_site()) == cut_(q) && p.last_coord() == q.last_coord() + 1);
	    s.insert(p_run<P>(start, q));
	  }
	}


	template <typename I, typename P, typename S>
	void
	from_image_to_site_set(const I& input, const Gpoint<P>&,
			       S& s,
			       const std::pair< mln_value(I), p_run<P> >&)
	{
	  s.clear();
	  mln_fwd_piter(I) p(input.domain());
	  p.start();
	  for (;;)
	  {
	    if (! p.is_valid()) // The end.
	      break;
	    mln_value(I) v = input(p);
	    P start = p, q;
	    // Go to run end.
	    do
	    {
	      q = p;
	      p.next();
	    }
	    while (p.is_valid() && input(p) == v &&
		cut_(p.to_site()) == cut_(q) && p.last_coord() == q.last_coord() + 1);
	    s.insert(v, p_run<P>(start, q));
	  }
	}


	template <typename I, typename P, typename S>
	void
	from_image_to_site_set(const I& input, const Gpoint<P>&,
			       S& s,
			       const std::pair< mln_value(I), P >&)
	{
	  s.clear();
	  mln_fwd_piter(I) p(input.domain());
	  for_all(p)
	    s.insert(input(p), p);
	}


	template <typename I, typename S>
        inline
        void
        from_image_to_site_set(const Image<I>& from, Site_Set<S>& to)
        {
	  from_image_to_site_set(exact(from), mln_deduce(I, domain_t, element)(),
				 exact(to),   mln_i_element(S)());
	}

	template <typename I>
        inline
        void
        from_image_to_site_set(const Image<I>& from_, p_array<mln_psite(I)>& to)
        {
	  const I& from = exact(from_);

	  mln_piter(I) p(from.domain());
	  for_all(p)
	    if (from(p))
	      to.append(p);
	}


      } // end of namespace mln::convert::impl::internal


      // Facade.

      template <typename I, typename S>
      inline
      void
      from_image_to_site_set(const Image<I>& from, Site_Set<S>& to)
      {
	internal::from_image_to_site_set(exact(from), exact(to));
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::convert::impl

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_IMPL_FROM_IMAGE_TO_SITE_SET_HH
