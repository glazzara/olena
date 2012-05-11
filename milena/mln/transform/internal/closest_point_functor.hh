// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_TRANSFORM_INTERNAL_CLOSEST_POINT_FUNCTOR_HH
# define MLN_TRANSFORM_INTERNAL_CLOSEST_POINT_FUNCTOR_HH

/// \file
///
/// Closest point functor.

# include <mln/core/site_set/p_array.hh>
# include <mln/core/macros.hh>
# include <mln/data/fill.hh>
# include <mln/trait/ch_value.hh>

namespace mln
{

  namespace transform
  {

    namespace internal
    {


      /// Functor to be passed to a canvas.
      /// Computes an image of closest points.
      ///
      /// The image values are initialized with \p pset.nsites().
      template <typename I>
      struct closest_point_functor_with_indexes
      {
	typedef mln_value(I) V;
	typedef mln_psite(I) P;

	closest_point_functor_with_indexes(const p_array<P>& pset);

	mln_ch_value(I,unsigned) cp_ima;
	const p_array<P>& pset_;

	/// Generic version interface.
	void init(const I&);
	bool inqueue_p_wrt_input_p(const V& input_p);
	void init_p(const P&);
	bool inqueue_p_wrt_input_n(const V& input_n);
	void process(const P&, const P&);

	/// Fast version interface.
	void init_(const I& input);
	bool inqueue_p_wrt_input_p_(const V& input_p);
	void init_p_(unsigned);
	bool inqueue_p_wrt_input_n_(const V& input_n);
	void process_(unsigned p, unsigned n);
      };

      /// Functor to be passed to a canvas.
      /// Computes an image of closest points.
      ///
      /// The image values are initialized with \p literal::origin.
      template <typename I>
      struct closest_point_functor_with_sites
      {
	typedef mln_value(I) V;
	typedef mln_psite(I) P;

	closest_point_functor_with_sites();

	mln_ch_value(I,P) cp_ima;

	/// Generic version interface.
	void init(const I&);
	bool inqueue_p_wrt_input_p(const V& input_p);
	void init_p(const P&);
	bool inqueue_p_wrt_input_n(const V& input_n);
	void process(const P&, const P&);

	/// Fast version interface.
	void init_(const I& input);
	bool inqueue_p_wrt_input_p_(const V& input_p);
	void init_p_(unsigned p);
	bool inqueue_p_wrt_input_n_(const V& input_n);
	void process_(unsigned p, unsigned n);
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      closest_point_functor_with_indexes<I>::closest_point_functor_with_indexes(
				    const p_array<mln_psite(I)>& pset)
	: pset_(pset)
      {
      }

      template <typename I>
      inline
      void
      closest_point_functor_with_indexes<I>::init(const I& input)
      {
	initialize(cp_ima, input);
	data::fill(cp_ima, pset_.nsites());

	mln_piter(p_array<mln_psite(I)>) p(pset_);
	for_all(p)
	  cp_ima(p) = p.index();
      }

      template <typename I>
      inline
      bool
      closest_point_functor_with_indexes<I>::inqueue_p_wrt_input_p(const V& input_p)
      {
	return input_p == true;
      }

      template <typename I>
      inline
      void
      closest_point_functor_with_indexes<I>::init_p(const P&)
      {
      }

      template <typename I>
      inline
      bool
      closest_point_functor_with_indexes<I>::inqueue_p_wrt_input_n(const V& input_n)
      {
	return input_n == false;
      }

      template <typename I>
      inline
      void closest_point_functor_with_indexes<I>::process(const P& p, const P& n)
      {
	cp_ima(n) = cp_ima(p);
      }



      template <typename I>
      inline
      void
      closest_point_functor_with_indexes<I>::init_(const I& input)
      {
	init(input);
      }

      template <typename I>
      inline
      bool
      closest_point_functor_with_indexes<I>::inqueue_p_wrt_input_p_(const V& input_p)
      {
	return input_p == true;
      }

      template <typename I>
      inline
      void closest_point_functor_with_indexes<I>::init_p_(unsigned)
      {
      }

      template <typename I>
      inline
      bool
      closest_point_functor_with_indexes<I>::inqueue_p_wrt_input_n_(const V& input_n)
      {
	return input_n == false;
      }

      template <typename I>
      inline
      void
      closest_point_functor_with_indexes<I>::process_(unsigned p, unsigned n)
      {
	cp_ima.element(n) = cp_ima.element(p);
      }


      ///				   ///
      /// closest_point_functor_with_sites ///
      ///				   ///


      template <typename I>
      inline
      closest_point_functor_with_sites<I>::closest_point_functor_with_sites()
      {
      }

      template <typename I>
      inline
      void
      closest_point_functor_with_sites<I>::init(const I& input)
      {
	initialize(cp_ima, input);
	data::fill(cp_ima, literal::origin);
      }

      template <typename I>
      inline
      bool
      closest_point_functor_with_sites<I>::inqueue_p_wrt_input_p(const V& input_p)
      {
	return input_p == true;
      }

      template <typename I>
      inline
      void
      closest_point_functor_with_sites<I>::init_p(const P& p)
      {
	cp_ima(p) = p;
      }

      template <typename I>
      inline
      bool
      closest_point_functor_with_sites<I>::inqueue_p_wrt_input_n(const V& input_n)
      {
	return input_n == false;
      }

      template <typename I>
      inline
      void
      closest_point_functor_with_sites<I>::process(const P& p, const P& n)
      {
	cp_ima(n) = cp_ima(p);
      }



      template <typename I>
      inline
      void
      closest_point_functor_with_sites<I>::init_(const I& input)
      {
	init(input);
      }

      template <typename I>
      inline
      bool
      closest_point_functor_with_sites<I>::inqueue_p_wrt_input_p_(const V& input_p)
      {
	return input_p == true;
      }

      template <typename I>
      inline
      void closest_point_functor_with_sites<I>::init_p_(unsigned p)
      {
	cp_ima.element(p) = cp_ima.point_at_offset(p);
      }

      template <typename I>
      inline
      bool
      closest_point_functor_with_sites<I>::inqueue_p_wrt_input_n_(const V& input_n)
      {
	return input_n == false;
      }

      template <typename I>
      inline
      void
      closest_point_functor_with_sites<I>::process_(unsigned p, unsigned n)
      {
	cp_ima.element(n) = cp_ima.element(p);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::transform::internal

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_INTERNAL_CLOSEST_POINT_FUNCTOR_HH
