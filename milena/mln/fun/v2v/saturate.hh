// Copyright (C) 2007, 2009, 2010 EPITA Research and Development
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

#ifndef MLN_FUN_V2V_SATURATE_HH
# define MLN_FUN_V2V_SATURATE_HH

/*! \file
 *
 * \brief FIXME.
 */

# include <mln/core/concept/function.hh>
# include <mln/metal/converts_to.hh>
# include <mln/trait/value_.hh>
# include <mln/value/cast.hh>


namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      // FIXME: Doc!

      template <typename V>
      struct saturate : public Function_v2v< saturate<V> >
      {
	saturate();
	saturate(const V& min, const V& max);

	typedef V result;

	template <typename W>
	V operator()(const W& w) const;

      protected:
	V min_, max_;
	mutable bool needs_update_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      saturate<V>::saturate()
	: min_(mln_min(V)),
	  max_(mln_max(V))
      {
	needs_update_ = true;
      }

      template <typename V>
      inline
      saturate<V>::saturate(const V& min, const V& max)
	: min_(min),
	  max_(max)
      {
	mln_precondition(max > min);
	needs_update_ = true;
      }

      template <typename V>
      template <typename W>
      inline
      V
      saturate<V>::operator()(const W& w) const
      {
	// FIXME: Check that W is a larger type than V; otherwise
	// alt code.

	static W min_W, max_W;
	if (needs_update_)
	  {
	    min_W = mln::value::cast<W>(min_);
	    max_W = mln::value::cast<W>(max_);
 	    needs_update_ = false;
	  }

	// FIXME: Below we need something more powerful that mlc_converts_to
	// for instance, with W=int_s<10u> and V=int_u<8u>, it does not
	// works cause the cast is not obvious...
	// mlc_converts_to(W, V)::check();

	if (w < min_W)
	  return min_;
	if (w > max_W)
	  return max_;

	V output;
	convert::from_to(w, output);
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2V_SATURATE_HH
