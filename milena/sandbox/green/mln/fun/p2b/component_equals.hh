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

#ifndef MLN_FUN_P2B_COMPONENT_EQUALS_HH
# define MLN_FUN_P2B_COMPONENT_EQUALS_HH

/// \file
///
/// In the vectorial image context (as RGB, HSL, HSV), compare one component
/// to a specific value.

# include <mln/core/alias/point2d.hh>
# include <mln/core/concept/function.hh>
# include <mln/trait/value/comp.hh>


namespace mln
{

  namespace fun
  {

    namespace p2b
    {

      /// \brief Functor that compare the i-th component of a value.
      // V is for the type of the value received
      // i is the ith component to select
      template <typename I, unsigned i>
      struct component_equals : public Function_v2b< component_equals<I,i> >
      {
	typedef bool result;
	bool operator()(const point2d& p) const;

	component_equals(const I& img,
			 const mln_trait_value_comp(mln_value(I),i)& ref);

	const mln_trait_value_comp(mln_value(I),i) _ref;
	const I&                                   _img;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I, unsigned i>
      inline
      component_equals<I,i>::
      component_equals(const I& img,
		       const mln_trait_value_comp(mln_value(I),i)& ref)
	: _ref(ref), _img(img)
      {
      }

      template <typename I, unsigned i>
      inline
      bool
      component_equals<I,i>::operator()(const point2d& p) const
      {
	typedef mln_value(I)                    t_value;
	typedef mln_trait_value_comp(t_value,i) mln_value_comp;

	t_value        v = _img(p);
	mln_value_comp c = trait::value::internal::comp<t_value,i>::on(v);

	return (c == _ref);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2b

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2B_COMPONENT_EQUALS_HH
