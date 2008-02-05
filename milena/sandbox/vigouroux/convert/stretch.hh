// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef OLENA_CONVERT_STRETCH_HH
# define OLENA_CONVERT_STRETCH_HH

# include <mlc/type.hh>

# include <oln/basics.hh>

# include <ntg/basics.hh>

# include <oln/convert/abstract/conversion.hh>

# include <set>

# include <vector>

namespace oln {

  namespace convert {

    /*! Functor to stretch a value from a range Input to a range Output.
    **
    ** \see stretch_balance
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/convert/stretch.hh>
    ** #include <ntg/all.hh>
    ** #include <iostream>
    **
    ** int main()
    ** {
    **   oln::image2d<ntg::int_u8> lena = oln::load(IMG_IN "lena256.pgm");
    **
    **   oln::io::save(apply(oln::convert::stretch<ntg::int_u<3> >(), lena),
    ** 		IMG_OUT "oln_convert_stretch.pgm");
    ** }
    ** \endcode
    ** \image html lena256_pgm.png
    ** \image latex lena256_pgm.png
    ** =>
    ** \image html oln_convert_stretch.png
    ** \image latex oln_convert_stretch.png
    */
    template<class Output, class Exact = mlc::final>
    struct stretch
      : public abstract::conversion_to_type<Output, typename mlc::exact_vt<stretch<Output, Exact>, Exact>::ret >
    {
      template< class Input >
      Output doit(const Input& v) const {
	return Output(ntg::cast::rbound<Output, float>
		      (
		       double(v - ntg_min_val(Input))
		       / double(ntg_max_val(Input) - ntg_min_val(Input))
		       * (ntg_max_val(Output) - ntg_min_val(Output))
		       + ntg_min_val(Output))
		      );
      }

      static std::string
      name()
      {
	// FIXME: Exact is not an integre type !
        return std::string("stretch<") + ntg_name(Output) + ", "
          + Exact::name() + ">";
      }
    };

    /*! Stretch the value of an image.
    **
    ** This function stretches values between \a min_in and \a max_in
    ** of an image \a in, to a range that goes from \a min_out to \a max_out.
    ** \arg in Input image, must be have scalar values
    ** \arg min_in Lower bound of the range in the input. All values smaller
    ** than min_in are converted to \a min_out.
    ** \arg max_in Upper bound of the range in the input. All values
    ** greater than max_in
    **  are converted to \a max_out.
    ** \arg min_out Low bound of the range in the output.
    ** \arg max_out Upper bound of the range in the output.
    **
    ** Difference between "apply(stretch<T>(), im)" and "stretch_balance<T>(im)":
    ** the first one stretches all the range of oln_value_type(T), the second
    ** stretches only values that appear is the image:
    ** \code
    ** #include <oln/basics1d.hh>
    ** #include <oln/convert/stretch.hh>
    ** #include <ntg/all.hh>
    ** #include <iostream>
    **
    ** int main()
    ** {
    **
    **   oln::image1d<ntg::int_u<17> > im(4);
    **   im(0) = 0; im(1) = 2; im(2) = 5000; im(3) = 4000;
    **
    **   // print "0 170 511 340"
    **   //(values of the *image* are dispatched on the range [0..2^9-1])
    **   std::cout << oln::convert::stretch_balance<ntg::int_u<9> >(im) << std::endl;
    **
    **   //print " 0 0 19 16"   (19 is the result of 5000 * 2^9 / 2^17)
    **   //(values of *int_u<17>* are dispatched on the range [0..2^9-1])
    **   std::cout << apply(oln::convert::stretch<ntg::int_u<9> >(), im) << std::endl;
    ** }
    ** \endcode
    **
    ** This function is useful to stretch images of label:
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/convert/stretch.hh>
    ** #include <oln/level/cc.hh>
    ** #include <ntg/all.hh>
    ** #include <iostream>
    **
    ** int main()
    ** {
    **   oln::image2d<ntg::bin> light = oln::load(IMG_IN "face_se.pbm");
    **
    **   //Extraction of the connected components:
    **   unsigned card;
    **   oln::image2d<ntg::int_u8> cc
    **    = oln::level::frontp_connected_component<ntg::int_u8>(light,
    ** 							  oln::neighb_c8(),
    ** 							  card);
    **   oln::io::save(cc, IMG_OUT "oln_convert_stretch_dark.pgm");
    **   oln::io::save(oln::convert::stretch_balance<ntg::int_u8>(cc),
    ** 		IMG_OUT "oln_convert_stretch_balance.pgm");
    ** }
    ** \endcode
    ** \image html face_se_pbm.png
    ** \image latex face_se_pbm.png
    ** => Without stretch_balance:
    ** \image html oln_convert_stretch_dark.png
    ** \image latex oln_convert_stretch_dark.png
    ** => With stretch_balance:
    ** \image html oln_convert_stretch_balance.png
    ** \image latex oln_convert_stretch_balance.png
    */
   template<class DestValue, class I>
   typename mute<I, DestValue>::ret
   stretch_balance(const oln::abstract::non_vectorial_image<I> &in,
		   const oln_value_type(I) & min_in
		    = ntg_min_val(oln_value_type(I)),
		   const oln_value_type(I) & max_in
		    = ntg_max_val(oln_value_type(I)),
		   const DestValue & min_out = ntg_min_val(DestValue),
		   const DestValue & max_out = ntg_max_val(DestValue))
    {
      typedef typename
	ntg_is_a(DestValue, ntg::non_vectorial)::ensure_type ensure_type;

      typename mute<I, DestValue>::ret out(in.size());

      //FIXME: I would like to remove the static_cast.
      std::vector<ntg_cumul_type(DestValue)>
	tab(static_cast<int>(max_in - min_in + 1));
      typedef typename std::set<oln_value_type(I)> set_type;
      set_type s;
      oln_iter_type(I) it(in);

      for_all(it)
	if (in[it] <= max_in && in[it] >= min_in)
	      s.insert(in[it]);
      if (s.size() == 1)
	{
	  for_all(it)
	    out[it] = ntg_zero_val(DestValue);
	  return out;
	}
      {
	unsigned cpt = 0;
	for (typename set_type::const_iterator it(s.begin());
	     it != s.end(); ++it, ++cpt)
	  tab[*it - min_in] = cpt * (max_out - min_out) / (s.size() - 1);
      }
      for_all(it)
	if (min_in <= in[it])
	  {
	    if (in[it] <= max_in)
	      out[it] = tab[in[it] - min_in] + min_out;
	    else
	      out[it] = max_out;
	  }
      else
	out[it] = min_out;
      return out;
    }
  } // end of convert.

} // end of oln.

#endif // OLENA_CONVERT_STRETCH_HH
