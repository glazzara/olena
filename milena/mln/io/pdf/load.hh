// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_PDF_LOAD_HH
# define MLN_IO_PDF_LOAD_HH

/// \file
///
/// \brief Load a multiple page PDF document using libpoppler.

#include <iostream>
#include <mln/core/image/image2d.hh>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page-renderer.h>


namespace mln
{

  namespace io
  {

    namespace pdf
    {

      /**! \brief Load a multiple page PDF document using libpoppler.

	 \param[out] arr       An array of images.
	 \param[in]  filename  The name of the input file.
	 \param[in]  dpi       Document resolution.

	 Page numbering starts from 0.

	 \ingroup iopdf
      */
      template <typename I>
      void load(util::array<I>& arr, const std::string& filename,
		float dpi);

      /**! \overload

	 \brief Load a range of pages from a PDF document using
	 libpoppler.

	 \param[out] arr       An array of images.
	 \param[in]  filename  The name of the input file.
	 \param[in]  first_page First page number.
	 \param[in]  last_page Last page number.
	 \param[in]  dpi       Document resolution.

	 Page numbering starts from 0.

	 \ingroup iopdf
      */
      template <typename I>
      void load(util::array<I>& arr, const std::string& filename,
		int first_page, int last_page, float dpi);


      /**! \overload

	 \brief Load specific pages from a PDF document using
	 libpoppler.

	 \param[out] arr       An array of images.
	 \param[in]  filename  The name of the input file.
	 \param[in]  pages     An array of pages numbers.
	 \param[in]  dpi       Document resolution.

	 Page numbering starts from 0.

	 \ingroup iopdf
      */
      template <typename I>
      void load(util::array<I>& arr, const std::string& filename,
		util::array<int> pages, float dpi);

      /**! \overload

	 \brief Load a specific page from a PDF document using
	 libpoppler.

	 \param[out] ima       An image.
	 \param[in]  filename  The name of the input file.
	 \param[in]  page      The page number.
	 \param[in]  dpi       Document resolution.

	 Page numbering starts from 0.

	 \ingroup iopdf
      */
      template <typename I>
      void load(Image<I>& ima, const std::string& filename,
		int page, float dpi);


      /**! \overload

	 \brief Load a multiple page PDF document using libpoppler.

	 \param[out] arr       An array of images.
	 \param[in]  filename  The name of the input file.

	 DPI resolution is set to 300.
	 Page numbering starts from 0.

	 \ingroup iopdf
      */
      template <typename I>
      void load(util::array<I>& arr, const std::string& filename);

      /**! \overload

	 \brief Load specific pages from a PDF document using
         libpoppler.

	 \param[out] arr       An array of images.
	 \param[in]  filename  The name of the input file.
	 \param[in]  pages     An array of pages numbers.

	 DPI resolution is set to 300.
	 Page numbering starts from 0.

	 \ingroup iopdf
      */
      template <typename I>
      void load(util::array<I>& arr, const std::string& filename,
		util::array<int> pages);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename I>
	void copy_data_argb32(I& ima, const poppler::image& pima)
	{
	  mln_value(I) *vima = &ima(ima.domain().pmin());
	  const unsigned char *
	    nsites = (unsigned char *)pima.const_data()
	    + pima.width() * pima.height() * 4;

	  unsigned next = 4;
	  for (const unsigned char *v =  (unsigned char *)pima.const_data();
	       v < nsites; v += next, ++vima)
	  {
	    vima->red() = *(v + 2);
	    vima->green() = *(v + 1);
	    vima->blue() =  *v;
	  }
	}

	template <typename I>
	void copy_data_rgb24(I& ima, const poppler::image& pima)
	{
	  mln_value(I) *vima = &ima(ima.domain().pmin());
	  const unsigned char *
	    nsites = (unsigned char *)pima.const_data()
	    + pima.width() * pima.height() * 3;

	  unsigned next = 3;
	  for (const unsigned char *v =  (unsigned char *)pima.const_data();
	       v < nsites; v += next, ++vima)
	  {
	    vima->red() = *v;
	    vima->green() = *(v + 1);
	    vima->blue() = *(v + 2);
	  }
	}


	template <typename I>
	void copy_data(I& ima, const poppler::image& pima)
	{
	  mln_precondition(ima.is_valid());

	  switch(pima.format())
	  {
	    case poppler::image::format_invalid:
	    case poppler::image::format_mono:
	      std::cout << "Loading black and white pdf is not implemented!";
	      abort();
	      break;
	    case poppler::image::format_rgb24:
	      copy_data_rgb24(ima, pima);
	      break;
	    case poppler::image::format_argb32:
	      copy_data_argb32(ima, pima);
	      break;
	  }
	}


	template <typename I>
	void extract_page(const poppler::document* pdf, int i,
			  util::array<I>& arr, float dpi)
	{
	  // Read page
	  poppler::page* p = pdf->create_page(i);

	  // Render Page
	  poppler::page_renderer pr;
	  if (! pr.can_render())
	  {
	    std::cout << "PDF cannot be rendered! Make sure libpoppler is "
		      << "compiled with a render backend." << std::endl;
	    abort();
	  }
	  poppler::image pima = pr.render_page(p, dpi, dpi);

	  // Prepare MLN image.
	  I ima(make::box2d(pima.height(), pima.width()), 0);
	  mln_assertion(ima.is_valid());

	  // Copy data
	  internal::copy_data(ima, pima);

	  // Append result
	  arr.append(ima);
	}

      } // end of namespace mln::io::pdf::internal


      template <typename I>
      void load(util::array<I>& arr, const std::string& filename, float dpi)
      {
	mln_trace("mln::io::pdf::load");

	// Load document
	poppler::document *pdf = poppler::document::load_from_file(filename);
	if (pdf == 0)
	{
	  std::cerr << "Error: Cannot load pdf " << filename << std::endl;
	  abort();
	}

	for (int i = 0; i < pdf->pages(); ++i)
	  internal::extract_page(pdf, i, arr, dpi);

	delete pdf;

      }

      template <typename I>
      void load(util::array<I>& arr, const std::string& filename,
		int first_page, int last_page, float dpi)
      {
	mln_trace("mln::io::pdf::load");

	// Load document
	poppler::document *pdf = poppler::document::load_from_file(filename);
	if (pdf == 0)
	{
	  std::cerr << "Error: Cannot load PDF " << filename << std::endl;
	  abort();
	}

	if (first_page < 0 || first_page > (pdf->pages() - 1)
	    || last_page < 0 || last_page > (pdf->pages() - 1))
	{
	  std::cout << "Error while loading PDF: page range is not correct!"
		    << std::endl;
	  abort();
	}

	for (int i = first_page; i <= last_page; ++i)
	  internal::extract_page(pdf, i, arr, dpi);

	delete pdf;

      }


      template <typename I>
      void load(util::array<I>& arr, const std::string& filename,
		util::array<int> pages, float dpi)
      {
	mln_trace("mln::io::pdf::load");

	// Load document
	poppler::document *pdf = poppler::document::load_from_file(filename);
	if (pdf == 0)
	{
	  std::cerr << "Error: Cannot load PDF " << filename << std::endl;
	  abort();
	}

	for (unsigned i = 0; i < pages.size(); ++i)
	{
	  if (pages[i] < 0 || pages[i] > pdf->pages() - 1)
	  {
	    std::cout << "Error while loading PDF: selected page "
		      << pages[i] << " does not exist!" << std::endl;
	    abort();
	  }

	  internal::extract_page(pdf, pages[i], arr, dpi);
	}

	delete pdf;

      }


      template <typename I>
      void load(Image<I>& ima, const std::string& filename,
		int page, float dpi)
      {
	util::array<I> arr;
	load(arr, filename, page, page, dpi);
	mln_assertion(arr.size() == 1);
	exact(ima) = arr[0];
      }

      template <typename I>
      void load(util::array<I>& arr, const std::string& filename)
      {
	load(arr, filename, 300);
      }


      template <typename I>
      void load(util::array<I>& arr, const std::string& filename,
		util::array<int> pages)
      {
	load(arr, filename, pages, 300);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::pdf

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PDF_LOAD_HH
