// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRANSFORM_INTERNAL_DISTANCE_FUNCTOR_HH
# define MLN_TRANSFORM_INTERNAL_DISTANCE_FUNCTOR_HH

/// \file
///
/// Distance functor.
///
/// \todo Move all functors into their corresponding file.

# include <mln/core/macros.hh>



namespace mln
{

  namespace transform
  {

    namespace internal
    {

      template <typename I>
      struct distance_functor
      {
	typedef mln_value(I) V;
	typedef mln_site(I)  P;

	void init(const I&);
	bool inqueue_p_wrt_input_p(const V& input_p);
	void init_p(const P&);
	bool inqueue_p_wrt_input_n(const V& input_n);
	void process(const P&, const P&);


	void init_(const I&) {}
	bool inqueue_p_wrt_input_p_(const V& input_p) { return input_p == true; }
	void init_p_(unsigned) {}
	bool inqueue_p_wrt_input_n_(const V& input_n) { return input_n == false; }
	void process_(unsigned, unsigned) {}
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void
      distance_functor<I>::init(const I&)
      {
      }

      template <typename I>
      inline
      bool
      distance_functor<I>::inqueue_p_wrt_input_p(const V& input_p)
      {
	return input_p == true;
      }

      template <typename I>
      inline
      void
      distance_functor<I>::init_p(const P&)
      {
      }

      template <typename I>
      inline
      bool
      distance_functor<I>::inqueue_p_wrt_input_n(const V& input_n)
      {
	return input_n == false;
      }

      template <typename I>
      inline
      void distance_functor<I>::process(const P&, const P&)
      {
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::transform::internal

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_INTERNAL_DISTANCE_FUNCTOR_HH
