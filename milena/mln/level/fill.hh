// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_LEVEL_FILL_HH
# define MLN_LEVEL_FILL_HH

/*! \file mln/level/fill.hh
 *
 * \brief Fill an image, that is, set pixel values.
 *
 * \todo Re-organize this file contents + Overload for fastest images.
 */

# include <cstring>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/inplace.hh>
# include <mln/level/memset_.hh>


// Specializations are in:
# include <mln/level/fill.spe.hh>

namespace mln
{

  namespace level
  {

    /*! Fill the whole image \p ima with the single value \p v.
     *
     * \param[in,out] ima The image to be filled.
     * \param[in] v The value to assign to all pixels.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Optimize when \p ima is large and sizeof(mln_value(I)) > 1.
     */
    template <typename I>
    void fill(Image<I>& ima, const mln_value(I)& v);


    /*! Fill the whole image \p ima with the function \p f.
     *
     * \param[in,out] ima The image to be filled.
     * \param[in] f The function.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Take benefit from quantization when possible.
     */
    template <typename I, typename F>
    void fill(Image<I>& ima, const Function_p2v<F>& f);


    /*! Fill the image \p ima by applying the function \p f.
     *
     * \param[in,out] ima The image to be filled.
     * \param[in] f The function that defines the value of every pixel.
     *
     * The signature of \p f has to be:
     * " value f(const point& p) "
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Take benefit from quantization when possible.
     */
    template <typename I>
    void fill(Image<I>& ima,  mln_value(I) (*(&f))(const mln_point(I)& p));

    template <typename I>
    void fill_f(Image<I>& ima,  mln_value(I) (*f)(const mln_point(I)& p));


    /*! Fill the image \p ima with the values given by the array \p arr.
     *
     * \param[in,out] ima The image to be filled.
     * \param[in] arr The array of values.
     *
     * \warning The size of the array has to be larger than the number
     * of image points, otherwise the program crashes.
     *
     * \pre \p ima has to be initialized.
     * \pre N == \p ima.npoints
     */
    template <typename I, unsigned N>
    void fill(Image<I>& ima, mln_value(I) (&arr)[N]);


    /*! Fill the image \p ima with the values of the image \p data.
     *
     * \param[in,out] ima The image to be filled.
     * \param[in] data The image.
     *
     * \warning The definition domain of \p ima has to be included in
     * the one of \p data.
     *
     * \pre \p ima.domain <= \p data.domain.
     *
     * \todo Use memcpy when possible.
     */
    template <typename I, typename J>
    void fill(Image<I>& ima, const Image<J>& data);




# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {
	template <typename I>
	inline
	void fill_with_value(I& ima, const mln_value(I)& value)
	{
	  trace::entering("level::impl::generic::fill_with_value");

	  mln_piter(I) p(ima.domain());
	  for_all(p)
	    ima(p) = value;

	  trace::exiting("level::impl::generic::fill_with_value");
	}

      } // end if namespace mln::level::impl::generic

    } // end of namespace mln::level::impl



    // with: value

    template <typename I>
    inline
    void fill(Image<I>& ima, const mln_value(I)& value)
    {
      trace::entering("level::fill");

      mlc_is(mln_trait_image_io(I), trait::image::io::write)::check(); // FIXME: Only the upcoming general facade!!! 
      mln_precondition(exact(ima).has_data());
      impl::fill_with_value(mln_trait_image_speed(I)(), exact(ima),
			    value);

      trace::exiting("level::fill");
    }


    // with: Function_p2v<F>

    template <typename I, typename F>
    inline
    void fill(Image<I>& ima_, const Function_p2v<F>& f_)
    {
      trace::entering("level::fill");

      I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      const F& f = exact(f_);
      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = f(p);

      trace::exiting("level::fill");
    }


    // with: value f(const point&)

    template <typename I>
    inline
    void fill_f(Image<I>& ima_,
		mln_value(I) (*f)(const mln_point(I)& p))
    {
      trace::entering("level::fill_f");

      mln_precondition(f != 0);
      I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = f(p);

      trace::exiting("level::fill_f");
    }


    // with: value arr[N]

    template <typename I, unsigned N>
    inline
    void fill(Image<I>& ima_, mln_value(I) (&arr)[N])
    {
      trace::entering("level::fill");

      I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      mln_precondition(N == ima.npoints());
      mln_piter(I) p(ima.domain());
      unsigned i = 0;
      for_all(p)
	ima(p) = arr[i++];

      trace::exiting("level::fill");
    }


    // with: Image<J>

    template <typename I, typename J>
    inline
    void fill(Image<I>& ima_, const Image<J>& data_)
    {
      trace::entering("level::fill");

      I&        ima = exact(ima_);
      const J& data = exact(data_);
      mln_precondition(ima.domain() <= data.domain());

      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = data(p);

      trace::exiting("level::fill");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_FILL_HH
