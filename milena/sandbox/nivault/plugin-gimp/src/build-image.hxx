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

#include <sstream>
#include "build-image.hh"

typedef mln::image2d<mln::value::rgb8> I; 

static I ima;

const I& get_ima()
{
  return ima;
}

void build_milena_image(GimpDrawable *drawable)
{
  I tmp(mln::make::box2d(0, 0, drawable->height - 1, drawable->width - 1), 0);
  ima = tmp;
  mln_piter_(I) p(ima.domain());
  GimpRGB color;
  GimpPixelRgn region;

  gimp_pixel_rgn_init(&region,
		      drawable,
		      0,
		      0,
		      drawable->width,
		      drawable->height,
		      FALSE,
		      FALSE);
  
  gimp_pixel_rgn_get_rect(&region,
			  (guchar *) ima.buffer(),
			  0,
			  0,
			  drawable->width,
			  drawable->height);
}

gboolean draw_milena_image(GtkWidget* area,
			   GdkEventExpose *event,
			   gpointer data)
{
//   GimpDrawable* drawable = (GimpDrawable *)area->window;
//   GimpPixelRgn region;
//   std::stringstream ss;
//   gimp_pixel_rgn_init(&region,
// 		      drawable,
// 		      0,
// 		      0,
// 		      drawable->width,
// 		      drawable->height,
// 		      FALSE,
// 		      FALSE);  
//   ss << region.bpp << std::endl;
//   g_message(ss.str().c_str());


//   gimp_pixel_rgn_set_rect(&region,
// 			  (const guchar *) ima.buffer(),
// 			  0,
// 			  0,
// 			  drawable->width,
// 			  drawable->height / 4);


  mln_piter_(I) p(ima.domain());
  assert(area->window);
  GdkGC *cc = gdk_gc_new(area->window);
  GdkColor col;

  for_all(p)
  {
    mln::value::rgb8 rgb = ima(p);
    col.red = 256 * rgb.red();
    col.green = 256 * rgb.green();
    col.blue = 256 * rgb.blue();
    gdk_gc_set_rgb_fg_color(cc, &col);

    gdk_draw_point(area->window,
		   cc,
		   p[1], p[0]);
  }
  g_object_unref (cc);
}
