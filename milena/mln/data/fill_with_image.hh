// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_DATA_FILL_WITH_IMAGE_HH
# define MLN_DATA_FILL_WITH_IMAGE_HH

/*! \file mln/data/fill_with_image.hh
 *
 * \brief Fill an image with the values from another image.
 *
 * \todo Specialize with line_piter...
 */

# include <mln/core/concept/image.hh>


// Specializations are in:
# include <mln/data/fill_with_image.spe.hh>


namespace mln
{

  namespace data
  {

    /// Fill the image \p ima with the values of the image \p data.
    ///
    /// \param[in,out] ima The image to be filled.
    /// \param[in] data The image.
    ///
    /// \warning The definition domain of \p ima has to be included in
    /// the one of \p data.
    ///
    /// \pre \p ima.domain <= \p data.domain.
    ///
    /// \{
    template <typename I, typename J>
    void fill_with_image(Image<I>& ima, const Image<J>& data);
    /// \}



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename I, typename J>
      inline
      void fill_with_image_tests(Image<I>& ima, const Image<J>& data)
      {
	// Avoid a warning about an undefined variable when NDEBUG
	// is not defined.
	(void)ima;
	(void)data;

	mlc_is(mln_trait_image_pw_io(I),
	       mln::trait::image::pw_io::read_write)::check();
	mlc_converts_to(mln_value(J), mln_value(I))::check();
	mln_precondition(exact(ima).has_data());
	mln_precondition(exact(data).has_data());
	mln_precondition(exact(ima).domain() <= exact(data).domain());
      }

    } // end of namespace mln::data::internal


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename J>
	void fill_with_image(Image<I>& ima_, const Image<J>& data_)
	{
	  trace::entering("data::impl::generic::fill_with_image");
	  I& ima = exact(ima_);
	  const J& data = exact(data_);

	  data::internal::fill_with_image_tests(ima, data);

	  mln_piter(I) p(ima.domain());
	  for_all(p)
	    ima(p) = data(p);

	  trace::exiting("data::impl::generic::fill_with_image");
	}

      } // end if namespace mln::data::impl::generic

    } // end of namespace mln::data::impl


    // Facade.

    template <typename I, typename J>
    inline
    void fill_with_image(Image<I>& ima, const Image<J>& data)
    {
      trace::entering("data::fill_with_image");

      internal::fill_with_image_(ima, data);

      trace::exiting("data::fill_with_image");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_FILL_WITH_IMAGE_HH
