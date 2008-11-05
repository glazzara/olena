// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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


#include <mln/core/image/image2d.hh>

#include <mln/win/rectangle2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

# include <mln/core/alias/window2d.hh>

#include <mln/level/transform.hh>
#include <mln/binarization/threshold.hh>
#include <mln/estim/mean.hh>
#include <mln/morpho/hit_or_miss.hh>
#include <mln/morpho/dilation.hh>
#include <mln/morpho/opening.hh>

#include <mln/win/hline2d.hh>
#include <mln/win/vline2d.hh>

int main(int argc, const char * argv[])
{
  using namespace mln;
  using namespace value;

  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.pgm [other_files.pgm]" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; ++i)
    {
      typedef int_u8 int_t;

      border::thickness = 21;

      image2d<int_t> ima;
      io::pgm::load(ima, argv[i]);

      // Compute the mean
      int_t mean = estim::mean(ima);


      image2d<bool> imab = binarization::threshold(ima, 10);



      window2d winout;
      window2d winin;

//       static const bool matout [] = {0, 0, 0, 0, 0, 0, 0,
// 				     0, 0, 1, 0, 0, 0, 0,
// 				     0, 0, 1, 0, 0, 0, 0,
// 				     0, 0, 1, 0, 0, 0, 0,
// 				     0, 0, 1, 1, 1, 1, 0,
// 				     0, 0, 0, 0, 0, 0, 0,
// 				     0, 0, 0, 0, 0, 0, 0};

      static const bool blmatout [] = {0, 0, 0, 0, 0,
				     0, 1, 0, 0, 0,
				     0, 1, 0, 0, 0,
				     0, 1, 1, 1, 0,
				     0, 0, 0, 0, 0};

//       static const bool blmatout [] = {0, 0, 0,
//    				       1, 0, 0,
// 				       1, 1, 0};


      convert::from_to(blmatout, winout);

//       static const bool matin [] = {0, 0, 0, 1, 0, 0, 0,
// 				    0, 0, 0, 1, 0, 0, 0,
// 				    0, 0, 0, 1, 0, 0, 0,
// 				    0, 0, 0, 1, 1, 1, 1,
// 				    0, 0, 0, 0, 0, 0, 0,
// 				    0, 0, 0, 0, 0, 0, 0,
// 				    0, 0, 0, 0, 0, 0, 0};

      static const bool blmatin [] = {0, 0, 1, 0, 0,
				    0, 0, 1, 0, 0,
				    0, 0, 1, 1, 1,
				    0, 0, 0, 0, 0,
				    0, 0, 0, 0, 0};

//       static const bool blmatin [] = {0, 1, 0,
// 				      0, 1, 1,
// 				      0, 0, 0};

      convert::from_to(blmatin, winin);
      image2d<bool> bottom_left = morpho::hit_or_miss(imab, winout, winin);


//       static const bool brmatout [] = {0, 0, 0,
// 				       0, 0, 1,
// 				       0, 1, 1};

//       static const bool brmatin [] = {0, 1, 0,
// 				      1, 1, 0,
// 				      0, 0, 0};

      static const bool brmatout [] = {0, 0, 0, 0, 0,
				       0, 0, 0, 1, 0,
				       0, 0, 0, 1, 0,
				       0, 1, 1, 1, 0,
				       0, 0, 0, 0, 0};

      static const bool brmatin [] = {0, 0, 1, 0, 0,
				      0, 0, 1, 0, 0,
				      1, 1, 1, 0, 0,
				      0, 0, 0, 0, 0,
				      0, 0, 0, 0, 0};


      convert::from_to(brmatout, winout);
      convert::from_to(brmatin, winin);
      image2d<bool> bottom_right = morpho::hit_or_miss(imab, winout, winin);

//       static const bool urmatout [] = {0, 1, 1,
// 				       0, 0, 1,
// 				       0, 0, 0};

//       static const bool urmatin [] = {0, 0, 0,
// 				      1, 1, 0,
// 				      0, 1, 0};

      static const bool urmatout [] = {0, 0, 0, 0, 0,
				       0, 1, 1, 1, 0,
				       0, 0, 0, 1, 0,
				       0, 0, 0, 1, 0,
				       0, 0, 0, 0, 0};

      static const bool urmatin [] = {0, 0, 0, 0, 0,
				      0, 0, 0, 0, 0,
				      1, 1, 1, 0, 0,
				      0, 0, 1, 0, 0,
				      0, 0, 1, 0, 0};


      convert::from_to(urmatout, winout);
      convert::from_to(urmatin, winin);
      image2d<bool> up_right = morpho::hit_or_miss(imab, winout, winin);


//       static const bool ulmatout [] = {1, 1, 0,
// 				       1, 0, 0,
// 				       0, 0, 0};

//       static const bool ulmatin [] = {0, 0, 0,
// 				      0, 1, 1,
// 				      0, 1, 0};

      static const bool ulmatout [] = {0, 0, 0, 0, 0,
				       0, 1, 1, 1, 0,
				       0, 1, 0, 0, 0,
				       0, 1, 0, 0, 0,
				       0, 0, 0, 0, 0};

      static const bool ulmatin [] = {0, 0, 0, 0, 0,
				      0, 0, 0, 0, 0,
				      0, 0, 1, 1, 1,
				      0, 0, 1, 0, 0,
				      0, 0, 1, 0, 0};


      convert::from_to(ulmatout, winout);
      convert::from_to(ulmatin, winin);
      image2d<bool> up_left = morpho::hit_or_miss(imab, winout, winin);


      static const bool umatout [] = {0, 0, 0, 0, 0,
				      1, 1, 1, 1, 1,
				      0, 0, 0, 0, 0,
				      0, 0, 0, 0, 0,
				      0, 0, 0, 0, 0};

      static const bool umatin [] = {0, 0, 0, 0, 0,
				     0, 0, 0, 0, 0,
				     0, 1, 1, 1, 0,
				     0, 0, 0, 0, 0,
				     0, 0, 0, 0, 0};


      convert::from_to(umatout, winout);
      convert::from_to(umatin, winin);
      image2d<bool> up = morpho::hit_or_miss(imab, winout, winin);


      static const bool bmatout [] = {0, 0, 0, 0, 0,
				      0, 0, 0, 0, 0,
				      0, 0, 0, 0, 0,
				      1, 1, 1, 1, 1,
				      0, 0, 0, 0, 0};

      static const bool bmatin [] = {0, 0, 0, 0, 0,
				     0, 0, 0, 0, 0,
				     0, 1, 1, 1, 0,
				     0, 0, 0, 0, 0,
				     0, 0, 0, 0, 0};


      convert::from_to(bmatout, winout);
      convert::from_to(bmatin, winin);
      image2d<bool> bottom = morpho::hit_or_miss(imab, winout, winin);


      static const bool lmatout [] = {0, 1, 0, 0, 0,
				      0, 1, 0, 0, 0,
				      0, 1, 0, 0, 0,
				      0, 1, 0, 0, 0,
				      0, 1, 0, 0, 0};

      static const bool lmatin [] = {0, 0, 0, 0, 0,
				     0, 0, 1, 0, 0,
				     0, 0, 1, 0, 0,
				     0, 0, 1, 0, 0,
				     0, 0, 0, 0, 0};


      convert::from_to(lmatout, winout);
      convert::from_to(lmatin, winin);
      image2d<bool> left = morpho::hit_or_miss(imab, winout, winin);


      static const bool rmatout [] = {0, 0, 0, 1, 0,
				      0, 0, 0, 1, 0,
				      0, 0, 0, 1, 0,
				      0, 0, 0, 1, 0,
				      0, 0, 0, 1, 0};

      static const bool rmatin [] = {0, 0, 0, 0, 0,
				     0, 0, 1, 0, 0,
				     0, 0, 1, 0, 0,
				     0, 0, 1, 0, 0,
				     0, 0, 0, 0, 0};


      convert::from_to(rmatout, winout);
      convert::from_to(rmatin, winin);
      image2d<bool> right = morpho::hit_or_miss(imab, winout, winin);




      std::string name(argv[i]);
      name.erase(name.length() - 4);
      io::pbm::save( imab, name.append("_bin.pbm"));

      name = argv[i];
      name.erase(name.length() - 4);
      //      image2d<bool> final(right.domain());
      //      level::fill(final, false);
      //     mln_piter_(image2d<bool>) p(final.domain());
      //     for_all(p)
      //	final(p) = up_left(p) || up_right(p) || bottom_right(p) || bottom_left(p) || up(p) || bottom(p) || left(p) || right(p);

      image2d<bool> inter (up_left + up_right + bottom_right + bottom_left + up + bottom + left + right);

      static const bool vert [] = {0, 0, 0, 1, 0, 0, 0,
				   0, 0, 0, 1, 0, 0, 0,
				   0, 0, 0, 1, 0, 0, 0,
				   0, 0, 0, 1, 0, 0, 0,
				   0, 0, 0, 1, 0, 0, 0,
				   0, 0, 0, 1, 0, 0, 0,
				   0, 0, 0, 1, 0, 0, 0};

      static const bool hori [] = {0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0,
				   1, 1, 1, 1, 1, 1, 1,
				   0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0};

      convert::from_to(vert, winin);
      convert::from_to(hori, winout);

      image2d<bool> final = morpho::opening(inter, win::hline2d(20));
      final += morpho::opening(inter, win::vline2d(20));

      io::pbm::save( final,
		     name.append("_hom.pbm"));
    }
}
