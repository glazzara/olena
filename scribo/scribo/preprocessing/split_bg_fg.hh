// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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


#ifndef SCRIBO_PREPROCESSING_SPLIT_BG_FG_HH
# define SCRIBO_PREPROCESSING_SPLIT_BG_FG_HH

/// \file
///
/// \brief Split the background and the foreground.

# include <mln/core/macros.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/pw/all.hh>

# include <mln/data/fill.hh>
# include <mln/data/transform.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>

# include <mln/io/ppm/load.hh>
# include <mln/io/ppm/save.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/save.hh>

# include <mln/math/diff_abs.hh>
# include <mln/math/min.hh>

# include <mln/morpho/closing/area.hh>
# include <mln/morpho/opening/area.hh>
# include <mln/morpho/elementary/dilation.hh>

# include <mln/labeling/blobs.hh>
# include <mln/labeling/colorize.hh>
# include <mln/labeling/compute.hh>
# include <mln/accu/stat/mean.hh>

# include <mln/util/couple.hh>




namespace scribo
{

  namespace preprocessing
  {

    using namespace mln;


    /*! \brief Split the background and the foreground.

      \param[in] input  A color image.
      \param[in] lambda Lambda used for morphological closing/opening.
      \param[in] delta Max distance between values in closing and
                 opening image.

      \return A couple of color images. The first is the background
              and the second is the foreground.
     */
    template <typename I>
    mln::util::couple<mln_concrete(I), mln_concrete(I)>
    split_bg_fg(const Image<I>& input, unsigned lambda, unsigned delta);



# ifndef MLN_INCLUDE_ONLY



    // Internal routines.

    namespace internal
    {

      using value::rgb8;


      template <unsigned n>
      void
      split(const image2d< value::rgb<n> >& input,
	    // out:
	    image2d< value::int_u<n> >& r,
	    image2d< value::int_u<n> >& g,
	    image2d< value::int_u<n> >& b)
      {
	trace::entering("scribo::preprocessing::internal::split");
	initialize(r, input);
	initialize(g, input);
	initialize(b, input);

	// Generic version:

	//       mln_piter(box2d) p(input.domain());
	//       for_all(p)
	// 	{
	// 	  r(p) = input(p).red();
	// 	  g(p) = input(p).green();
	// 	  b(p) = input(p).blue();
	// 	}

	typedef image2d< value::rgb<n> >   I;
	typedef image2d< value::int_u<n> > O;

	mln_pixter(const I) p_i(input);
	mln_pixter(O) p_r(r);
	mln_pixter(O) p_g(g);
	mln_pixter(O) p_b(b);
	for (p_i.start(), p_r.start(), p_g.start(), p_b.start();
	     p_i.is_valid();
	     p_i.next(), p_r.next(), p_g.next(), p_b.next())
	{
	  const value::rgb<n>& c = p_i.val();
	  p_r.val() = c.red();
	  p_g.val() = c.green();
	  p_b.val() = c.blue();
	}

	trace::exiting("scribo::preprocessing::internal::split");
      }


      template <unsigned n>
      image2d< value::rgb<n> >
      merge(const image2d< value::int_u<n> >& r,
	    const image2d< value::int_u<n> >& g,
	    const image2d< value::int_u<n> >& b)
      {
	trace::entering("scribo::preprocessing::internal::merge");
	image2d< value::rgb<n> > output(r.domain());

	//       mln_piter(box2d) p(output.domain());
	//       for_all(p)
	// 	{
	// 	  value::rgb<n>& c = output(p);
	// 	  c.red()   = r(p);
	// 	  c.green() = g(p);
	// 	  c.blue()  = b(p);
	// 	}

	typedef image2d< value::int_u<n> > I;
	mln_pixter(const I) p_r(r);
	mln_pixter(const I) p_g(g);
	mln_pixter(const I) p_b(b);
	typedef image2d< value::rgb<n> >   O;
	mln_pixter(O) p_o(output);

	for (p_o.start(), p_r.start(), p_g.start(), p_b.start();
	     p_o.is_valid();
	     p_o.next(), p_r.next(), p_g.next(), p_b.next())
	{
	  value::rgb<n>& c = p_o.val();
	  c.red()   = p_r.val();
	  c.green() = p_g.val();
	  c.blue()  = p_b.val();
	}

	trace::exiting("scribo::preprocessing::internal::merge");
	return output;
      }


      inline
      image2d< value::rgb8 >
      diff_abs(const image2d< value::rgb8 >& input1,
	       const image2d< value::rgb8 >& input2)
      {
	image2d< value::rgb8 > output(input1.domain());
	mln_piter_(box2d) p(input1.domain());
	for_all(p)
	{
	  value::rgb8& c = output(p);
	  c.red() = math::diff_abs(input1(p).red(), input2(p).red());
	  c.green() = math::diff_abs(input1(p).green(), input2(p).green());
	  c.blue() = math::diff_abs(input1(p).blue(), input2(p).blue());
	}
	return output;
      }


      inline
      image2d< value::rgb8 >
      inverted_diff_abs(const image2d< value::rgb8 >& input1,
			const image2d< value::rgb8 >& input2)
      {
	image2d< value::rgb8 > output(input1.domain());
	mln_piter_(box2d) p(input1.domain());
	for_all(p)
	{
	  value::rgb8& c = output(p);
// 	  c.red()   = 255 - math::diff_abs(input1(p).red(), input2(p).red());
// 	  c.green() = 255 - math::diff_abs(input1(p).green(), input2(p).green());
// 	  c.blue()  = 255 - math::diff_abs(input1(p).blue(), input2(p).blue());

	  c.red()   = 255 - math::min(2 * math::diff_abs(input1(p).red(), input2(p).red()), 255);
	  c.green() = 255 - math::min(2 * math::diff_abs(input1(p).green(), input2(p).green()), 255);
	  c.blue()  = 255 - math::min(2 * math::diff_abs(input1(p).blue(), input2(p).blue()), 255);
	}
	return output;
      }


      inline
      unsigned dist(const rgb8& c1, const rgb8& c2)
      {
	unsigned d = 0;
	d += std::abs(c1.red()   - c2.red());
	d += std::abs(c1.green() - c2.green());
	d += std::abs(c1.blue()  - c2.blue());
	return d;
      }


      inline
      image2d<rgb8>
      background_analyze(const image2d<rgb8>& input,
			 unsigned lambda, unsigned delta)
      {
	trace::entering("scribo::preprocessing::internal::background_analyze");

	image2d<value::int_u8> r, g, b;
	split(input, r, g, b);
	image2d<rgb8> closed, opened;
	closed = merge(morpho::closing::area(r, c4(), lambda),
		       morpho::closing::area(g, c4(), lambda),
		       morpho::closing::area(b, c4(), lambda));
	opened = merge(morpho::opening::area(r, c4(), lambda),
		       morpho::opening::area(g, c4(), lambda),
		       morpho::opening::area(b, c4(), lambda));

	image2d<bool> mask(input.domain());
	mln_piter_(box2d) p(input.domain());
	for_all(p)
	  mask(p) = (dist(closed(p), opened(p)) >= delta);

	image2d<rgb8> output = duplicate(input);
	data::fill((output | pw::value(mask)).rw(), rgb8(255,255,0));

//     {
//       io::ppm::save(output, "temp_output.ppm");
//       io::pbm::save(mask, "temp_mask.pbm");
//     }

	unsigned nblobs;
	image2d<unsigned> lab, con;
	lab = labeling::blobs(mask, c4(), nblobs);

//     {
//       io::ppm::save(labeling::colorize(rgb8(),
// 				       lab,
// 				       nblobs),
// 		    "temp_blobs.ppm");
//     }

	con = morpho::elementary::dilation(lab, c8());
	data::fill((con | (pw::value(lab) != pw::cst(0u))).rw(),
		   0u);

//     {
//       io::ppm::save(labeling::colorize(rgb8(),
// 				       con,
// 				       nblobs),
// 		    "temp_con.ppm");
//     }

	typedef accu::stat::mean< rgb8, algebra::vec<3,float>, rgb8 > A;
	mln::util::array<rgb8> m = labeling::compute(A(), input, con, nblobs);

	data::fill((output | pw::value(mask)).rw(),
		   data::transform(lab, m));

	trace::exiting("scribo::preprocessing::internal::toggle");
	return output;
      }


    } // end of namespace scribo::internal




    // Facade

    template <typename I>
    mln::util::couple<mln_concrete(I), mln_concrete(I)>
    split_bg_fg(const Image<I>& input_, unsigned lambda, unsigned delta)
    {
      trace::entering("scribo::preprocessing::split_bg_fg");

      using namespace mln;

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mln_precondition(lambda >= 1);
      mln_precondition(delta >= 1);

      image2d<value::rgb8>
	bg = internal::background_analyze(input, lambda, delta);
      image2d<value::rgb8> fg = internal::inverted_diff_abs(input, bg);

      trace::exiting("scribo::preprocessing::split_bg_fg");
      return mln::make::couple(bg, fg);
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::preprocessing

} // end of namespace scribo

#endif // ! SCRIBO_PREPROCESSING_SPLIT_BG_FG_HH
