// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_TEST_PREDICATE_HH
# define MLN_TEST_PREDICATE_HH

/// \file
///
/// Test a predicate on the pixel values of an image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/concept/site_set.hh>


namespace mln
{

  namespace test
  {

    /// Test if all pixel values of \p ima verify the predicate \p
    /// f.
    ///
    /// \param[in] ima The image.
    /// \param[in] f The predicate.
    //
    template <typename I, typename F>
    bool predicate(const Image<I>& ima, const Function_v2b<F>& f);


    /// Test if all pixel values of \p lhs and \p rhs verify the
    /// predicate \p f.
    ///
    /// \param[in] lhs The image.
    /// \param[in] rhs The image.
    /// \param[in] f The predicate.
    //
    template <typename I, typename J, typename F>
    bool predicate(const Image<I>& lhs, const Image<J>& rhs, const Function_vv2b<F>& f);


    /// Test if all points of \p pset verify the predicate \p f.
    ///
    /// \param[in] pset The point set.
    /// \param[in] f The predicate.
    //
    template <typename S, typename F>
    bool predicate(const Site_Set<S>& pset, const Function_v2b<F>& f);


# ifndef MLN_INCLUDE_ONLY


    // Tests.

    namespace internal
    {

      template <typename I, typename F>
      inline
      void predicate_tests(const Image<I>& ima,
			   const Function_v2b<F>& f)
      {
        mln_precondition(exact(ima).is_valid());
	(void) ima;
	(void) f;
      }

      template <typename I, typename J, typename F>
      inline
      void predicate_tests(const Image<I>& lhs_,
			   const Image<J>& rhs_,
			   const Function_vv2b<F>& f)
      {
	const I& lhs = exact(lhs_);
	const J& rhs = exact(rhs_);

	mln_precondition(lhs.is_valid());
	mln_precondition(rhs.is_valid());
	mln_precondition(lhs.domain() == rhs.domain());
	(void) lhs;
	(void) rhs;
	(void) f;
      }

      template <typename S, typename F>
      inline
      void predicate_tests(const Site_Set<S>& pset,
			   const Function_v2b<F>& f)
      {
	mln_precondition(exact(pset).is_valid());
	(void) pset;
	(void) f;
      }

    } // end of namespace mln::test::internal


    // Implementations.

    namespace impl
    {

      template <typename I, typename F>
      inline
      bool predicate_(trait::image::speed::any, const I& ima, const F& f)
      {
	internal::predicate_tests(ima, f);

	mln_piter(I) p(ima.domain());
	for_all(p)
	  if (! f(ima(p)))
	    return false;
	return true;
      }

      template <typename I, typename F>
      inline
      bool predicate_(trait::image::speed::fastest, const I& ima, const F& f)
      {
	internal::predicate_tests(ima, f);

	mln_pixter(const I) pxl(ima);
	for_all(pxl)
	  if (! f(pxl.val()))
	    return false;
	return true;
      }

      template <typename I, typename J, typename F>
      inline
      bool predicate_(trait::image::speed::any,
		      trait::image::speed::any,
		      const I& lhs, const J& rhs, const F& f)
      {
	internal::predicate_tests(lhs, rhs, f);

	mln_piter(I) p(lhs.domain());
	for_all(p)
	  if (! f(lhs(p), rhs(p)))
	    return false;
	return true;
      }

      template <typename I, typename J, typename F>
      inline
      bool predicate_(trait::image::speed::fastest,
		      trait::image::speed::fastest,
		      const I& lhs, const J& rhs, const F& f)
      {
	internal::predicate_tests(lhs, rhs, f);

	mln_pixter(const I) pxl1(lhs);
	mln_pixter(const J) pxl2(rhs);
	for_all_2(pxl1, pxl2)
	  if (! f(pxl1.val(), pxl2.val()))
	    return false;
	return true;
      }

      template <typename S, typename F>
      inline
      bool predicate_(const Site_Set<S>& pset, const F& f)
      {
	internal::predicate_tests(pset, f);

	mln_piter(S) p(exact(pset));
	for_all(p)
	  if (! f(p))
	    return false;
	return true;
      }

    } // end of namespace mln::test::impl



    // Facades.


    template <typename I, typename F>
    inline
    bool predicate(const Image<I>& ima, const Function_v2b<F>& f)
    {
      mln_trace("test::predicate");

      internal::predicate_tests(ima, f);
      bool res = impl::predicate_(mln_trait_image_speed(I)(), exact(ima),
				  exact(f));

      return res;
    }


    template <typename I, typename J, typename F>
    inline
    bool predicate(const Image<I>& lhs_, const Image<J>& rhs_, const Function_vv2b<F>& f)
    {
      mln_trace("test::predicate");

      const I& lhs = exact(lhs_);
      const J& rhs = exact(rhs_);

      internal::predicate_tests(lhs_, rhs_, f);

      bool res = impl::predicate_(mln_trait_image_speed(I)(),
				  mln_trait_image_speed(J)(),
				  lhs, rhs,
				  exact(f));

      return res;
    }

    template <typename S, typename F>
    inline
    bool predicate(const Site_Set<S>& pset, const Function_v2b<F>& f)
    {
      mln_trace("test::predicate");

      internal::predicate_tests(pset, f);

      bool res = impl::predicate_(exact(pset), exact(f));

      return res;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::test

} // end of namespace mln


#endif // ! MLN_TEST_PREDICATE_HH
