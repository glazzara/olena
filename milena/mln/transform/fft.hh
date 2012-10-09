// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_TRANSFORM_FFT_HH
# define MLN_TRANSFORM_FFT_HH

#  include <mln/core/image/image2d.hh>
#  include <mln/estim/min_max.hh>
#  include <mln/opt/at.hh>

#  include <complex>

#  include <fftw3.h>

namespace mln {

  namespace internal {

    /// Dispatch traits for fftw
    enum fft_dispatch { fft_cplx, fft_real };

    template <typename T>
    struct fft_trait;

    /*!
    ** \brief fft trait.
    */
    template <>
    struct fft_trait<double>
    {
      static const fft_dispatch		which = fft_real; ///< Real dispatch.
      typedef double			fftw_input; ///< Type of input.
    };

    /*!
    ** \brief fft trait.
    **
    ** \specialization for ntg::cplx<R, T>
    */
    template <typename T>
    struct fft_trait< std::complex<T> >
    {
      static const fft_dispatch		which = fft_cplx; ///< Complex dispatch.
      typedef std::complex <T>		fftw_input; ///< Type of input.
    };

    /*!
    ** _fft<ntg::cplx_representation, T>
    **
    ** \param T Data type.
    */
    template <class T>
    class _fft
    {
    public:
      /*!
      ** \brief Accessor to transformed image.
      **
      ** Const version.
      */
      const image2d< std::complex<T> > transformed_image() const
      {
	return trans_im;
      }

      /*!
      ** \brief Accessor to transformed image.
      **
      ** Non const version.
      */
      image2d< std::complex<T> >& transformed_image()
      {
	return trans_im;
      }

      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      ** |T[p]|.
      **
      ** \tparam R Data type of the resulting image.
      **
      ** \param ordered Kind of traversal.
      */
      template <class R>
      image2d<R> transformed_image_magn(bool ordered = true) const
      {
	// FIXME : check that R is real

	image2d<R> new_im(trans_im.domain());

	if (ordered)
	  for (unsigned row = 0; row < new_im.nrows(); ++row)
	    for (unsigned col = 0; col < new_im.ncols(); ++col)
	      opt::at(new_im, row, col) =
		std::norm(opt::at(trans_im, (row + trans_im.nrows() / 2) % trans_im.nrows(),
				      (col + trans_im.ncols() / 2) % trans_im.ncols()));
	else
	  {
	    mln_piter(image2d< std::complex<T> >) it(trans_im.domain());

	    for_all(it)
	      new_im(it) = std::norm(trans_im(it));
	  }

	return new_im;
      }

      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      ** |T[p]|.
      **
      ** \param ordered Kind of traversal.
      */
      image2d<T> transformed_image_magn(bool ordered = true) const
      {
	return transformed_image_magn<T>(ordered);
      }

      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      **  a clipped value of |T[p]|.\n
      **
      ** \param R Data type of the resulting image.
      **
      ** \param clip Value used for clipping.
      ** \param ordered Kind of traversal.
      */

      template <class R>
      image2d<R> transformed_image_clipped_magn(const double clip,
						bool ordered = true) const
      {
	// Check that R is real

	image2d<R> new_im(trans_im.domain());
	// check that clip is >=0 and <=1 ?
	double max;
	mln_piter(image2d<T>) it(trans_im.domain());

	for_all(it)
	  if (std::norm(trans_im(it)) > max)
	    max = std::norm(trans_im(it));

	if (ordered)
	  for (unsigned row = 0; row < new_im.nrows(); ++row)
	    for (unsigned col = 0; col < new_im.ncols(); ++col)
	      {
		if (std::norm(opt::at(trans_im, (row + trans_im.nrows() / 2) % trans_im.nrows(),
					  (col + trans_im.ncols() / 2) %
					  trans_im.ncols())) >= max * clip)
		  opt::at(new_im, row, col) = mln_max(R);
		else
		  opt::at(new_im, row, col) =
		    (double) mln_max(R) *
		    std::norm(opt::at(trans_im, (row + trans_im.nrows() / 2) % trans_im.nrows(),
					  (col + trans_im.ncols() / 2) %
					  trans_im.ncols())) / (max * clip);
      }
	else
	  {
	    for_all(it)
	    {
	      if (std::norm(trans_im(it)) >= max * clip)
		new_im(it) = mln_max(R);
	      else
		new_im(it) = (double) mln_max(R) *
		  std::norm(trans_im(it)) / (max * clip);
	    }
	  }

	return new_im;
      }

      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      **  a clipped value of |T[p]|.\n
      **
      ** \param clip Value used for clipping.
      ** \param ordered Kind of traversal.
      */
      image2d<T> transformed_image_clipped_magn(const double clip,
						bool ordered = true) const
      {
	return transformed_image_clipped_magn<T>(clip, ordered);
      }

      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      ** a clipped value of |T[p]|.\n
      **
      ** \param R Data type of the resulting image.
      **
      ** \param ordered Kind of traversal.
      */

      image2d<T> transformed_image_clipped_magn(bool ordered = true) const
      {
	return transformed_image_clipped_magn<T>(1, ordered);
      }

      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      ** a clipped value of |T[p]|.\n
      **
      ** \param R Data type of the resulting image.
      **
      ** \param ordered Kind of traversal.
      */

      template <class R>
      image2d<R> transformed_image_clipped_magn(bool ordered = true) const
      {
	return transformed_image_clipped_magn<R>(1, ordered);
      }

      // FIXME: Find a more elegant way to fix range interval on a and b.
      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      ** a log translated value of |T[p]| on interval [a; b].\n
      **
      ** \param a Lower bound.
      ** \param b Upper bound.
      ** \param ordered Kind of traversal.
      */

      template <class R>
      image2d<R> transformed_image_log_magn(double a,
					    double b,
					    bool ordered = true) const
      {
	// Check that R is real
	// 0 <= a <= 1000
	// 0 <= b <= 1000

	image2d<R> new_im(trans_im.domain());

	double max = 0;
	mln_piter(image2d<R>) it(trans_im.domain());

	for_all(it)
	  if (std::norm(trans_im(it)) > max)
	    max = std::norm(trans_im(it));


	if (ordered)
	  for (unsigned row = 0; row < new_im.nrows(); ++row)
	    for (unsigned col = 0; col < new_im.ncols(); ++col)
	      opt::at(new_im, row, col) =
		log(a + b * std::norm(opt::at(trans_im, (row + trans_im.nrows() / 2) % trans_im.nrows(),
						  (col + trans_im.ncols() / 2) % trans_im.ncols()))) /
		log (a + b * max) * mln_max(R);
	else
	  {
	    mln_piter(image2d< std::complex<T> >) it(trans_im.domain());

	    for_all(it)
	      new_im(it) = log(a + b * std::norm(trans_im(it))) /
	      log (a + b * max) * mln_max(R);
	  }

	return new_im;
      }

      // FIXME: Find a more elegant way to fix boundaries of a and b.
      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      ** a log translated value of |T[p]| on interval [a; b].\n
      **
      ** \param a Lower bound.
      ** \param b Upper bound.
      ** \param ordered Kind of traversal.
      */

	image2d<T> transformed_image_log_magn(double a,
					      double b,
					      bool ordered = true) const
      {
	return transformed_image_log_magn<T>(a, b, ordered);
      }

      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      ** a log translated value of |T[p]| on interval [1; 100].\n
      **
      ** \param R Data type of the resulting image.
      **
      ** \param ordered Kind of traversal.
      */

      template <class R>
      image2d<R> transformed_image_log_magn(bool ordered = true) const
      {
	return transformed_image_log_magn<R>(1, 100, ordered);
      }

      /*!
      ** \brief Accessor to transformed image.
      **
      ** For each point p of the transformed image T, you get
      ** a log translated value of |T[p]| on interval [1; 100].\n
      **
      ** \param ordered Kind of traversal.
      */

      image2d<T> transformed_image_log_magn(bool ordered = true) const
      {
	return transformed_image_log_magn<T>(1, 100, ordered);
      }


      /*!
      ** \brief Destructor.
      **
      ** Let memory free for other big images !!!
      */
      ~_fft()
      {
	fftw_free(in);
	fftw_free(out);
	fftw_destroy_plan(p);
	fftw_destroy_plan(p_inv);
      }

    protected:

      typename fft_trait<T>::fftw_input	*in; ///< Input image.
      std::complex<T>			*out; ///< Complex image.
      fftw_plan				p; ///< Plan.
      fftw_plan				p_inv; ///< inverted plan.
      image2d< std::complex<T> >	trans_im; ///< Transformed image.

    };

  } // end of namespace internal

  namespace transform {

    /*!
    ** \brief fft class declaration.
    **
    ** \param T Data type.
    ** \param which Dispatch.
    */
    template <class T,
	      internal::fft_dispatch which = internal::fft_trait<T>::which >
    class fft;

    /*!
    ** \brief fft specialization for fft real dispatch.
    **
    ** \param T Data type.
    */
    template <class T>
    class fft<T, internal::fft_real> : public internal::_fft<T>
    {

    public:

      /*!
      ** \brief Constructor.
      **
      ** Initialization of data in order to compute the fft.
      **
      ** \param original_im Image to process.
      */
      template <typename D>
      fft(const image2d<D>& original_im)
      {
	this->in  = (T*) fftw_malloc(original_im.nrows() * original_im.ncols() * sizeof(T));
	this->out = (std::complex<T>*)
	  fftw_malloc(original_im.nrows() * (original_im.ncols() / 2 + 1) * sizeof(std::complex<T>));

	for (unsigned row = 0; row < original_im.nrows(); ++row)
	  for (unsigned col = 0; col < original_im.ncols(); ++col)
	    this->in[row * original_im.ncols() + col] = opt::at(original_im, row, col);

	this->p = fftw_plan_dft_r2c_2d (original_im.nrows(), original_im.ncols(),
					this->in, reinterpret_cast<fftw_complex*>(this->out), FFTW_ESTIMATE);
	this->p_inv = fftw_plan_dft_c2r_2d (original_im.nrows(), original_im.ncols(),
					    reinterpret_cast<fftw_complex*>(this->out), this->in, FFTW_ESTIMATE);

	this->trans_im = image2d< std::complex<T> >(original_im.domain());
      }

      /*!
      ** \brief Compute and return the transform.
      */
      image2d< std::complex<T> > transform()
      {
	fftw_execute(this->p);

	unsigned denom = this->trans_im.nrows() * this->trans_im.ncols();
	int i = 0;
	for (unsigned row = 0; row < this->trans_im.nrows(); ++row)
	  for (unsigned col = 0; col <= this->trans_im.ncols() / 2; ++col)
	    {
	      this->out[i] = std::complex<T> (this->out[i].real() / denom, this->out[i].imag() / denom);
	      opt::at(this->trans_im, row, col) = this->out[i];
	      ++i;
	    }
	for (unsigned row = 0; row < this->trans_im.nrows(); ++row)
	  for (unsigned col = this->trans_im.ncols() - 1; col > this->trans_im.ncols() / 2; --col)
	    opt::at(this->trans_im, row, col) = opt::at(this->trans_im, this->trans_im.nrows() - row - 1,
							    this->trans_im.ncols() - col - 1);
	return this->trans_im;
      }

      /*!
      ** \brief Compute and return the invert transform.
      **
      ** \param R Data type of output image.
      */
      template <class R>
      image2d<R> transform_inv()
      {
	// FIXME : Check that R is real

	for (unsigned row = 0; row < this->trans_im.nrows(); ++row)
	  for (unsigned col = 0; col <= this->trans_im.ncols() / 2; ++col)
	    this->out[row * (this->trans_im.ncols() / 2 + 1) + col] =
	      opt::at(this->trans_im, row, col);

	fftw_execute(this->p_inv);

	image2d<R> new_im(this->trans_im.domain());
	int i = 0;
	for (unsigned row = 0; row < this->trans_im.nrows(); ++row)
	  for (unsigned col = 0; col < this->trans_im.ncols(); ++col)
	    {
	      opt::at(new_im, row, col) = (this->in[i] >= mln_min(R) ?
				     (this->in[i] <= mln_max(R) ?
				      (R)this->in [i] :
				      mln_min(R)) :
				     mln_max(R));
	      ++i;
	    }
	return new_im;
      }

      /*!
      ** \brief Shift zero-frequency component of discrete Fourier transform
      ** to center of spectrum.
      **
      ** \param R The data type of the image returned.
      **
      ** The zero-frequency component of discrete Fourier transform are moved
      ** to center of the image :
      **
      ** \htmlonly
      ** <table>
      **   <tr><td>1</td><td>2</td></tr>
      **   <tr><td>3</td><td>4</td></tr>
      ** </table>
      ** becomes
      ** <table>
      **   <tr><td>4</td><td>3</td></tr>
      **   <tr><td>2</td><td>1</td></tr>
      ** </table>
      ** \endhtmlonly
      **
      */

      /*
      template <class R>
      image2d<R> shift_transform_inv()
      {
	image2d<R> t = transform_inv<R>();
	image2d<R> st(t.size());

	// We have to exchange t_1 with t_1_dest and not directly t_3 because
	// they have not he same size.
	typedef morpher::piece_morpher< image2d<R> > piece_t;
	piece_t t_1(t, dpoint2d(0, 0),
		    image2d_size((t.size().nrows() - 1) / 2,
				 (t.size().ncols() - 1) / 2,
				 t.border()));
	piece_t t_1_dest(st, dpoint2d(t.nrows() - t_1.nrows(),
				     t.ncols() - t_1.ncols()),
			 image2d_size(t_1.nrows(), t_1.ncols(),
				      t.border()));
	piece_t t_2(t, dpoint2d(0, (t.size().ncols() - 1) / 2),
		    image2d_size((t.size().nrows() - 1) / 2,
				 t.size().ncols() - (t.size().ncols() - 1) / 2,
				 t.border()));
	piece_t t_2_dest(st, dpoint2d(t.nrows() - t_2.nrows(), 0),
			 image2d_size(t_2.nrows(), t_2.ncols(),
				      t.border()));
	piece_t t_3(t, dpoint2d((t.size().nrows() - 1) / 2, 0),
		    image2d_size(t.size().nrows() - (t.size().nrows() - 1) / 2,
				 (t.size().ncols() - 1) / 2,
				 t.border()));
	piece_t t_3_dest(st, dpoint2d(0, t.ncols() - t_3.ncols()),
			 image2d_size(t_3.nrows(), t_3.ncols(),
				      t.border()));
	piece_t t_4(t, dpoint2d((t.size().nrows() - 1) / 2,
				(t.size().ncols() - 1) / 2),
		    image2d_size(t.size().nrows() - (t.size().nrows() - 1) / 2,
				 t.size().ncols() - (t.size().ncols() - 1) / 2,
				 t.border()));
	piece_t t_4_dest(st, dpoint2d(0, 0),
			 image2d_size(t_4.nrows(), t_4.ncols(),
				      t.border()));

	oln_iter_type(piece_t) i1(t_1);
	for_all(i1)
	  t_1_dest[i1] = t_1[i1];
	oln_iter_type(piece_t) i2(t_2);
	for_all(i2)
	  t_2_dest[i2] = t_2[i2];
	oln_iter_type(piece_t) i3(t_3);
	for_all(i3)
	  t_3_dest[i3] = t_3[i3];
	oln_iter_type(piece_t) i4(t_4);
	for_all(i4)
	  t_4_dest[i4] = t_4[i4];

	return st;
      }
      */


      /*!
      ** \brief Compute and return the invert transform.
      */
      image2d<T> transform_inv()
      {
	return transform_inv<T>();
      }

      /*!
      ** \brief Shift zero-frequency component of discrete Fourier transform
      ** to center of spectrum.
      */
      /*
      image2d<T> shift_transform_inv()
      {
	return shift_transform_inv<T>();
      }
      */

    };

    /*!
    ** \brief fft specialization for fft complex dispatch.
    **
    ** \param T Data type.
    ** \param R Complex representation kind.
    */
    template <class T>
    class fft<T, internal::fft_cplx> : public internal::_fft<T>
    {

    public:

      /*!
      ** \brief Constructor.
      **
      ** Initialization of data in order to compute the fft.
      **
      ** \param original_im Image to process.
      */
      fft(const image2d< std::complex<T> >& original_im)
      {
	this->in = fftw_malloc(sizeof(std::complex<T>) *
			       original_im.nrows() * original_im.ncols());
	this->out = fftw_malloc(sizeof(std::complex<T>) *
				original_im.nrows() * original_im.ncols());

	for (unsigned row = 0; row < original_im.nrows(); ++row)
	  for (unsigned col = 0; col < original_im.ncols(); ++col)
	    {
	      this->in[row * original_im.ncols() + col].re =
		original_im(row, col).real();
	      this->in[row * original_im.ncols() + col].im =
		original_im(row, col).imag();
	    }

	this->p = fftw_plan_dft_2d(original_im.nrows(), original_im.ncols(),
				   this->in, this->out,
				   FFTW_FORWARD, FFTW_ESTIMATE);
	this->p_inv = fftw_plan_dft_2d(original_im.nrows(), original_im.ncols(),
				       this->out, this->in,
				       FFTW_BACKWARD, FFTW_ESTIMATE);

	this->trans_im = image2d< std::complex<T> >(original_im.domain());
      }

      /*!
      ** \brief Compute and return the transform.
      */
      image2d< std::complex<T> > transform()
      {
	fftw_execute(this->p);

	unsigned denom = this->trans_im.nrows() * this->trans_im.ncols();
	int i = 0;
	for (unsigned row = 0; row < this->trans_im.nrows(); ++row)
	  for (unsigned col = 0; col < this->trans_im.ncols(); ++col)
	    {
	      this->out[i].re /= denom;
	      this->out[i].im /= denom;
	      this->trans_im(row, col) = std::complex<double>(this->out[i].re,
							 this->out[i].im);
	      ++i;
	    }
	return this->trans_im;
      }

      /*!
      ** \brief Compute and return the invert transform.
      **
      ** \param R Data type of output image.
      */
      template <class R>
      image2d< std::complex<R> > transform_inv()
      {
	for (unsigned row = 0; row < this->trans_im.nrows(); ++row)
	  for (unsigned col = 0; col < this->trans_im.ncols(); ++col)
	    {
	      this->out[row * this->trans_im.ncols() + col].re = this->trans_im(row, col).real();
	      this->out[row * this->trans_im.ncols() + col].im = this->trans_im(row, col).imag();
	    }

	fftw_execute(this->p_inv);

	image2d< std::complex<R> > new_im(this->trans_im.size());
	int i = 0;
	for (unsigned row = 0; row < this->trans_im.nrows(); ++row)
	  for (unsigned col = 0; col < this->trans_im.ncols(); ++col)
	    {
	      new_im(row, col) = this->in[i];
	      ++i;
	    }
	return new_im;
      }

      /*!
      ** \brief Compute and return the invert transform.
      */
      image2d< std::complex<T> > transform_inv()
      {
	return transform_inv<T>();
      }

    };

  } // end of namespace transform

} // end of namespace oln

#endif // ! MLN_TRANSFORM_FFT_HH
