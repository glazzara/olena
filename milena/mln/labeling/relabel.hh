// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_LABELING_RELABEL_HH
# define MLN_LABELING_RELABEL_HH

/// \file mln/labeling/relabel.hh
///
/// Remove components and relabel a labeled image.

# include <mln/core/concept/image.hh>
# include <mln/make/relabelfun.hh>
# include <mln/level/transform.hh>
# include <mln/level/transform_inplace.hh>
# include <mln/value/label.hh>

namespace mln
{

  namespace labeling
  {

    /// Remove components and relabel a labeled image.
    /// \input[in]  label the labeled image.
    /// \input[in]  nlabels the number of labels in \p label.
    /// \input[out] new_nlabels the number of labels after relabeling.
    /// \input[in]  f function returning whether a label must be replaced
    ///		      by the background.
    ///
    /// \return the relabeled image.
    template <typename I, typename F>
    mln_concrete(I)
    relabel(const Image<I>&	    label,
	    const mln_value(I)&	    nlabels,
	    mln_value(I)&	    new_nlabels,
	    const Function_l2b<F>&  fl2b);

    /// Remove components and relabel a labeled image.
    /// \input[in]  label the labeled image.
    /// \input[in]  nlabels the number of labels in \p label.
    /// \input[out] new_nlabels the number of labels after relabeling.
    /// \input[in]  f function returning the new component id for each pixel
    /// value.
    ///
    /// \return the relabeled image.
    template <typename I, typename F>
    mln_concrete(I)
    relabel(const Image<I>&	    label,
	    const mln_value(I)&	    nlabels,
	    mln_value(I)&	    new_nlabels,
	    const Function_l2l<F>&  fl2l);

    /// Remove components and relabel a labeled image inplace.
    /// \input[in, out] label the labeled image.
    /// \input[in, out] nlabels the number of labels in \p label.
    /// \input[in]  f function returning whether a label must be replaced
    ///		      by the background.
    ///
    template <typename I, typename F>
    void
    relabel_inplace(Image<I>&		    label,
		    mln_value(I)&	    nlabels,
		    const Function_l2b<F>&  fl2b);

    /// Remove components and relabel a labeled image inplace.
    /// \input[in, out] label the labeled image.
    /// \input[in, out] nlabels the number of labels in \p label.
    /// \input[in]	f function returning the new component id for each
    /// pixel value.
    ///
    template <typename I, typename F>
    void
    relabel_inplace(Image<I>&		    label,
		    mln_value(I)&	    nlabels,
		    const Function_l2l<F>&  fl2l);


# ifndef MLN_INCLUDE_ONLY



    namespace internal
    {

      template <typename I, typename F>
      void
      relabel_tests(const Image<I>&	    label,
		    const mln_value(I)&	    nlabels,
		    mln_value(I)&	    new_nlabels,
		    const Function<F>&	    f)
      {
        // FIXME: we may want to check that it is exactly a label.
        mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
        mln_precondition(exact(label).is_valid());
	(void) label;
	(void) nlabels;
	(void) new_nlabels;
	(void) f;
      }

      template <typename I, typename F>
      inline
      void
      relabel_inplace_tests(Image<I>&		    label,
			    mln_value(I)&	    nlabels,
			    const Function<F>&	    f)
      {
        // FIXME: we may want to check that it is exactly a label.
        mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
        mln_precondition(exact(label).is_valid());
	(void) label;
	(void) nlabels;
	(void) f;
      }

    } // end of namespace mln::labeling::internal



    template <typename I, typename F>
    inline
    mln_concrete(I)
    relabel(const Image<I>&	    label,
	    const mln_value(I)&	    nlabels,
	    mln_value(I)&	    new_nlabels,
	    const Function_l2l<F>&  fl2l)
    {
      trace::entering("labeling::relabel");

      internal::relabel_tests(label, nlabels, new_nlabels, fl2l);

      mln_concrete(I) output = level::transform(label, fl2l);

      trace::exiting("labeling::relabel");
      return output;
    }



    template <typename I, typename F>
    inline
    mln_concrete(I)
    relabel(const Image<I>&	    label,
	    const mln_value(I)&	    nlabels,
	    mln_value(I)&	    new_nlabels,
	    const Function_l2b<F>&  fl2b)
    {
      trace::entering("labeling::relabel");

      internal::relabel_tests(label, nlabels, new_nlabels, fl2b);

      typedef fun::l2l::relabel<mln_value(I)> fl2l_t;
      fl2l_t fl2l = make::relabelfun(fl2b, nlabels, new_nlabels);
      mln_concrete(I) output = labeling::relabel(label, nlabels, new_nlabels, fl2l);

      trace::exiting("labeling::relabel");
      return output;
    }



    template <typename I, typename F>
    inline
    void
    relabel_inplace(Image<I>&		    label,
		    mln_value(I)&	    nlabels,
		    const Function_l2l<F>&  fl2l)
    {
      trace::entering("labeling::relabel_inplace");

      internal::relabel_inplace_tests(label, nlabels, fl2l);

      level::transform_inplace(label, fl2l);

      trace::exiting("labeling::relabel_inplace");
    }



    template <typename I, typename F>
    inline
    void
    relabel_inplace(Image<I>&		    label,
		    mln_value(I)&	    nlabels,
		    const Function_l2b<F>&  fl2b)
    {
      trace::entering("labeling::relabel_inplace");

      internal::relabel_inplace_tests(label, nlabels, fl2b);

      typedef fun::l2l::relabel<mln_value(I)> fl2l_t;
      fl2l_t fl2l = make::relabelfun(fl2b, nlabels, nlabels);
      labeling::relabel_inplace(label, nlabels, fl2l);

      trace::exiting("labeling::relabel_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_RELABEL_HH
