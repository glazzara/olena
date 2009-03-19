// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_FILTER_SMALL_COMPONENTS_HH
# define SCRIBO_FILTER_SMALL_COMPONENTS_HH

/// \file scribo/filter/small_components.hh
///
/// Remove small components in a binary image.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/function.hh>

# include <mln/labeling/blobs.hh>
# include <mln/labeling/relabel.hh>

# include <mln/util/array.hh>
# include <mln/value/label_16.hh>

# include <mln/pw/all.hh>

# include <scribo/util/text.hh>
# include <scribo/make/text.hh>


namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// Remove small components in a binary image.
    /// Set to 'false' all the removed components.
    ///
    /// \param[in] input_     A binary image.
    /// \param[in] nbh_	      A neighborhood used for labeling \p input_.
    /// \param[in] label_type The label type used for labeling.
    /// \param[in] min_size   The minimum cardinality of a component.
    ///
    /// \return A binary image without small components.
    template <typename I, typename N, typename V>
    mln_concrete(I)
    small_components(const Image<I>& input_,
		     const Neighborhood<N>& nbh_,
		     const V& label_type,
		     unsigned min_size);


    /// Remove too small text components.
    ///
    /// \param[in] text	      Text data.
    /// \param[in] min_size   The minimum cardinality of a component.
    ///
    /// \return updated text data.
    template <typename I>
    scribo::util::text<I>
    small_components(const scribo::util::text<I>& text,
		     unsigned min_size);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {


      /// Filter Functor. Return false for all components which are too
      /// small.
      template <typename R>
      struct filter_small_components_functor
	: Function_l2b< filter_small_components_functor<R> >
      {
	filter_small_components_functor(const mln::util::array<R>& nsitecomp,
					unsigned min_size)
	  : nsitecomp_(nsitecomp), min_size_(min_size)
	{
	}


	/// Return false if the components area is strictly inferior to
	/// \p min_size_.
	bool operator()(const value::label_16& l) const
	{
	  return nsitecomp_[l] >= min_size_;
	}


	const mln::util::array<R>& nsitecomp_;
	unsigned min_size_;
      };


    } //  end of namespace scribo::filter::internal



    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    small_components(const Image<I>& input_,
		     const Neighborhood<N>& nbh_,
		     const V& label_type,
		     unsigned min_size)
    {
      trace::entering("scribo::filter::small_components");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      V nlabels;
      mln_ch_value(I,V) lbl = labeling::blobs(input, nbh, nlabels);

      typedef accu::count<mln_psite(I)> accu_count_t;
      typedef mln_result(accu_count_t) accu_count_res_t;
      typedef mln::util::array<accu_count_res_t> nsitecomp_t;
      nsitecomp_t nsitecomp = labeling::compute(accu_count_t(), lbl, nlabels);

      typedef internal::filter_small_components_functor<accu_count_res_t> func_t;
      func_t fl2b(nsitecomp, min_size);
      labeling::relabel_inplace(lbl, nlabels, fl2b);

      mln_concrete(I) output = duplicate(input);
      data::fill((output | pw::value(lbl) == literal::zero).rw(), false);

      trace::exiting("scribo::filter::small_components");
      return output;
    }


    template <typename I>
    inline
    scribo::util::text<I>
    small_components(const scribo::util::text<I>& text,
		     unsigned min_size)
    {
      trace::entering("scribo::filter::small_components");

      mln_precondition(text.is_valid());

      typedef mln_site(I) P;
      typedef accu::count<P> accu_count_t;
      typedef mln_result(accu_count_t) accu_count_res_t;
      typedef mln::util::array<accu_count_res_t> nsitecomp_t;

      fun::i2v::array<bool> f(text.nbboxes().next(), false);
      f(0) = true;
      mln::util::array<box<P> > bresult;
      bresult.append(box<P>());
      for_all_components(i, text.bboxes())
      {
	accu_count_res_t count = set::compute(accu_count_t(), text.bbox(i));
	if (count >= min_size)
	{
	  bresult.append(text.bbox(i));
	  f(i) = true;
	}
      }

      mln_value(I) new_nbboxes;
      I new_lbl = labeling::relabel(text.label_image(), text.nbboxes(),
				    new_nbboxes, f);

      trace::exiting("scribo::filter::small_components");
      return scribo::make::text(bresult, new_lbl, new_nbboxes);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_SMALL_COMPONENTS_HH
