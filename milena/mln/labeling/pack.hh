// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_LABELING_PACK_HH
# define MLN_LABELING_PACK_HH

/// \file mln/labeling/pack.hh
///
/// Remove components and relabel a labeled image in order to have a
/// contiguous labeling.

# include <mln/core/concept/image.hh>

# include <mln/make/relabelfun.hh>

# include <mln/level/compute.hh>
# include <mln/level/transform.hh>

# include <mln/accu/label_used.hh>



namespace mln
{

  namespace labeling
  {

    /// Relabel a labeled image in order to have a contiguous labeling.
    /// \input[in]  label	The labeled image.
    /// \input[out] new_nlabels The number of labels after relabeling.
    ///
    /// \return The relabeled image.
    //
    template <typename I>
    mln_concrete(I)
    pack(const Image<I>& label, mln_value(I)& new_nlabels);



    /// Relabel inplace a labeled image in order to have a contiguous
    /// labeling.
    /// \input[in]  label	The labeled image.
    /// \input[out] new_nlabels The number of labels after relabeling.
    //
    template <typename I>
    void
    pack_inplace(Image<I>& label, mln_value(I)& new_nlabels);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename I>
      void
      pack_tests(const Image<I>& label, mln_value(I)& new_nlabels)
      {
        // FIXME: we may want to check that it is exactly a label.
        mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
        mln_precondition(exact(label).is_valid());
	(void) label;
	(void) new_nlabels;
      }

    } // end of mln::labeling::internal



    template <typename I>
    mln_concrete(I)
    pack(const Image<I>& label, mln_value(I)& new_nlabels)
    {
      trace::entering("labeling::pack");

      internal::pack_tests(label, new_nlabels);

      fun::i2v::array<bool>
	fv2b = level::compute(accu::meta::label_used(), label);

      mln_value(I) tmp_nlabels = fv2b.size() - 1;
      mln_concrete(I)
	output = level::transform(label,
				  make::relabelfun(fv2b,
						   tmp_nlabels,
						   new_nlabels));

      trace::exiting("labeling::pack");
      return output;
    }



    template <typename I>
    void
    pack_inplace(Image<I>& label, mln_value(I)& new_nlabels)
    {
      trace::entering("labeling::pack_inplace");

      internal::pack_tests(label, new_nlabels);

      fun::i2v::array<bool>
	fv2b = level::compute(accu::meta::label_used(), label);

      mln_value(I) tmp_nlabels = fv2b.size() - 1;
      exact(label) = level::transform(label,
				      make::relabelfun(fv2b,
						       tmp_nlabels,
						       new_nlabels));

      trace::exiting("labeling::pack_inplace");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_PACK_HH
