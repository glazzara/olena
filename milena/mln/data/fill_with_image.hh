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

#ifndef MLN_DATA_FILL_WITH_IMAGE_HH
# define MLN_DATA_FILL_WITH_IMAGE_HH

/// \file
///
/// Fill an image with the values from another image.
///
/// \todo Specialize with line_piter...

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
    template <typename I, typename J>
    void fill_with_image(Image<I>& ima, const Image<J>& data);



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
	mln_precondition(exact(ima).is_valid());
	mln_precondition(exact(data).is_valid());
	mln_precondition(exact(ima).domain() <= exact(data).domain());
      }

    } // end of namespace mln::data::internal


    namespace impl
    {

      namespace generic
      {

	/// Generic implementation.
	///
	/// \param[in,out] ima_ The image to be filled.
	/// \param[in] data_ The image.
	//
	template <typename I, typename J>
	void fill_with_image(Image<I>& ima_, const Image<J>& data_)
	{
	  mln_trace("data::impl::generic::fill_with_image");
	  I& ima = exact(ima_);
	  const J& data = exact(data_);

	  data::internal::fill_with_image_tests(ima, data);

	  mln_piter(I) p(ima.domain());
	  for_all(p)
	    ima(p) = static_cast<mln_value(I)>(data(p));

	}

      } // end if namespace mln::data::impl::generic

    } // end of namespace mln::data::impl


    // Facade.

    template <typename I, typename J>
    inline
    void fill_with_image(Image<I>& ima, const Image<J>& data)
    {
      mln_trace("data::fill_with_image");

      internal::fill_with_image_(ima, data);

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_FILL_WITH_IMAGE_HH
