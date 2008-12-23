// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRANSFORM_INTERNAL_INFLUENCE_ZONE_FUNCTOR_HH
# define MLN_TRANSFORM_INTERNAL_INFLUENCE_ZONE_FUNCTOR_HH

/// \file mln/transform/internal/influence_zone_functor.hh
///
/// Influence zone functor.

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
	typedef mln_site(I)  P;

	mln_concrete(I) output;

	void init(const I& input);
	bool inqueue_p_wrt_input_p(const V& input_p);
	bool inqueue_p_wrt_input_n(const V& input_n);
	void process(const P& p, const P& n);
      };


# ifndef MLN_INCLUDE_ONLY

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
	return input_p != 0u;
      }

      template <typename I>
      inline
      bool
      influence_zone_functor<I>::inqueue_p_wrt_input_n(const V& input_n)
      {
	return input_n == 0u;
      }

      template <typename I>
      inline
      void influence_zone_functor<I>::process(const P& p, const P& n)
      {
	output(n) = output(p);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::transform::internal

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_INTERNAL_INFLUENCE_ZONE_FUNCTOR_HH
