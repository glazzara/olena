// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_LINEAR_LOCAL_CONVOLVE_HH
# define MLN_LINEAR_LOCAL_CONVOLVE_HH

/// \file
///
/// Local convolution at a point.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/site.hh>
# include <mln/core/concept/generalized_pixel.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/metal/const.hh>



namespace mln
{

  namespace linear
  {

    namespace local
    {

      /*! \brief Local convolution of image \p input at point \p p by the
          weighted window \p w_win.

         \warning Computation of the \p result is performed with the
         type \c R.

         \warning The weighted window is used as-is, considering that
         its symmetrization is handled by the client.

	 \ingroup mlnlinear
       */
      template <typename I, typename P, typename W, typename R>
      void convolve(const Image<I>&           input,
		    const Site<P>&	      p,
		    const Weighted_Window<W>& w_win,
		    R&			      result);


      /*! \brief Local convolution around (generalized) pixel \p by the
          weighted window \p w_win.

         \warning Computation of the \p result is performed with the
         type \c R.

         \warning The weighted window is used as-is, considering that
         its symmetrization is handled by the client.

	 \ingroup mlnlinear
       */
      template <typename P, typename W, typename R>
      void convolve(const Generalized_Pixel<P>& p,
		    const Weighted_Window<W>&   w_win,
		    R&				result);


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	template <typename I, typename P, typename W, typename R>
	inline
	void convolve(trait::image::speed::any,
		      const I&	      input,
		      const Site<P>&  p_,
		      const W&        w_win,
		      R&	      result)
	{
	  const P& p = exact(p_);

	  R tmp = literal::zero; // FIXME: zero?
	  mln_qiter(W) q(w_win, p);
	  for_all(q) if (input.has(q))
	    tmp += input(q) * q.w();
	  result = tmp;
	}

	template <typename I, typename P, typename W, typename R>
	inline
	void convolve(trait::image::speed::fastest,
		      const I&	      input,
		      const Site<P>&  p_,
		      const W&	      w_win,
		      R&	      result)
	{
	  const P& p = exact(p_);

	  mln_precondition(input.border() >= w_win.delta());

	  R tmp = 0;
	  unsigned i = 0;
	  mln_qixter(const I, W) q(input, w_win, p);
	  for_all(q)
	    tmp += w_win.w(i++) * q.val();
	  result = tmp;
	}

	template <typename P, typename W, typename R>
	inline
	void convolve(const Generalized_Pixel<P>& p_,
		      const W&                    w_win,
		      R&			  result)
	{
	  const P& p = mln::internal::force_exact<P>(p_);
	  mln_precondition(p.ima().border() >= w_win.delta());

	  R tmp = 0;
	  unsigned i = 0;
	  // FIXME: mln_qixter(const P, W) should work
	  // FIXME: so make the trait make this job...
	  mln_qixter(mlc_const(mln_image(P)), W) q(p, w_win);
	  for_all(q)
	    tmp += w_win.w(i++) * q.val();
	  result = tmp;
	}

      } // end of namespace mln::linear::impl


      // Facades.

      template <typename I, typename P, typename W, typename R>
      inline
      void convolve(const Image<I>&	      input,
		    const Site<P>&	      p,
		    const Weighted_Window<W>& w_win,
		    R&			      result)
      {
	mln_precondition(exact(input).is_valid());
	impl::convolve(mln_trait_image_speed(I)(), exact(input),
		       p, exact(w_win), result);
      }

      template <typename P, typename W, typename R>
      inline
      void convolve(const Generalized_Pixel<P>& p,
		    const Weighted_Window<W>&   w_win,
		    R&				result)
      {
	impl::convolve(p, exact(w_win), result);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::linear::local

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_LOCAL_CONVOLVE_HH
