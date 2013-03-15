// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///
///

#ifndef SCRIBO_PRIMITIVE_MERGE_COMPONENTS_HH
# define SCRIBO_PRIMITIVE_MERGE_COMPONENTS_HH

# include <mln/core/concept/function.hh>
# include <mln/core/image/vmorph/fun_image.hh>
# include <mln/data/fill.hh>
# include <mln/pw/all.hh>

# include <scribo/core/component_set.hh>


namespace scribo
{

  namespace primitive
  {

    namespace merge
    {

      using namespace mln;


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename V>
	struct rebase_label : mln::Function_v2v<rebase_label<V> >
	{
	  typedef V result;

	  rebase_label()
	    : base_label_(0)
	  {}

	  rebase_label(const V& base_label)
	    : base_label_(base_label)
	  {}

	  V operator()(const V& v) const
	  {
	    if (v)
	      return unsigned(base_label_) + unsigned(v);
	    return literal::zero;
	  }


	  V base_label_;
	};


      } // end of namespace scribo::primitive::merge


      template <typename L>
      component_set<L>
      components(const component_set<L>& lhs, const component_set<L>& rhs)
      {
	mln_trace("scribo::primitive::merge::components");

	const L& lhs_lbl = lhs.labeled_image();
	const L& rhs_lbl = rhs.labeled_image();

	typedef mln_value(L) V;


	V nlabels = unsigned(lhs.nelements()) + unsigned(rhs.nelements());

	// Merge labeled images.
	//
	// Rebase labeling in the second image, according to the
	// number of labels in the first one.
	//
	L lbl_merge = duplicate(lhs_lbl);

	internal::rebase_label<V> f(lhs.nelements());
	fun_image<internal::rebase_label<V>, L> fima(f, rhs_lbl);
	data::fill((lbl_merge | (pw::value(lbl_merge) == pw::cst(0))).rw(), fima);

 	component_set<L> output(lbl_merge, nlabels);

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::merge

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_MERGE_COMPONENTS_HH
