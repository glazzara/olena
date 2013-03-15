// Copyright (C) 2011, 2012, 2013 EPITA Research and Development
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

#ifndef SCRIBO_IO_IMG_SAVE_HH
# define SCRIBO_IO_IMG_SAVE_HH

/// \file
///
/// \brief Save document information as an image.

# include <libgen.h>
# include <fstream>
# include <sstream>

# include <map>

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/io/magick/save.hh>
# include <mln/data/transform.hh>
# include <mln/subsampling/antialiased.hh>

# include <scribo/core/document.hh>

# include <scribo/io/img/internal/text_img_visitor.hh>
# include <scribo/io/img/internal/non_text_img_visitor.hh>
# include <scribo/io/img/internal/full_img_visitor.hh>
# include <scribo/io/img/internal/debug_img_visitor.hh>


namespace scribo
{

  namespace io
  {

    namespace img
    {

      /*! \brief Supported image formats

	Text : Display text regions.

	NonText : Display non-text regions.

	Full : Text and non-text regions.

	DebugWoImage : Display all regions onto a subsampled copy of
	the original image. A dark background is used as background.

	DebugWithImage : Display all regions onto a subsampled copy of
	the original image. The original image is used as background.

       */
      enum Format
      {
	Text,
	NonText,
	Full,
	DebugWoImage,
	DebugWithImage,
      };


      /*! \brief Save document information as an image

      */
      template <typename L>
      void
      save(const document<L>& doc, const std::string& output_name,
	   Format format);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	// FIXME: should be moved as parameter of io::img::save.  This
	// requires to update some functors.
	unsigned reduction_factor = 1;

	struct highlight_mask
	  : Function_v2v<highlight_mask>
	{
	  typedef value::rgb8 result;

	  highlight_mask(float ratio)
	    : ratio(ratio)
	  {
	  }

	  result operator()(const result& v) const
	  {
	    result out = v;
	    out.red() = std::min(out.red() * ratio, 255.f);
	    out.green() = std::min(out.green() * ratio, 255.f);
	    out.blue() = std::min(out.blue() * ratio, 255.f);
	    return out;
	  }

	  float ratio;
	};


	template <typename L>
	mln::image2d<value::rgb8>
	save_text(const document<L>& doc)
	{
	  mln_precondition(doc.is_valid());
	  mln::image2d<value::rgb8> output = duplicate(doc.image());
	  scribo::io::img::internal::text_img_visitor f(output);
	  doc.accept(f);
	  return output;
	}

	template <typename L>
	mln::image2d<value::rgb8>
	save_non_text(const document<L>& doc)
	{
	  mln_precondition(doc.is_valid());
	  mln::image2d<value::rgb8> output = duplicate(doc.image());
	  scribo::io::img::internal::non_text_img_visitor f(output);
	  doc.accept(f);
	  return output;
	}

	template <typename L>
	mln::image2d<value::rgb8>
	save_full(const document<L>& doc)
	{
	  mln_precondition(doc.is_valid());
	  mln::image2d<value::rgb8> output = duplicate(doc.image());
	  scribo::io::img::internal::full_img_visitor<L> f(output);
	  doc.accept(f);
	  return output;
	}

	template <typename L>
	mln::image2d<value::rgb8>
	save_debug_without_image(const document<L>& doc)
	{
	  mln_precondition(doc.is_valid());

	  const box2d& ima_domain = doc.image().domain();
	  box2d domain(ima_domain.pmin() / reduction_factor,
		       ima_domain.pmax() / reduction_factor);
	  mln::image2d<value::rgb8> output(domain);
	  data::fill(output, literal::black);

	  scribo::io::img::internal::debug_img_visitor<L> f(output,
							    reduction_factor);
	  doc.accept(f);
	  return output;
	}

	template <typename L>
	mln::image2d<value::rgb8>
	save_debug_with_image(const document<L>& doc)
	{
	  mln_precondition(doc.is_valid());
	  internal::highlight_mask highlight(0.5f);
	  mln::image2d<value::rgb8>
	    output = data::transform(doc.image(), highlight);
	  if (reduction_factor > 1)
	    output = mln::subsampling::antialiased(output, reduction_factor);

	  scribo::io::img::internal::debug_img_visitor<L> f(output,
							    reduction_factor);
	  doc.accept(f);
	  return output;
	}

      } // end of namespace scribo::io::img::internal



      // FACADE

      template <typename L>
      void
      save(const document<L>& doc,
	   const std::string& output_name,
	   Format format)
      {
	mln_trace("scribo::io::img::save");

	mln_precondition(doc.is_open());

	mln::image2d<value::rgb8> output;

	// Choose saving method.
	switch (format)
	{
	  case Text:
	    output = internal::save_text(doc);
	    break;

	  case NonText:
	    output = internal::save_non_text(doc);
	    break;

	  case Full:
	    output = internal::save_full(doc);
	    break;

	  case DebugWoImage:
	    output = internal::save_debug_without_image(doc);
	    break;

	  case DebugWithImage:
	    output = internal::save_debug_with_image(doc);
	    break;

	  default:
	    trace::warning("scribo::io::img::save - "
			   "Invalid image format! Skip saving...");
	    return;
	}

	mln::io::magick::save(output, output_name.c_str());

      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::io::img

  } // end of namespace scribo::io

} // end of namespace scribo


#endif // ! SCRIBO_IO_IMG_SAVE_HH
