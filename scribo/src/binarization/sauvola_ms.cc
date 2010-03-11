// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/alias/neighb2d.hh>
#include <mln/data/stretch.hh>
#include <mln/data/paste.hh>
// #include <mln/debug/iota.hh>
// #include <mln/debug/quiet.hh>
// #include <mln/debug/println.hh>
// #include <mln/debug/println_with_border.hh>
#include <mln/debug/filename.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/all.hh>
#include <mln/literal/colors.hh>
#include <mln/math/sqr.hh>
#include <mln/math/abs.hh>

#include <mln/subsampling/antialiased.hh>

#include <mln/transform/influence_zone_geodesic.hh>
#include <mln/util/timer.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/border/equalize.hh>
#include <mln/border/mirror.hh>
#include <mln/border/adjust.hh>
#include <mln/debug/filename.hh>

#include <mln/core/box_runend_piter.hh>
#include <mln/core/box_runstart_piter.hh>

#include <scribo/subsampling/integral_single_image.hh>
//#include <scribo/subsampling/integral.hh>

#include <scribo/core/macros.hh>
#include <scribo/core/object_image.hh>

#include <scribo/filter/objects_small.hh>
#include <scribo/filter/objects_large.hh>

#include <scribo/filter/objects_thin.hh>
#include <scribo/filter/objects_thick.hh>

#include <scribo/primitive/extract/objects.hh>

#include <scribo/binarization/sauvola_threshold.hh>
#include <scribo/binarization/internal/first_pass_functor.hh>

#include <scribo/canvas/integral_browsing.hh>

#include <scribo/debug/usage.hh>
#include <scribo/debug/save_object_diff.hh>


namespace mln
{

  using value::int_u8;


  unsigned my_find_root(image2d<unsigned>& parent, unsigned x)
  {
    if (parent.element(x) == x)
      return x;
    return parent.element(x) = my_find_root(parent,
					    parent.element(x));
  }


  image2d<int_u8>
  compute_t_n_and_e_2(const image2d<int_u8>& sub, image2d<int_u8>& e_2,
		      unsigned lambda_min, unsigned lambda_max,
		      unsigned q, unsigned i, unsigned w,
		      const image2d<util::couple<double,double> >& integral_sum_sum_2)
  // lambdas: limits of component cardinality at this scale
  {
    typedef image2d<int_u8> I;
    typedef point2d P;

    util::timer tt;
    float t_;

    unsigned ratio = std::pow(q, i - 2);  // Ratio in comparison to e_2


    tt.restart();

    unsigned w_2 = w / 2 * ratio;

    // 1st pass
    scribo::binarization::internal::first_pass_functor< image2d<int_u8> >
      f(sub);
    scribo::canvas::integral_browsing(integral_sum_sum_2,
				      ratio,
				      w_2, w_2,
				      f);

//     debug::println("mask", f.msk);
//     debug::println("parent", f.parent);
//     debug::println("card", f.card);

    t_ = tt;
    if (mln::debug::quiet)
      std::cout << "1st pass - " << t_ << std::endl;

    tt.restart();

    {
      util::array<mln_value_(I) *> ptr(ratio);
      unsigned nrows = geom::nrows(e_2);

      mln_box_runend_piter_(I) sp(sub.domain()); // Backward.
      unsigned ncols = sp.run_length();
      for_all(sp)
      {
	unsigned p = &sub(sp) - sub.buffer(); // Offset
	P site = sp;

	{
	  P tmp = site * ratio;

	  // FIXME: to be removed!
	  if (tmp.row() + ratio >= nrows)
	    ptr.resize(nrows - tmp.row());

	  ptr(0) = &e_2(tmp);
	  // FIXME: pointers could just be updated with an offset.
	  for (unsigned j = 1; j < ptr.size(); ++j)
	  {
 	    tmp[0] += 1;
	    ptr(j) = & e_2(tmp);
	  }
	}

	for (unsigned j = 0; j < ncols; ++j)
	{
	  if (f.msk.element(p))
	  {

	    mln_site_(I) sq = site * ratio;

	    if (f.parent.element(p) == p)
	    {
	      // test over the component cardinality
	      f.msk.element(p) = f.card.element(p) > lambda_min
  		&& f.card.element(p) < lambda_max;

	      if (f.msk.element(p) && e_2(sq) == 0u)
	      {
		for (unsigned l = 0; l < ptr.size(); ++l)
		  std::memset(ptr(l), i, ratio * sizeof(mln_value_(I)));
//  		debug(sq) = i;
	      }

	    }
	    else
	    {
	      // Propagation
	      f.msk.element(p) = f.msk.element(f.parent.element(p));

	      if (f.msk.element(p) && e_2(sq) == 0u)
	      {
		for (unsigned l = 0; l < ptr.size(); ++l)
		  std::memset(ptr(l), i, ratio * sizeof(mln_value_(I)));
//  		debug(sq) = i;
	      }

	    }
	  }

	  for (unsigned l = 0; l < ptr.size(); ++l)
	    ptr(l) -= ratio;

	  --site[1];
	  --p;
	}

      }

      t_ = tt;
      if (mln::debug::quiet)
	std::cout << "2nd pass - " << t_ << std::endl;

//       io::pgm::save(e_2, mln::debug::filename("e.pgm", i));
//       io::pgm::save(debug, mln::debug::filename("debug.pgm", i));
//       debug::println(msk);
//       io::pbm::save(f.msk, mln::debug::filename("mask.pbm", i));
//       io::pgm::save(data::stretch(int_u8(), card), mln::debug::filename("card.pgm"));
    } // end of 2nd pass

//     io::pgm::save(f.t_sub, mln::debug::filename("t.pgm", i));
    return f.t_sub;
  }



  template <typename I, typename J, typename K>
  mln_ch_value(I, bool)
  binarize_generic(const I& in, const J& e2, const util::array<K>& t_ima,
		   unsigned s)
  {
    mln_ch_value(I,bool) out;
    initialize(out, in);

    typedef const mln_value(K)* ptr_type;

    ptr_type ptr_t[5];
    ptr_t[2] = & t_ima[2].at_(0, 0);
    ptr_t[3] = & t_ima[3].at_(0, 0);
    ptr_t[4] = & t_ima[4].at_(0, 0);


    const mln_value(J)* ptr_e2   = & e2.at_(0, 0);
    const mln_value(I)* ptr__in = & in.at_(0, 0);
    bool*    ptr__out = & out.at_(0, 0);


    // Since we iterate from a smaller image in the largest ones and
    // image at scale 1 does not always have a size which can be
    // divided by (4*s), some sites in the border may not be processed
    // and we must skip them.
    unsigned more_offset = in.border() - ((4 * s) - in.ncols() % (4 * s));

    if (more_offset == (4 * s))
      more_offset = 0; // No offset needed.

    const int
      nrows4 = t_ima[4].nrows(), ncols4 = t_ima[4].ncols(),


      delta1  = in.delta_index(dpoint2d(+1, -(s - 1))),
      delta1b = in.delta_index(dpoint2d(+1, -(s + s - 1))),
      delta1c = in.delta_index(dpoint2d(-(s + s - 1), +1)),
      delta1d = in.delta_index(dpoint2d(+1, -(s * 4 - 1))),
      delta1e = in.delta_index(dpoint2d(-(s * 4 - 1), +1)),
      delta1f = in.delta_index(dpoint2d(-(s - 1), +1)),

      delta2  = t_ima[2].delta_index(dpoint2d(+1, -1)),
      delta2b = t_ima[2].delta_index(dpoint2d(+1, -3)),
      delta2c = t_ima[2].delta_index(dpoint2d(-3, +1)),

      delta3  = t_ima[3].delta_index(dpoint2d(+1, -1)),

      eor1 = in.delta_index(dpoint2d(+4 * s, - in.ncols() - in.border())) + more_offset,
      eor2 = t_ima[2].delta_index(dpoint2d(+4,- t_ima[2].ncols())),
      eor3 = t_ima[3].delta_index(dpoint2d(+2,- t_ima[3].ncols())),
      eor4 = t_ima[4].delta_index(dpoint2d(+1,- t_ima[4].ncols()));

    mln_value(J) threshold;
    for (int row4 = 0; row4 < nrows4; ++row4)
    {
      for (int col4 = 0; col4 < ncols4; ++col4)
      {
	// top left  1
	{
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1f; ptr__in += delta1f;
	  }

	  ++ptr_t[2]; ++ptr_e2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1b; ptr__in += delta1b;
	  }

	  ptr_t[2] += delta2; ptr_e2 += delta2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1f; ptr__in += delta1f;

	  }

	  ++ptr_t[2]; ++ptr_e2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1c; ptr__in += delta1c;
	  }

	  ptr_t[2] -= delta2; ptr_e2 -= delta2;
	}

	// top right 1
	ptr_t[3] += 1;
	{
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1f; ptr__in += delta1f;
	  }

	  ++ptr_t[2]; ++ptr_e2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1b; ptr__in += delta1b;
	  }

	  ptr_t[2] += delta2; ptr_e2 += delta2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1f; ptr__in += delta1f;
	  }

	  ++ptr_t[2]; ++ptr_e2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1d; ptr__in += delta1d;
	  }

	  ptr_t[2] += delta2b; ptr_e2 += delta2b;
	}

	// bot left  1
	ptr_t[3] += delta3;
	{
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1f; ptr__in += delta1f;
	  }

	  ++ptr_t[2]; ++ptr_e2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1b; ptr__in += delta1b;
	  }

	  ptr_t[2] += delta2; ptr_e2 += delta2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1f; ptr__in += delta1f;
	  }

	  ++ptr_t[2]; ++ptr_e2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1c; ptr__in += delta1c;
	  }

	  ptr_t[2] -= delta2; ptr_e2 -= delta2;
	}

	// bot right 1
	ptr_t[3] += 1;
	{
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1f; ptr__in += delta1f;
	  }

	  ++ptr_t[2]; ++ptr_e2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1b; ptr__in += delta1b;
	  }

	  ptr_t[2] += delta2; ptr_e2 += delta2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1f; ptr__in += delta1f;
	  }

	  ++ptr_t[2]; ++ptr_e2;
	  threshold = *ptr_t[*ptr_e2];
	  {
	    for (unsigned i = 1; i < s; ++i)
	    {
	      for (unsigned j = 1; j < s; ++j)
	      {
		*ptr__out = *ptr__in < threshold;
		++ptr__out; ++ptr__in;
	      }

	      *ptr__out = *ptr__in < threshold;
	      ptr__out += delta1; ptr__in += delta1;
	    }

	    for (unsigned j = 1; j < s; ++j)
	    {
	      *ptr__out = *ptr__in < threshold;
	      ++ptr__out; ++ptr__in;
	    }
	    *ptr__out = *ptr__in < threshold;
	    ptr__out += delta1e; ptr__in += delta1e;
	  }
	}

	// bot right -> next top left
	ptr_t[2] += delta2c; ptr_e2 += delta2c;
	ptr_t[3] = ptr_t[3] - delta3;
	ptr_t[4] += 1;
      }

      // eof -> next bof
      ptr__out += eor1; ptr__in  += eor1;
      ptr_t[2] += eor2; ptr_e2 += eor2;
      ptr_t[3] += eor3;
      ptr_t[4] += eor4;
    }


//     mln::debug::println(out);

    return out;
  }




  unsigned sub(unsigned nbr, unsigned down_scaling)
  {
    return (nbr + down_scaling - 1) / down_scaling;
  }

  template <typename I>
  util::array<util::couple<mln_domain(I), unsigned> >
  compute_sub_domains(const I& ima, unsigned n_scales, unsigned s)
  {
    util::array<util::couple<unsigned, unsigned> > n(n_scales + 2);

    n(1) = make::couple(ima.nrows(), ima.ncols());
    n(2) = make::couple(sub(n(1).first(), s),
			sub(n(1).second(), s));
    for (unsigned i = 3; i <= n_scales + 1; ++i)
      n(i) = make::couple(sub(n(i - 1).first(), 2),
			  sub(n(i - 1).second(), 2));


    util::array<util::couple<mln_domain(I), unsigned> > out(n.size());
    out(0) = make::couple(make::box2d(1,1), 1u);
    out(1) = make::couple(make::box2d(ima.nrows(), ima.ncols()), 2u);
    out(n_scales + 1) = make::couple(make::box2d(n(n_scales + 1).first(),
						 n(n_scales + 1).second()),
				     1u);

    for (unsigned i = n_scales; i > 1; --i)
      out(i) = make::couple(make::box2d(2 * out(i + 1).first().nrows(),
					2 * out(i + 1).first().ncols()),
			    2 * out(i + 1).second());

    out(1).second() = std::max(out(2).first().ncols() * s - ima.ncols(),
			       out(2).first().nrows() * s - ima.nrows());

//     out(1).second() = std::max(ima.ncols() % (4 * s),
// 			       ima.nrows() % (4 * s));

// out(2).first().ncols() * s - ima.ncols(),
// 			       out(2).first().nrows() * s - ima.nrows() );

    return out;
  }



  bool
  check_args(int argc, char * argv[])
  {
    if (argc < 5 || argc > 7)
      return false;

//     int nb_scale = atoi(argv[3]);
    int s = atoi(argv[3]);
//     int q = atoi(argv[5]);

//     if (q < 2)
//     {
//       std::cout << "q must be greater than 2." << std::endl;
//       return false;
//     }
    if (s < 1 || s > 3)// || s < q)
    {
      std::cout << "s must be set to 2 or 3."
		<< std::endl;
      return false;
    }


//     if (nb_scale < 1)
//     {
//       std::cout << "Not enough scales." << std::endl;
//       return false;
//     }

//     if ((argc - 7) != (nb_scale - 1))
//     {
//       std::cout << "Not enough area threshold."
// 		<< "There must be nb_scale - 1 thresholds."
// 		<< std::endl;
//       return false;
//     }

    return true;
  }


  void data_rand(image2d<unsigned>& e2)
  {
    unsigned v = 2;
    mln_piter_(box2d) p(e2.domain());
    for_all(p)
    {
      e2(p) = v++;
      if (v == 5) v = 2;
    }
  }


} // end of namespace mln;




const char *args_desc[][2] =
{
  { "input.pgm", "A graylevel image." },
  { "w", "Window size. (Common value: 51)" },
  { "s", "First subsampling ratio (Common value: 2)." },
  { "min_area",    "Minimum object area (relative to scale 2) (Common value: 200)" },
  { "debug", "Display debug/bench data if set to 1" },
  {0, 0}
};




int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;
  using value::rgb8;
  using value::int_u8;
  using value::int_u16;
  using value::label_16;

  typedef image2d<label_16> L;

  if (!check_args(argc, argv))
    return scribo::debug::usage(argv,
				"Multi-Scale Binarization of a color image based on Sauvola's algorithm.",
				"input.pgm w s area_thresholds output.pbm [debug]",
				args_desc, "A binary image.");

  trace::entering("main");

  mln::debug::internal::filename_prefix = argv[1];

  // Window size.
  unsigned w = atoi(argv[2]);

  // First subsampling scale.
  unsigned s = atoi(argv[3]);

  // Number of subscales.
  unsigned nb_subscale = 3;//atoi(argv[3]);

  // Subscale step.
  unsigned q = 2;//atoi(argv[5]);

  mln::debug::quiet = true;

  if (argc == 7)
    mln::debug::quiet = ! atoi(argv[6]);


  if (mln::debug::quiet)
    std::cout << "Running Sauvola_ms with w = " << w
	      << ", s = " << s
	      << ", nb_subscale = " << nb_subscale
	      << ", q = " << q
	      << std::endl;

  typedef image2d<value::int_u8> I;
  dpoint2d none(0, 0);

  mln::util::timer
    timer_,
    sauvola_timer_;;

  // Tmp variable used for timer results;
  float t_;

  I input_full;
  io::pgm::load(input_full, argv[1]);

  {
    unsigned max_dim = math::max(input_full.ncols() / 2,
				 input_full.nrows() / 2);
    if (w > max_dim)
    {
      std::cout << "------------------" << std::endl;
      std::cout << "The window is too large! Image size is only "
		<< input_full.nrows() << "x" << input_full.ncols()
		<< std::endl
		<< "Window size must not exceed " << max_dim
		<< std::endl;
      return 1;
    }
  }

//   I input_full(9,9);
//   mln::debug::iota(input_full);

  sauvola_timer_.start();

  unsigned lambda_min = atoi(argv[4]);
  unsigned lambda_max = lambda_min * q; // * atoi(argv[7])


  util::array<I> t_ima;

  // Make sure t_ima indexes start from 2.
  {
    I dummy(1,1);
    for (unsigned i = 0; i < nb_subscale + 2; ++i)
      t_ima.append(dummy);
  }



  image2d<int_u8> e_2;
  util::array<I> sub_ima;

  // Make sure sub_ima indexes start from 2.
  {
    I dummy(1,1);
    sub_ima.append(dummy);
    sub_ima.append(dummy);
  }


  timer_.restart();

  util::array<util::couple<box2d, unsigned> >
    sub_domains = compute_sub_domains(input_full, nb_subscale, s);

  if (mln::debug::quiet)
    std::cout << "adjusting input border to " << sub_domains(1).second()
	      << std::endl;

  border::adjust(input_full, sub_domains(1).second());
  border::mirror(input_full);

//   mln::debug::println_with_border(input_full);

  t_ = timer_;
  if (mln::debug::quiet)
    std::cout << "sub domains computed and adjust input border size  - "
	      << t_ << std::endl;

  // Resize input and compute integral images.
  timer_.restart();
  image2d<util::couple<double,double> > integral_sum_sum_2;

  if (mln::debug::quiet)
    std::cout << "sub_domain(2).domain() == " << sub_domains(2).first() << std::endl;

  sub_ima.append(scribo::subsampling::integral(input_full, s,
					       integral_sum_sum_2,
					       sub_domains[2].first(),
					       sub_domains[2].second()));

//   mln::debug::println(integral_sum_sum_2);

  t_ = timer_;
  if (mln::debug::quiet)
    std::cout << "subsampling 1 -> 2 And integral images - " << t_
	      << " - nsites = "
	      << input_full.domain().nsites() << " -> "
	      << sub_ima[2].domain().nsites() << " - "
	      << input_full.domain() << " -> "
	      << sub_ima[2].domain() << std::endl;


  for (unsigned i = 3; i <= nb_subscale + 1; ++i)
  {
    timer_.restart();
    sub_ima.append(mln::subsampling::antialiased(sub_ima[i - 1], q, none,
						 sub_domains[i].first(),
						 sub_domains[i].second()));
    t_ = timer_;
    if (mln::debug::quiet)
      std::cout << "subsampling " << (i - 1) << " -> " << i
		<< " - " << t_
		<< " - nsites = "
		<< sub_ima[i].domain().nsites() << " - "
		<< sub_ima[i].domain()
		<< std::endl;
  }


  initialize(e_2, sub_ima[2]);
  data::fill(e_2, 0u);

  // Compute threshold image.

  // Highest scale -> no maximum component size.
  {
    timer_.restart();
    int i = sub_ima.size() - 1;
    unsigned ratio = std::pow(q, i - 2); // Ratio compared to e_2
    t_ima[i] = compute_t_n_and_e_2(sub_ima[i], e_2,
				   lambda_min / ratio,
				   mln_max(unsigned),
				   q, i, w, integral_sum_sum_2);

    t_ = timer_;
    if (mln::debug::quiet)
      std::cout << "Scale " << i
		<< " - 1/"  << s * ratio
		<< " compute t_n and update e - " << t_ << std::endl;
  }

  // Other scales -> maximum and minimum component size.
  {
    for (int i = sub_ima.size() - 2; i > 2; --i)
    {
      timer_.restart();
      unsigned ratio = std::pow(q, i - 2); // Ratio compared to e_2
      t_ima[i] = compute_t_n_and_e_2(sub_ima[i], e_2,
				     lambda_min / ratio,
				     lambda_max / ratio,
				     q, i, w, integral_sum_sum_2);

      t_ = timer_;
      if (mln::debug::quiet)
	std::cout << "Scale " << i
		  << " - 1/"  << s * ratio
		  << " compute t_n and update e - " << t_ << std::endl;
    }
  }

  // Lowest scale -> no minimum component size.
  {
    timer_.restart();
    t_ima[2] = compute_t_n_and_e_2(sub_ima[2], e_2, 0, lambda_max,
				   1, 2, w, integral_sum_sum_2);
    t_ = timer_;
    if (mln::debug::quiet)
      std::cout << "Scale " << 2
		<< " - 1/" << s
		<< " compute t_n and update e - " << t_ << std::endl;

  }

  if (mln::debug::quiet)
    std::cout << "--------" << std::endl;
//   io::pgm::save(e_2, mln::debug::filename("e.pgm"));



  timer_.restart();
  e_2 = transform::influence_zone_geodesic(e_2, c8());
  t_ = timer_;
  if (mln::debug::quiet)
    std::cout << "influence zone - " << t_ << std::endl;



// Saving influence zone scale image.
//   io::pgm::save(e_2, mln::debug::filename("e_ext.pgm"));
//   io::pbm::save(bin, argv[8]);
//   io::pgm::save(t, mln::debug::filename("t.pgm"));

//   for (unsigned i = 2; i < t_ima.size(); ++i)
//     io::pgm::save(t_ima[i], mln::debug::filename("t.pgm", i));


  timer_.restart();
  image2d<bool> out_new = binarize_generic(input_full, e_2, t_ima, s);
//  image2d<bool> out_new = binarize(input_full, e_2, t_ima);
  t_ = timer_;
  if (mln::debug::quiet)
    std::cout << "Compute bin - " << t_ << std::endl;

  t_ = sauvola_timer_;
  if (mln::debug::quiet)
    std::cout << "Sauvola : " << t_ << std::endl;
  io::pbm::save(out_new, argv[5]);
}

