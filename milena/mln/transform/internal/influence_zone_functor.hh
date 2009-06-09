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

#ifndef MLN_TRANSFORM_INTERNAL_INFLUENCE_ZONE_FUNCTOR_HH
# define MLN_TRANSFORM_INTERNAL_INFLUENCE_ZONE_FUNCTOR_HH

/// \file
///
/// \brief Influence zone functor.

# include <mln/core/routine/duplicate.hh>



namespace mln
{

  namespace transform
  {

    namespace internal
    {

      template <typename I>
      struct influence_zone_functor
      {
	typedef mln_value(I) V;
	typedef mln_psite(I)  P;

	influence_zone_functor();
	influence_zone_functor(const V& background_value);

	mln_concrete(I) output;

	void init(const I& input);
	bool inqueue_p_wrt_input_p(const V& input_p);
	bool inqueue_p_wrt_input_n(const V& input_n);
	void init_p(const P&);
	void process(const P& p, const P& n);

	void init_(const I& input);
	bool inqueue_p_wrt_input_p_(const V& input_p);
	bool inqueue_p_wrt_input_n_(const V& input_n);
	void init_p_(unsigned);
	void process_(unsigned p, unsigned n);

	V background_value_;
      };


# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      inline
      influence_zone_functor<I>::influence_zone_functor()
      {
	background_value_ = literal::zero;
      }


      template <typename I>
      inline
      influence_zone_functor<I>::influence_zone_functor(const V& background_value)
      {
	background_value_ = background_value;
      }


      // Generic implementation.

      template <typename I>
      inline
      void
      influence_zone_functor<I>::init(const I& input)
      {
	output = duplicate(input);
      }

      template <typename I>
      inline
      bool
      influence_zone_functor<I>::inqueue_p_wrt_input_p(const V& input_p)
      {
	return input_p != background_value_;
      }

      template <typename I>
      inline
      bool
      influence_zone_functor<I>::inqueue_p_wrt_input_n(const V& input_n)
      {
	return input_n == background_value_;
      }

      template <typename I>
      inline
      void
      influence_zone_functor<I>::init_p(const P&)
      {
      }

      template <typename I>
      inline
      void influence_zone_functor<I>::process(const P& p, const P& n)
      {
	output(n) = output(p);
      }



      // Fastest implementation.

      template <typename I>
      inline
      void
      influence_zone_functor<I>::init_(const I& input)
      {
	output = duplicate(input);
      }

      template <typename I>
      inline
      bool
      influence_zone_functor<I>::inqueue_p_wrt_input_p_(const V& input_p)
      {
	return input_p != background_value_;
      }

      template <typename I>
      inline
      bool
      influence_zone_functor<I>::inqueue_p_wrt_input_n_(const V& input_n)
      {
	return input_n == background_value_;
      }

      template <typename I>
      inline
      void
      influence_zone_functor<I>::init_p_(unsigned)
      {
      }

      template <typename I>
      inline
      void influence_zone_functor<I>::process_(unsigned p, unsigned n)
      {
	output.element(n) = output.element(p);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::transform::internal

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_INTERNAL_INFLUENCE_ZONE_FUNCTOR_HH
