// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
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

#ifndef MLN_BORDER_FILL_HH
# define MLN_BORDER_FILL_HH

/*! \file
 *
 * \brief Define function that fills border using a given value.
 */

# include <cstring>

# include <mln/core/concept/image.hh>
# include <mln/core/box_runstart_piter.hh>
# include <mln/opt/element.hh>


namespace mln
{

  namespace border
  {

    /*! \brief Fill the virtual (outer) border of image \p ima with
     *  the single value \p v.
     *
     * \param[in,out] ima The image whose border is to be filled.
     * \param[in] v The value to assign to all border pixels.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Optimize with memset if possible.
     *
     * \ingroup mlnborderext
     */
    template <typename I>
    void fill(const Image<I>& ima, const mln_value(I)& v);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename I>
      inline
      void fill_tests(const Image<I>& ima, const mln_value(I)&)
      {
	mln_precondition(exact(ima).is_valid());
	(void) ima;
      }

    } // end of namespace mln::border::internal


    namespace impl
    {

      template <typename I>
      inline
      void fill_size_1(const Image<I>& ima_, const mln_value(I)& v)
      {
	mln_trace("border::impl::fill_size_1");

	const I& ima = exact(ima_);
	internal::fill_tests(ima, v);

	typedef mln_psite(I) P;
	typedef mln_psite(I) P;
	mln_box_runstart_piter(I) pl(ima.domain());

	unsigned len_r = pl.run_length();
	unsigned st = 0;

	for_all (pl)
	{
	  unsigned end = ima.offset_of_point (pl);
	  if (st < end)
	    std::memset((void*)&opt::element(ima, st),
			*(const int*)(&v),
			end - st);
	  st = end + len_r;
	}
	if (st < opt::nelements(ima))
	  std::memset((void*)&opt::element(ima, st),
		      *(const int*)(&v),
		      opt::nelements(ima) - st);

      }


      template <typename I>
      inline
      void fill_size_n(const I& ima_, const mln_value(I)& v)
      {
	mln_trace("border::impl::fill_size_n");

	I& ima = const_cast<I&>( exact(ima_) );
	internal::fill_tests(ima, v);

	typedef mln_psite(I) P;
	mln_box_runstart_piter(I) pl(ima.domain());
	unsigned len_r = pl.run_length();
	unsigned st = 0;

	for_all (pl)
	{
	  unsigned end = ima.offset_of_point (pl);
	  for (unsigned i = st; i < end; ++i)
	    opt::element(ima, i) = v;
	  st = end + len_r;
	}
	for (unsigned i = st; i < opt::nelements(ima); ++i)
          opt::element(ima, i) = v;

      }


    } // end of namespace mln::border::impl


    namespace internal
    {

      // Dispatch.

      template <typename I>
      inline
      void fill_dispatch(const Image<I>& ima, const mln_value(I)& v);

      template <typename I>
      inline
      void fill_dispatch(mln::trait::image::category::primary,
			 mln::trait::image::speed::fastest,
			 I& ima, const mln_value(I)& v)
      {
	if (sizeof(mln_value(I)) == 1)
	  impl::fill_size_1(ima, v);
	else
	  impl::fill_size_n(ima, v);
      }

      template <typename I>
      inline
      void fill_dispatch(mln::trait::image::category::primary,
			 mln::trait::image::speed::any,
			 I& /* ima */, const mln_value(I)& /* v */)
      {
	// No border so no-op.
      }

      template <typename I>
      inline
      void fill_dispatch(mln::trait::image::category::morpher,
			 mln::trait::image::speed::any,
			 I& ima, const mln_value(I)& v)
      {
	fill_dispatch(ima.unmorph_(), v);
      }

      template <typename I>
      inline
      void fill_dispatch(const Image<I>& ima_, const mln_value(I)& v)
      {
	I& ima = const_cast<I&>(exact(ima_));
	fill_dispatch(mln_trait_image_category(I)(),
		      mln_trait_image_speed(I)(),
		      ima, v);
      }

    } // end of namespace mln::border::internal



    // Facade.

    template <typename I>
    inline
    void fill(const Image<I>& ima, const mln_value(I)& v)
    {
      mln_trace("border::fill");

      internal::fill_tests(ima, v);
      internal::fill_dispatch(ima, v);

    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_FILL_HH
