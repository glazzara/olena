// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LABELING_PACK_HH
# define MLN_LABELING_PACK_HH

/// \file
///
/// Remove components and relabel a labeled image in order to have a
/// contiguous labeling.

# include <mln/core/concept/image.hh>

# include <mln/make/relabelfun.hh>

# include <mln/data/compute.hh>
# include <mln/data/transform.hh>

# include <mln/accu/label_used.hh>



namespace mln
{

  namespace labeling
  {

    /// Relabel a labeled image in order to have a contiguous labeling.
    ///
    /// \param[in]  label	The labeled image.
    /// \param[out] new_nlabels The number of labels after relabeling.
    /// \param[out] repack_fun  The function used to repack the labels.
    ///
    /// \return The relabeled image.
    //
    template <typename I>
    mln_concrete(I)
    pack(const Image<I>& label, mln_value(I)& new_nlabels,
	 fun::i2v::array<mln_value(I)>& repack_fun);


    /// \overload
    template <typename I>
    mln_concrete(I)
    pack(const Image<I>& label, mln_value(I)& new_nlabels);



    /// Relabel inplace a labeled image in order to have a contiguous
    /// labeling.
    ///
    /// \param[in]  label	The labeled image.
    /// \param[out] new_nlabels The number of labels after relabeling.
    /// \param[out] repack_fun  The function used to repack the labels.
    //
    template <typename I>
    void
    pack_inplace(Image<I>& label, mln_value(I)& new_nlabels,
		 fun::i2v::array<mln_value(I)>& repack_fun);


    /// \overload
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
        //mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
        mln_precondition(exact(label).is_valid());
	(void) label;
	(void) new_nlabels;
      }

    } // end of mln::labeling::internal



    template <typename I>
    mln_concrete(I)
    pack(const Image<I>& label, mln_value(I)& new_nlabels)
    {
      fun::i2v::array<mln_value(I)> repack_fun;
      return pack(label, new_nlabels, repack_fun);
    }


    template <typename I>
    mln_concrete(I)
    pack(const Image<I>& label, mln_value(I)& new_nlabels,
	 fun::i2v::array<mln_value(I)>& repack_fun)
    {
      mln_trace("labeling::pack");

      internal::pack_tests(label, new_nlabels);

      fun::i2v::array<bool>
	fv2b = data::compute(accu::meta::label_used(), label);

      mln_value(I) tmp_nlabels = fv2b.size() - 1;

      repack_fun = make::relabelfun(fv2b, tmp_nlabels, new_nlabels);

      mln_concrete(I)
	output = data::transform(label, repack_fun);

      return output;
    }


    template <typename I>
    void
    pack_inplace(Image<I>& label, mln_value(I)& new_nlabels)
    {
      fun::i2v::array<mln_value(I)> repack_fun;
      pack_inplace(label, new_nlabels, repack_fun);
    }


    template <typename I>
    void
    pack_inplace(Image<I>& label, mln_value(I)& new_nlabels,
		 fun::i2v::array<mln_value(I)>& repack_fun)
    {
      mln_trace("labeling::pack_inplace");

      internal::pack_tests(label, new_nlabels);

      fun::i2v::array<bool>
	fv2b = data::compute(accu::meta::label_used(), label);

      mln_value(I) tmp_nlabels = fv2b.size() - 1;
      repack_fun = make::relabelfun(fv2b, tmp_nlabels, new_nlabels);

      exact(label) = data::transform(label, repack_fun);

    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_PACK_HH
