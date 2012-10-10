// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory
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

#ifndef MLN_TRANSFORM_FFT_HH
# define MLN_TRANSFORM_FFT_HH

# include <complex>

# include <fftw3.h>

# include <mln/core/image/image2d.hh>
# include <mln/estim/min_max.hh>
# include <mln/opt/at.hh>


namespace mln
{

  namespace internal
  {

    /// Dispatch tags for FFT traits.
    enum fft_dispatch { fft_cplx, fft_real };


    /// \brief FFT traits.
    template <typename T>
    struct fft_trait;

    /// \brief FFT traits for `double'.
    template <>
    struct fft_trait<double>
    {
      /// Type of dispatch (real numbers).
      static const fft_dispatch	which = fft_real;
      /// Type of input (double-precision floating point numbers).
      typedef double fftw_input;
    };

    /// \brief FFT traits for `std::complex<T>'.
    template <typename T>
    struct fft_trait< std::complex<T> >
    {
      /// Type of dispatch (complex numbers).
      static const fft_dispatch which = fft_cplx;
      /// Type of input (complex numbers of the standard C++ library).
      typedef std::complex<T> fftw_input;
    };


    /// \brief Internal structure containining packing data and
    /// instructions for the (forward) and inverse (backward)
    /// transforms.
    ///
    /// \tparam T Data type.
    template <class T>
    class fft
    {
    public:
      /// \brief Accessor to transformed image (const version).
      const image2d< std::complex<T> >& transformed_image() const;

      /// \brief Accessor to transformed image (non const version).
      image2d< std::complex<T> >& transformed_image();

      /// \brief Accessor to the transformed image (magnitude).
      ///
      /// For each point \p p of the transformed image \p T,
      /// an image containing <tt>|T[p]|</tt> is returned.
      ///
      /// \tparam R Value type of the resulting image.
      ///
      /// \param ordered Kind of traversal.
      template <class R>
      image2d<R> transformed_image_magn(bool ordered = true) const;

      /// \brief Accessor to the transformed image (magnitude).
      ///
      /// For each point \p p of the transformed image \p T,
      /// an image containing <tt>|T[p]|</tt> is returned.
      ///
      /// \param ordered Kind of traversal.
      image2d<T> transformed_image_magn(bool ordered = true) const;

      /// \brief Accessor to the transformed image (clipped
      /// magnitude).
      ///
      /// For each point \p p of the transformed image \p T, an image
      /// containing a clipped value of <tt>|T[p]|</tt> is returned.
      ///
      /// \tparam R Value type of the resulting image.
      ///
      /// \param clip    Value used for clipping.
      /// \param ordered Kind of traversal.
      template <class R>
      image2d<R> transformed_image_clipped_magn(double clip,
						bool ordered = true) const;

      /// \brief Accessor to the transformed image (clipped
      /// magnitude).
      ///
      /// For each point \p p of the transformed image \p T, an image
      /// containing a clipped value of <tt>|T[p]|</tt> is returned.
      ///
      /// \param clip    Value used for clipping.
      /// \param ordered Kind of traversal.
      image2d<T> transformed_image_clipped_magn(double clip,
						bool ordered = true) const;

      /// \brief Accessor to the transformed image (clipped
      /// magnitude).
      ///
      /// For each point \p p of the transformed image \p T, an image
      /// containing a clipped value of <tt>|T[p]|</tt> is returned.
      ///
      /// \tparam R Value type of the resulting image.
      ///
      /// \param ordered Kind of traversal.
      template <class R>
      image2d<R> transformed_image_clipped_magn(bool ordered = true) const;

      /// \brief Accessor to the transformed image (clipped
      /// magnitude).
      ///
      /// For each point \p p of the transformed image \p T, an image
      /// containing a clipped value of <tt>|T[p]|</tt> is returned.
      ///
      /// \param ordered Kind of traversal.
      image2d<T> transformed_image_clipped_magn(bool ordered = true) const;

      /// \brief Accessor to the transformed image (log of the
      /// magnitude).
      ///
      /// For each point \p p of the transformed image \p T, an image
      /// containing a log value of <tt>|T[p]|</tt> translated in the
      /// interval [a, b].
      ///
      /// \tparam R Value type of the resulting image.
      ///
      /// \param a       Lower bound.
      /// \param b       Upper bound.
      /// \param ordered Kind of traversal.
      template <class R>
      image2d<R> transformed_image_log_magn(double a, double b,
					    bool ordered = true) const;

      /// \brief Accessor to the transformed image (log of the
      /// magnitude).
      ///
      /// For each point \p p of the transformed image \p T, an image
      /// containing a log value of <tt>|T[p]|</tt> translated in the
      /// interval [a, b].
      ///
      /// \param a       Lower bound.
      /// \param b       Upper bound.
      /// \param ordered Kind of traversal.
      image2d<T> transformed_image_log_magn(double a, double b,
					    bool ordered = true) const;

      /// \brief Accessor to the transformed image (log of the
      /// magnitude).
      ///
      /// For each point \p p of the transformed image \p T, an image
      /// containing a log value of <tt>|T[p]|</tt> translated in the
      /// interval [1, 100].
      ///
      /// \tparam R Value type of the resulting image.
      ///
      /// \param ordered Kind of traversal.
      template <class R>
      image2d<R> transformed_image_log_magn(bool ordered = true) const;

      /// \brief Accessor to the transformed image (log of the
      /// magnitude).
      ///
      /// For each point \p p of the transformed image \p T, an image
      /// containing a log value of <tt>|T[p]|</tt> translated in the
      /// interval [1, 100].
      ///
      /// \param ordered Kind of traversal.
      image2d<T> transformed_image_log_magn(bool ordered = true) const;

      ~fft();

    protected:
      /// Input image (used internally by FFTW).
      typename fft_trait<T>::fftw_input* in;
      /// Output (complex) image used internally by FFTW).
      std::complex<T>* out;
      /// (Forward) plan (command to be executed by FFTW).
      fftw_plan p;
      /// Inverse (backward) plan (command to be executed by FFTW).
      fftw_plan p_inv;
      /// Transformed image (``exported'' Milena image accessible
      /// through accessors).
      image2d< std::complex<T> > trans_im;
    };

  } // end of namespace mln::internal


  namespace transform
  {

    /// \brief FFT engine, producing a Discrete Fourier Transform
    /// using FFTW (version 3).
    ///
    /// \tparam T Data type.
    ///
    /// \param which Dispatch traits.
    template <class T,
	      internal::fft_dispatch which = internal::fft_trait<T>::which >
    class fft;


    /// \brief oFFT engine (specialization for images of real values).
    ///
    /// \tparam T Data type.
    template <class T>
    class fft<T, internal::fft_real> : public internal::fft<T>
    {
    public:
      /// \brief Constructor.
      ///
      /// Initialization of data for the computation of the FFT.
      ///
      /// \param original_im Image to process.
      template <typename D>
      fft(const image2d<D>& original_im);

      /// \brief Compute and return the transform (as a complex image).
      image2d< std::complex<T> > transform();

      /// \brief Compute and return the inverse transform (as a real
      /// image) of the FFT.
      ///
      /// \tparam R Value type of output image.
      template <class R>
      image2d<R> transform_inv();

      /// \brief Compute and return the inverse transform of the FFT.
      image2d<T> transform_inv();
    };


    /// \brief FFT engine (specialization for images of complex values).
    ///
    /// \tparam T Data type.
    template <class T>
    class fft<T, internal::fft_cplx> : public internal::fft<T>
    {
    public:
      /// \brief Constructor.
      ///
      /// Initialization of data for the computation of the FFT.
      ///
      /// \param original_im Image to process.
      fft(const image2d< std::complex<T> >& original_im);

      /// \brief Compute and return the transform (as a complex image).
      image2d< std::complex<T> > transform();

      /// \brief Compute and return the inverse transform (as a
      /// complex image).
      ///
      /// \tparam R Component type of the value type of the output
      /// image.
      template <class R>
      image2d< std::complex<R> > transform_inv();

      /// \brief Compute and return the inverse transform of the FFT.
      image2d< std::complex<T> > transform_inv();
    };

  } // end of namespace mln::transform



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <class T>
    inline
    const image2d< std::complex<T> >&
    fft<T>::transformed_image() const
    {
      return trans_im;
    }

    template <class T>
    inline
    image2d< std::complex<T> >&
    fft<T>::transformed_image()
    {
      return trans_im;
    }

    template <class T>
    template <class R>
    inline
    image2d<R>
    fft<T>::transformed_image_magn(bool ordered) const
    {
      // FIXME: Ensure R is real.

      const unsigned nrows = trans_im.nrows();
      const unsigned ncols = trans_im.ncols();
      image2d<R> new_im(trans_im.domain());
      if (ordered)
	for (unsigned row = 0; row < new_im.nrows(); ++row)
	  for (unsigned col = 0; col < new_im.ncols(); ++col)
	    opt::at(new_im, row, col) =
	      std::norm(opt::at(trans_im,
				(row + nrows / 2) % nrows,
				(col + ncols / 2) % ncols));
      else
	{
	  mln_piter(image2d< std::complex<T> >) it(trans_im.domain());
	  for_all(it)
	    new_im(it) = std::norm(trans_im(it));
	}
      return new_im;
    }

    template <class T>
    inline
    image2d<T>
    fft<T>::transformed_image_magn(bool ordered) const
    {
      return transformed_image_magn<T>(ordered);
    }

    template <class T>
    template <class R>
    inline
    image2d<R>
    fft<T>::transformed_image_clipped_magn(double clip, bool ordered) const
    {
      // FIXME: Ensure R is real.
      // FIXME: Ensure `clip' is between 0 and 1?

      double max = mln_min(double);
      mln_piter(image2d<T>) it(trans_im.domain());
      for_all(it)
	if (std::norm(trans_im(it)) > max)
	  max = std::norm(trans_im(it));

      const unsigned nrows = trans_im.nrows();
      const unsigned ncols = trans_im.ncols();
      image2d<R> new_im(trans_im.domain());
      if (ordered)
	for (unsigned row = 0; row < new_im.nrows(); ++row)
	  for (unsigned col = 0; col < new_im.ncols(); ++col)
	    if (std::norm(opt::at(trans_im,
				  (row + nrows / 2) % nrows,
				  (col + ncols / 2) % ncols))
		>= max * clip)
	      opt::at(new_im, row, col) = mln_max(R);
	    else
	      opt::at(new_im, row, col) =
		(double) mln_max(R) *
		std::norm(opt::at(trans_im,
				  (row + nrows / 2) % nrows,
				  (col + ncols / 2) % ncols)) / (max * clip);
      else
	for_all(it)
	  if (std::norm(trans_im(it)) >= max * clip)
	    new_im(it) = mln_max(R);
	  else
	    new_im(it) =
	      (double) mln_max(R) * std::norm(trans_im(it)) / (max * clip);
      return new_im;
    }

    template <class T>
    inline
    image2d<T>
    fft<T>::transformed_image_clipped_magn(double clip, bool ordered) const
    {
      return transformed_image_clipped_magn<T>(clip, ordered);
    }

    template <class T>
    template <class R>
    inline
    image2d<R>
    fft<T>::transformed_image_clipped_magn(bool ordered) const
    {
      return transformed_image_clipped_magn<R>(1, ordered);
    }

    template <class T>
    inline
    image2d<T>
    fft<T>::transformed_image_clipped_magn(bool ordered) const
    {
      return transformed_image_clipped_magn<T>(1, ordered);
    }

    template <class T>
    template <class R>
    inline
    image2d<R>
    fft<T>::transformed_image_log_magn(double a, double b, bool ordered) const
    {
      /* FIXME: Find a more elegant way to fix range interval on a
	 and b (Note from Roland: what does it mean?).  */

      // FIXME: Ensure R is real.
      // FIXME: Ensure 0 <= a <= 1000.
      // FIXME: Ensure 0 <= b <= 1000.

      image2d<R> new_im(trans_im.domain());

      double max = mln_min(double);
      mln_piter(image2d<R>) it(trans_im.domain());
      for_all(it)
	if (std::norm(trans_im(it)) > max)
	  max = std::norm(trans_im(it));

      const unsigned nrows = trans_im.nrows();
      const unsigned ncols = trans_im.ncols();
      if (ordered)
	for (unsigned row = 0; row < new_im.nrows(); ++row)
	  for (unsigned col = 0; col < new_im.ncols(); ++col)
	    opt::at(new_im, row, col) =
	      log(a + b * std::norm(opt::at(trans_im,
					    (row + nrows / 2) % nrows,
					    (col + ncols / 2) % ncols)))
	      / log (a + b * max) * mln_max(R);
      else
	{
	  mln_piter(image2d< std::complex<T> >) it(trans_im.domain());
	  for_all(it)
	    new_im(it) =
	    log(a + b * std::norm(trans_im(it)))
	    / log (a + b * max) * mln_max(R);
	}
      return new_im;
    }

    template <class T>
    inline
    image2d<T>
    fft<T>::transformed_image_log_magn(double a, double b, bool ordered) const
    {
      return transformed_image_log_magn<T>(a, b, ordered);
    }

    template <class T>
    template <class R>
    inline
    image2d<R>
    fft<T>::transformed_image_log_magn(bool ordered) const
    {
      return transformed_image_log_magn<R>(1, 100, ordered);
    }

    template <class T>
    inline
    image2d<T>
    fft<T>::transformed_image_log_magn(bool ordered) const
    {
      return transformed_image_log_magn<T>(1, 100, ordered);
    }

    template <class T>
    inline
    fft<T>::~fft()
    {
      fftw_free(in);
      fftw_free(out);
      fftw_destroy_plan(p);
      fftw_destroy_plan(p_inv);
    }

  } // end of namespace mln::internal


  namespace transform
  {

    /*--------------------------------------------------------.
    | FFT engine (specialization for images of real values).  |
    `--------------------------------------------------------*/

    template <class T>
    template <typename D>
    inline
    fft<T, internal::fft_real>::fft(const image2d<D>& original_im)
    {
      const unsigned nrows = original_im.nrows();
      const unsigned ncols = original_im.ncols();

      this->in  = (T*) fftw_malloc(nrows * ncols * sizeof(T));
      this->out = (std::complex<T>*) fftw_malloc(nrows
						 * (ncols / 2 + 1)
						 * sizeof(std::complex<T>));

      for (unsigned row = 0; row < nrows; ++row)
	for (unsigned col = 0; col < ncols; ++col)
	  this->in[row * ncols + col] =
	    opt::at(original_im, row, col);

      this->p =
	fftw_plan_dft_r2c_2d (nrows, ncols,
			      this->in,
			      reinterpret_cast<fftw_complex*>(this->out),
			      FFTW_ESTIMATE);
      this->p_inv =
	fftw_plan_dft_c2r_2d (nrows, ncols,
			      reinterpret_cast<fftw_complex*>(this->out),
			      this->in,
			      FFTW_ESTIMATE);
      this->trans_im = image2d< std::complex<T> >(original_im.domain());
    }

    template <class T>
    inline
    image2d< std::complex<T> >
    fft<T, internal::fft_real>::transform()
    {
      fftw_execute(this->p);

      const unsigned nrows = this->trans_im.nrows();
      const unsigned ncols = this->trans_im.ncols();
      unsigned denom = nrows * ncols;
      int i = 0;
      for (unsigned row = 0; row < nrows; ++row)
	for (unsigned col = 0; col <= ncols / 2; ++col)
	  {
	    this->out[i] = std::complex<T> (this->out[i].real() / denom,
					    this->out[i].imag() / denom);
	    opt::at(this->trans_im, row, col) = this->out[i];
	    ++i;
	  }
      for (unsigned row = 0; row < nrows; ++row)
	for (unsigned col = ncols - 1; col > ncols / 2; --col)
	  opt::at(this->trans_im, row, col) =
	    opt::at(this->trans_im, nrows - row - 1, ncols - col - 1);
      return this->trans_im;
    }

    template <class T>
    template <class R>
    inline
    image2d<R>
    fft<T, internal::fft_real>::transform_inv()
    {
      // FIXME: Ensure R is real.

      const unsigned nrows = this->trans_im.nrows();
      const unsigned ncols = this->trans_im.ncols();
      for (unsigned row = 0; row < nrows; ++row)
	for (unsigned col = 0; col <= ncols / 2; ++col)
	  this->out[row * (ncols / 2 + 1) + col] =
	    opt::at(this->trans_im, row, col);

      fftw_execute(this->p_inv);

      image2d<R> new_im(this->trans_im.domain());
      int i = 0;
      for (unsigned row = 0; row < nrows; ++row)
	for (unsigned col = 0; col < ncols; ++col)
	  {
	    opt::at(new_im, row, col) = (this->in[i] >= mln_min(R)
					 ? (this->in[i] <= mln_max(R)
					    ? (R) this->in [i]
					    : mln_min(R))
					 : mln_max(R));
	    ++i;
	  }
      return new_im;
    }

    template <class T>
    inline
    image2d<T>
    fft<T, internal::fft_real>::transform_inv()
    {
      return transform_inv<T>();
    }


    /*-----------------------------------------------------------.
    | FFT engine (specialization for images of complex values).  |
    `-----------------------------------------------------------*/

    template <class T>
    inline
    fft<T, internal::fft_cplx>::fft(const image2d< std::complex<T> >& original_im)
    {
      const unsigned nrows = original_im.nrows();
      const unsigned ncols = original_im.ncols();

      this->in  = fftw_malloc(sizeof(std::complex<T>) * nrows * ncols);
      this->out = fftw_malloc(sizeof(std::complex<T>) * nrows * ncols);

      for (unsigned row = 0; row < nrows; ++row)
	for (unsigned col = 0; col < ncols; ++col)
	  {
	    this->in[row * ncols + col].re = original_im(row, col).real();
	    this->in[row * ncols + col].im = original_im(row, col).imag();
	  }

      this->p     = fftw_plan_dft_2d(nrows, ncols, this->in, this->out,
				     FFTW_FORWARD, FFTW_ESTIMATE);
      this->p_inv = fftw_plan_dft_2d(nrows, ncols, this->out, this->in,
				     FFTW_BACKWARD, FFTW_ESTIMATE);
      this->trans_im = image2d< std::complex<T> >(original_im.domain());
    }

    template <class T>
    inline
    image2d< std::complex<T> >
    fft<T, internal::fft_cplx>::transform()
    {
      fftw_execute(this->p);

      const unsigned nrows = this->trans_im.nrows();
      const unsigned ncols = this->trans_im.ncols();
      unsigned denom = nrows * ncols;
      int i = 0;
      for (unsigned row = 0; row < nrows; ++row)
	for (unsigned col = 0; col < ncols; ++col)
	  {
	    this->out[i].re /= denom;
	    this->out[i].im /= denom;
	    this->trans_im(row, col) =
	      std::complex<double>(this->out[i].re, this->out[i].im);
	    ++i;
	  }
      return this->trans_im;
    }

    template <class T>
    template <class R>
    inline
    image2d< std::complex<R> >
    fft<T, internal::fft_cplx>::transform_inv()
    {
      const unsigned nrows = this->trans_im.nrows();
      const unsigned ncols = this->trans_im.ncols();
      for (unsigned row = 0; row < nrows; ++row)
	for (unsigned col = 0; col < ncols; ++col)
	  {
	    this->out[row * ncols + col].re =
	      this->trans_im(row, col).real();
	    this->out[row * ncols + col].im =
	      this->trans_im(row, col).imag();
	  }

      fftw_execute(this->p_inv);

      image2d< std::complex<R> > new_im(this->trans_im.size());
      int i = 0;
      for (unsigned row = 0; row < nrows; ++row)
	for (unsigned col = 0; col < ncols; ++col)
	  {
	    new_im(row, col) = this->in[i];
	    ++i;
	  }
      return new_im;
    }

    template <class T>
    inline
    image2d< std::complex<T> >
    fft<T, internal::fft_cplx>::transform_inv()
    {
      return transform_inv<T>();
    }

  } // end of namespace mln::transform

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_TRANSFORM_FFT_HH
