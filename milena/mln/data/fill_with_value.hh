// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_DATA_FILL_WITH_VALUE_HH
# define MLN_DATA_FILL_WITH_VALUE_HH

/// \file
///
/// Fill an image with a value, that is, set all pixel values to
/// the given value.
///
/// \todo Move specializations in a extra file.
/// \todo Activate fill_with_value_cell_wise.
/// \todo Optimize when \p ima is large and sizeof(mln_value(I)) > 1.

# include <cstdlib>

# include <mln/core/concept/image.hh>



// Specializations are in:
# include <mln/data/fill_with_value.spe.hh>


namespace mln
{

  namespace data
  {

    /// Fill the whole image \p ima with the single value \p v.
    ///
    /// \param[in,out] ima The image to be filled.
    /// \param[in] val The value to assign to all sites.
    ///
    /// \pre \p ima has to be initialized.
    ///
    /// \{
    template <typename I, typename V>
    void fill_with_value(Image<I>& ima, const V& val);
    /// \}



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename I, typename V>
      inline
      void fill_with_value_tests(Image<I>& ima, const V&)
      {
	mlc_converts_to(mln_exact(V), mln_value(I))::check();
	mln_precondition(exact(ima).is_valid());

	// Avoid a warning about an undefined variable when NDEBUG
	(void) ima;
      }

    } // end of namespace mln::data::internal


    namespace impl
    {

      namespace generic
      {

	/// Fill the whole image \p ima with the single value \p v.
	///
	/// \param[in,out] ima_ The image to be filled.
	/// \param[in] val The value to assign to all sites.
	///
	/// \pre \p ima has to be initialized.
	///
	//
	template <typename I, typename V>
	void fill_with_value(Image<I>& ima_, const V& val)
	{
	  mln_trace("data::impl::generic::fill_with_value");

	  I& ima = exact(ima_);

	  internal::fill_with_value_tests(ima, val);
          mlc_is(mln_trait_image_pw_io(I),
                 trait::image::pw_io::read_write)::check();

	  mln_value(I) v = static_cast<mln_value(I)>(exact(val));
	  mln_piter(I) p(ima.domain());
	  for_all(p)
	    ima(p) = v;

	}

      } // end if namespace mln::data::impl::generic

    } // end of namespace mln::data::impl


    // Facade.

    template <typename I, typename V>
    inline
    void fill_with_value(Image<I>& ima, const V& val)
    {
      mln_trace("data::fill_with_value");

      internal::fill_with_value_tests(ima, val);
      internal::fill_with_value_dispatch(ima, val);

    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_FILL_WITH_VALUE_HH
