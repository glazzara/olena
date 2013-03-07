// Copyright (C) 2010, 2011, 2012 EPITA Research and Development
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

#ifndef SCRIBO_TOOLCHAIN_INTERNAL_TEXT_IN_DOC_PREPROCESS_FUNCTOR_HH
# define SCRIBO_TOOLCHAIN_INTERNAL_TEXT_IN_DOC_PREPROCESS_FUNCTOR_HH

#include <mln/core/concept/image.hh>
#include <mln/data/transform.hh>
#include <mln/data/convert.hh>
#include <mln/fun/v2v/rgb_to_luma.hh>

#include <mln/subsampling/antialiased.hh>
#include <mln/util/timer.hh>

#include <scribo/binarization/sauvola.hh>
#include <scribo/binarization/sauvola_ms.hh>

#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/preprocessing/deskew.hh>
#include <scribo/preprocessing/denoise.hh>

#include <scribo/toolchain/internal/toolchain_functor.hh>

namespace scribo
{

  namespace toolchain
  {

    namespace internal
    {

      using namespace mln;

      /*! \brief Enum of the binarization algorithms available.

	Convert: Violently convert from color to binary.
	Sauvola: Local threshold binarization algoritm.
	SauvolaMs: Multi-scale local threshold binarization algoritm.
       */
      enum Binarization_Algo
      {
	Convert,
	Sauvola,
	SauvolaMs
      };


      /*! \brief Functor performing custom preprocessing algorithms on
       *  documents.

       Whatever the options selected, this functor converts an input
       image into a graylevel image and try to binarize the latter.

       Optional algorithms can be performed too:
       - Subsampling (enable_subsample), disabled by default.
       - Extract Background/Foreground (enable_fg_extraction), split
         background and foreground objects, disabled by default.
       - Deskew (enable_deskew), disabled by default.
       - Denoise (enable_denoise), enabled by default.

       The binarization algorithm can be chosen through
       binarization_algo option. It can be one of the algorithms
       detailed in enum Binarization_Algo.

       Few parameters can be set for algorithms:

       - Sauvola Multi-scale:
         * sauvola_win, the window size used in Sauvola based
           algorithms (default 101).
	 * sauvola_K2, a user parameter for Sauvola's threshold formula
	   at scale 2.
	   (default 0.34).
	 * sauvola_K3, a user parameter for Sauvola's threshold formula
	   at scale 3.
	   (default 0.34).
	 * sauvola_K4, a user parameter for Sauvola's threshold formula
	   at scale 4.
	   (default 0.34).

       - Extract background/foreground
         * lambda, the maximum area of the possible foreground objects
           (default 0, auto).

       */
      template <typename I>
      struct text_in_doc_preprocess_functor
	: public Toolchain_Functor
      {

	text_in_doc_preprocess_functor();

	virtual int nsteps() const;

	//===============
	// Core function
	//===============

	mln_ch_value(I,bool) operator()(const Image<I>& input_);


	//=========
	// Options
	//=========

	// Settings
	bool enable_subsample;
	bool enable_fg_extraction;
	bool enable_deskew;
	bool enable_denoising;

	Binarization_Algo binarization_algo;

	unsigned sauvola_win;
	double sauvola_k2;
	double sauvola_k3;
	double sauvola_k4;
	unsigned lambda;

	// Results
	mln_concrete(I) fg;
	mln_concrete(I) bg;
	image2d<bool> output;

#  ifndef SCRIBO_NDEBUG
	//=============
	// DEBUG TOOLS
	//=============
	virtual void on_start();
	virtual void on_end();
	virtual void on_progress();

	mln::util::timer t;
	mln::util::timer gt;
#  endif // ! SCRIBO_NDEBUG

      private: // Methods
	unsigned find_best_scale(const Image<I>& ima_);

      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      text_in_doc_preprocess_functor<I>::text_in_doc_preprocess_functor()
	: enable_subsample(false),
	  enable_fg_extraction(false),
	  enable_deskew(false),
	  enable_denoising(false),
	  binarization_algo(SauvolaMs),
	  sauvola_win(101),
	  sauvola_k2(0.34),
	  sauvola_k3(0.34),
	  sauvola_k4(0.34),
	  lambda(0)
      {
      }


      //===============
      // Core function
      //===============

      template <typename I>
      mln_ch_value(I,bool)
      text_in_doc_preprocess_functor<I>::operator()(const Image<I>& input_)
      {
	trace::entering("scribo::toolchain::text_in_doc_preprocess");

	const I& input = exact(input_);
	mln_precondition(input.is_valid());

	mln_concrete(I) input_rgb = input;

	on_start();

	// Subsample
	//----------
	if (enable_subsample)
	{
	  on_new_progress_label("Subsample");

	  input_rgb = mln::subsampling::antialiased(input_rgb,
						    find_best_scale(input_rgb));

	  on_progress();
	}


	// Remove background
	//------------------
	if (enable_fg_extraction)
	{
	  on_new_progress_label("Foreground Extraction");

	  typedef mln::util::couple<mln_concrete(I), mln_concrete(I)> res_t;

	  unsigned rlambda = lambda;
	  if (!lambda)
	    rlambda = 1.2 * (input.nrows() + input.ncols());

	  res_t res = scribo::preprocessing::split_bg_fg(input_rgb, rlambda, 32);

	  bg = res.first();
	  fg = res.second();
	  input_rgb = res.second();

	  on_progress();
	}


	// Convert to grayscale image (always happens).
	//---------------------------------------------
	on_new_progress_label("Convert to gray-scale image");
	image2d<value::int_u8>
	  intensity_ima = mln::data::transform(input_rgb,
					       mln::fun::v2v::rgb_to_luma<value::int_u8>());
	on_progress();


	// Deskew
	//-------
	if (enable_deskew)
	{
	  on_new_progress_label("Deskew");

	  intensity_ima = scribo::preprocessing::deskew(intensity_ima);

	  on_progress();
	}


	// Binarization (always happens)
	//------------------------------
	if (binarization_algo == Sauvola)
	{
	  on_new_progress_label("Binarization (Sauvola)");
	  output = scribo::binarization::sauvola(intensity_ima);
	}
	else if (binarization_algo == SauvolaMs)
	{
	  on_new_progress_label("Binarization (Sauvola Multi-scale)");
	  output = scribo::binarization::sauvola_ms(intensity_ima,
						    sauvola_win, 3,
						    sauvola_k2,
						    sauvola_k3,
						    sauvola_k4);
	}
	else // binarization_algo == Convert
	{
	  on_new_progress_label("Binarization (Binary conversion)");
	  output = mln::data::convert(bool(), intensity_ima);
	}

	on_progress();


	// Denoise
	//--------
	if (enable_denoising)
	{
	  on_new_progress_label("Remove noise");

	  output = scribo::preprocessing::denoise(output, c8(), 2, 2);

	  on_progress();
	}

	on_end();

	return output;
      }



      template<typename I>
      int
      text_in_doc_preprocess_functor<I>::nsteps() const
      {
	return 2 + enable_denoising + enable_deskew
	  + enable_fg_extraction + enable_subsample;
      }


      template <typename I>
      unsigned
      text_in_doc_preprocess_functor<I>::find_best_scale(const Image<I>& ima_)
      {
	const I& ima = exact(ima_);
	if (ima.nrows() > 2500
	    && ima.nrows() < 5000
	    && ima.ncols() > 2500
	    && ima.ncols() < 5000)
	  return 2;

	if (ima.nrows() > 5000
	    && ima.ncols() > 5000)
	  return 3;

	return 1;
      }


#  ifndef SCRIBO_NDEBUG

      template <typename I>
      void
      text_in_doc_preprocess_functor<I>::on_start()
      {
	gt.start();
	t.start();
      }

      template <typename I>
      void
      text_in_doc_preprocess_functor<I>::on_end()
      {
	gt.stop();
	if (verbose)
	  std::cout << "Total time: " << gt << std::endl;
      }

      template <typename I>
      void
      text_in_doc_preprocess_functor<I>::on_progress()
      {
	t.stop();
	if (verbose)
	  std::cout << t << std::endl;
	t.restart();
      }


#  endif // ! SCRIBO_NDEBUG


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::toolchain::internal

  } // end of namespace scribo::toolchain

} // end of namespace scribo

#endif // ! SCRIBO_TOOLCHAIN_INTERNAL_TEXT_IN_DOC_PREPROCESS_FUNCTOR_HH
