/* GIMP Plug-in Template
 * Copyright (C) 2000-2004  Michael Natterer <mitch@gimp.org> (the "Author").
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the Author of the
 * Software shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the Author.
 */

#include "build-image.hh"

typedef mln::gimp_image<GIMP_RGB_IMAGE> I;

static I ima;

const I& get_ima()
{
  return ima;
}

struct toto : mln::Function_v2v<toto>
{
  typedef mln::value::rgb8 result;
  mln::value::rgb8 operator()(const mln::value::rgb8& c) const
  {
    return mln::value::rgb8(255 - c.red(), 255 - c.green(), 255 - c.blue());
  }
};

void build_milena_image(GimpPixelRgn *region)
{
  I tmp(region);
//   g_message(region->dirty ? "saaale" : "pas saaale");
//   g_message(region->shadow ? "shadoooow" : "pas shadoooow");
  ima = tmp;
  mln::point2d p1(0,0);
  mln::point2d p2(0,5);
  mln::point2d p3(20,50);
//   mln::io::ppm::save(ima, "/goinfre/out.ppm");
  std::cout << p1 << " : " << ima(p1) << std::endl;
  std::cout << p2 << " : " << ima(p2) << std::endl;
  std::cout << p3 << " : " << ima(p3) << std::endl;
//  mln::level::fill(ima, mln::literal::blue);

  toto fun;
  mln::image2d<mln::value::rgb8> tmp2(ima.domain());
  mln::level::transform(ima, fun, tmp2);
  mln::border::resize(tmp2, 0);

  gimp_pixel_rgn_set_rect(region,
			  (const guchar *) tmp2.buffer(),
			  0,
			  0,
			  region->w,
			  region->h);
   gimp_displays_flush ();
}

gboolean draw_milena_image(GtkWidget* area,
			   GdkEventExpose *event,
			   gpointer data)
{
  GimpDrawable* drawable = gimp_drawable_preview_get_drawable((GimpDrawablePreview *)area);
  assert(drawable != NULL);
  GimpPixelRgn region;
  std::stringstream ss;
  gimp_pixel_rgn_init(&region,
		      drawable,
		      0,
		      0,
		      drawable->width,
		      drawable->height,
		      FALSE,
		      FALSE);

//   gimp_pixel_rgn_set_rect(&region,
// 			  (const guchar *) ima.buffer(),
// 			  0,
// 			  0,
// 			  drawable->width,
// 			  drawable->height);
}
