// Copyright (C) 2007, 2008, 2009, 2010, 2011 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_DATA_FILL_HH
# define MLN_DATA_FILL_HH

/// \file
///
/// Fill an image, that is, set pixel values.
///
/// \todo Add a conversion "arr->fun" then get rid of the C array overload.

# include <mln/core/concept/function.hh>
# include <mln/pw/image.hh>
# include <mln/convert/to_fun.hh>

# include <mln/data/fill_with_image.hh>
# include <mln/data/fill_with_value.hh>


namespace mln
{

  namespace data
  {

    /*! Fill the whole image \p ima with the data provided by \p aux.
     *
     * \param[in,out] ima The image to be filled.
     * \param[in] data The auxiliary data to fill the image \p ima.
     *
     * \pre \p ima has to be initialized.
     */
    template <typename I, typename D>
    void fill(Image<I>& ima, const D& data);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      // tests

      template <typename I, typename D>
      inline
      void fill_tests(Image<I>& ima, const D&)
      {
        // Avoid a warning about an undefined variable when NDEBUG
        // is not defined.
	(void) ima;

	mlc_is(mln_trait_image_value_io(I), trait::image::value_io::read_write)::check();
	mln_precondition(exact(ima).is_valid());
	// FIXME: check for ambiguities...
      }

      // dispatch_overload

      template <typename I>
      void fill_dispatch_overload(I& ima, const mln_value(I)& v)
      {
	mln::data::fill_with_value(ima, v);
      }

      template <typename I, typename J>
      void fill_dispatch_overload(I& ima, const Image<J>& data)
      {
	mln::data::fill_with_image(ima, data);
      }

      template <typename I, typename F>
      void fill_dispatch_overload(I& ima, const Function<F>& f)
      {
	mlc_converts_to(mln_result(F), mln_value(I))::check();
	mln::data::fill_with_image(ima,
				    exact(f) | ima.domain());
      }

      template <typename I, typename R, typename A>
      void fill_dispatch_overload(I& ima, R (*f)(A))
      {
	mlc_converts_to(R, mln_value(I))::check();
	mln::data::fill_with_image(ima,
				    convert::to_fun(f) | ima.domain());
      }

      template <typename I, typename V, unsigned N>
      void fill_dispatch_overload(I& ima, V (&arr)[N])
      {
	mlc_converts_to(V, mln_value(I))::check();
	mln_precondition(N == ima.nsites());
	mln_fwd_piter(I) p(ima.domain());
	unsigned i = 0;
	for_all(p)
	  ima(p) = arr[i++];
      }

      // dispatch

      template <typename I, typename D>
      void fill_dispatch(Image<I>& ima, const D& data)
      {
	fill_dispatch_overload(exact(ima), exact(data));
      }

    } // end of namespace mln::data::internal


    // Facade.

    template <typename I, typename D>
    inline
    void fill(Image<I>& ima, const D& data)
    {
      trace::entering("data::fill");

      internal::fill_tests(ima, data);
      internal::fill_dispatch(ima, data);

      trace::exiting("data::fill");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_FILL_HH
