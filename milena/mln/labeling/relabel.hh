// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
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

#ifndef MLN_LABELING_RELABEL_HH
# define MLN_LABELING_RELABEL_HH

/// \file
///
/// Remove components and relabel a labeled image.


# include <mln/core/concept/image.hh>

# include <mln/make/relabelfun.hh>

# include <mln/data/transform.hh>
# include <mln/data/transform_inplace.hh>



namespace mln
{

  namespace labeling
  {

    /// Remove components and relabel a labeled image.
    /// \param[in]  label the labeled image.
    /// \param[in]  nlabels the number of labels in \p label.
    /// \param[out] new_nlabels the number of labels after relabeling.
    /// \param[in]  fv2b function returning whether a label must be replaced
    ///		      by the background.
    ///
    /// \return the relabeled image.
    //
    template <typename I, typename F>
    mln_concrete(I)
    relabel(const Image<I>&	    label,
	    const mln_value(I)&	    nlabels,
	    mln_value(I)&	    new_nlabels,
	    const Function_v2b<F>&  fv2b);


    /// Remove components and relabel a labeled image.
    /// \param[in]  label the labeled image.
    /// \param[in]  nlabels the number of labels in \p label.
    /// \param[in]  fv2v function returning the new component id for each pixel
    /// value.
    ///
    /// \return the relabeled image.
    //
    template <typename I, typename F>
    mln_concrete(I)
    relabel(const Image<I>&	    label,
	    const mln_value(I)&	    nlabels,
	    const Function_v2v<F>&  fv2v);


    /// Remove components and relabel a labeled image inplace.
    /// \param[in, out] label the labeled image.
    /// \param[in]      nlabels the number of labels in \p label.
    /// \param[in]      fv2b function returning whether a label must be replaced
    ///		        by the background.
    //
    template <typename I, typename F>
    void
    relabel_inplace(Image<I>&		    label,
		    const mln_value(I)&	    nlabels,
		    const Function_v2b<F>&  fv2b);


    /// Remove components and relabel a labeled image inplace.
    /// \param[in, out] label the labeled image.
    /// \param[in]      nlabels the number of labels in \p label.
    /// \param[in]	fv2v function returning the new component id for each
    /// pixel value.
    //
    template <typename I, typename F>
    void
    relabel_inplace(Image<I>&		    label,
		    const mln_value(I)&	    nlabels,
		    const Function_v2v<F>&  fv2v);


# ifndef MLN_INCLUDE_ONLY



    namespace internal
    {

      template <typename I>
      void
      relabel_tests(const Image<I>&	    label,
		    mln_value(I)&	    new_nlabels)
      {
        // FIXME: we may want to check that it is exactly a label.
//        mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
        mln_precondition(exact(label).is_valid());
	(void) label;
	(void) new_nlabels;
      }

      template <typename I, typename F>
      void
      relabel_tests(const Image<I>&	    label,
		    const mln_value(I)&	    nlabels,
		    mln_value(I)&	    new_nlabels,
		    const Function<F>&	    f)
      {
        // FIXME: we may want to check that it is exactly a label.
//        mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
        mln_precondition(exact(label).is_valid());
	(void) label;
	(void) nlabels;
	(void) new_nlabels;
	(void) f;
      }

      template <typename I, typename F>
      void
      relabel_tests(const Image<I>&	    label,
		    const mln_value(I)&	    nlabels,
		    const Function<F>&	    f)
      {
        // FIXME: we may want to check that it is exactly a label.
//        mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
        mln_precondition(exact(label).is_valid());
	(void) label;
	(void) nlabels;
	(void) f;
      }

      template <typename I, typename F>
      inline
      void
      relabel_inplace_tests(Image<I>&		    label,
			    const mln_value(I)&	    nlabels,
			    const Function<F>&	    f)
      {
        // FIXME: we may want to check that it is exactly a label.
//        mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
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
	    const Function_v2v<F>&  fv2v)
    {
      mln_trace("labeling::relabel");


      mlc_not_equal(mln_result(F),bool)::check();
      internal::relabel_tests(label, nlabels, fv2v);

      mln_concrete(I) output = data::transform(label, fv2v);

      return output;
    }



    template <typename I, typename F>
    inline
    mln_concrete(I)
    relabel(const Image<I>&	    label,
	    const mln_value(I)&	    nlabels,
	    mln_value(I)&	    new_nlabels,
	    const Function_v2b<F>&  fv2b)
    {
      mln_trace("labeling::relabel");

      internal::relabel_tests(label, nlabels, new_nlabels, fv2b);

      typedef fun::i2v::array<mln_value(I)> fv2v_t;
      fv2v_t fv2v = make::relabelfun(fv2b, nlabels, new_nlabels);
      mln_concrete(I) output = labeling::relabel(label, new_nlabels, fv2v);

      return output;
    }


    template <typename I, typename F>
    inline
    void
    relabel_inplace(Image<I>&		    label,
		    const mln_value(I)&	    nlabels,
		    const Function_v2v<F>&  fv2v)
    {
      mln_trace("labeling::relabel_inplace");

      mlc_not_equal(mln_result(F),bool)::check();
      internal::relabel_inplace_tests(label, nlabels, fv2v);

      data::transform_inplace(label, fv2v);

    }



    template <typename I, typename F>
    inline
    void
    relabel_inplace(Image<I>&		    label,
		    const mln_value(I)&	    nlabels,
		    const Function_v2b<F>&  fv2b)
    {
      mln_trace("labeling::relabel_inplace");

      internal::relabel_inplace_tests(label, nlabels, fv2b);

      typedef fun::i2v::array<mln_value(I)> fv2v_t;
      mln_value(I) tmp;
      fv2v_t fv2v = make::relabelfun(fv2b, nlabels, tmp);
      labeling::relabel_inplace(label, tmp, fv2v);

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_RELABEL_HH
