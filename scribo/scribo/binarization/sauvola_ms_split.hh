// Copyright (C) 2010, 2011, 2012, 2013 EPITA Research and Development
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

#ifndef SCRIBO_BINARIZATION_SAUVOLA_MS_SPLIT_HH
# define SCRIBO_BINARIZATION_SAUVOLA_MS_SPLIT_HH

# include <mln/core/concept/image.hh>

# include <mln/border/resize.hh>

# include <mln/data/transform.hh>
# include <mln/data/split.hh>

# include <scribo/binarization/sauvola_ms.hh>

/// \file
///
/// \brief Binarize a color image merging the binarization of each
/// component using Sauvola's algorithm.

namespace scribo
{

  using namespace mln;


  namespace binarization
  {

    /*! \brief Binarize a color image merging the binarization of each
        component using Sauvola's algorithm.

      \param[in] input_1 A color image.
      \param[in] w_1 The window size used to compute stats.
      \param[in] s The scale factor used for the first subscaling.
      \param[in] min_ntrue A site is set to 'True' in the output if it
                           is set to 'True' at least \p min_ntrue
                           components. Possible values: 1, 2, 3.
      \param[in] k2 Sauvola's formula parameter.
      \param[in] k3 Sauvola's formula parameter.
      \param[in] k4 Sauvola's formula parameter.

      \p w_1 is expressed according to the image at scale 0, i.e. the
      original size.

      \return A Boolean image.

      \ingroup grpalgobinsauvola
    */
    template <typename I>
    mln_ch_value(I, bool)
    sauvola_ms_split(const Image<I>& input_1, unsigned w_1,
		     unsigned s, unsigned min_ntrue, double k2,
		     double k3, double k4);

    /*! \overload
      k2, k3 and k4 are set to \p K.

      \ingroup grpalgobinsauvola
    */
    template <typename I>
    mln_ch_value(I, bool)
    sauvola_ms_split(const Image<I>& input_1, unsigned w_1,
		     unsigned s, unsigned min_ntrue, double K);


    /*! \overload
      k2, k3 and k4 are set to 0.34.

      \ingroup grpalgobinsauvola
    */
    template <typename I>
    mln_ch_value(I, bool)
    sauvola_ms_split(const Image<I>& input_1, unsigned w_1,
		     unsigned s, unsigned min_ntrue);



# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_ch_value(I, bool)
    sauvola_ms_split(const Image<I>& input_1_, unsigned w_1,
		     unsigned s, unsigned min_ntrue, double k2,
		     double k3, double k4)
    {
      mln_trace("scribo::binarization::sauvola_ms_split");

      mln_precondition(exact(input_1_).is_valid());
      mlc_is(mln_value(I), value::rgb8)::check();

      const I& input_1 = exact(input_1_);

      typedef mln_ch_value(I,bool) bin_t;

      mln_ch_value(I, value::int_u8) r_i, g_i, b_i;

      // Split the rgb8 image into 3 intensity images.
      mln::data::split(input_1, r_i, g_i, b_i);

      bin_t r_b, g_b, b_b;

      r_b = sauvola_ms(r_i, w_1, s);
      g_b = sauvola_ms(g_i, w_1, s);
      b_b = sauvola_ms(b_i, w_1, s);

      border::resize(r_b, input_1.border());
      border::resize(g_b, input_1.border());
      border::resize(b_b, input_1.border());

      bin_t output;
      initialize(output, input_1);

      typedef bool * b_ptr_t;
      b_ptr_t
	out_ptr = output.buffer(),
	r_ptr = r_b.buffer(),
	g_ptr = g_b.buffer(),
	b_ptr = b_b.buffer();

      unsigned ntrue;
      for (unsigned n = 0; n < output.nelements(); ++n)
      {
	ntrue = 0;
	if (*r_ptr)
	  ++ntrue;
	if (*g_ptr)
	  ++ntrue;
	if (*b_ptr)
	  ++ntrue;

	*out_ptr++ = ntrue >= min_ntrue;

	++r_ptr;
	++g_ptr;
	++b_ptr;
      }

      return output;
    }


    template <typename I>
    mln_ch_value(I, bool)
    sauvola_ms_split(const Image<I>& input_1, unsigned w_1,
		     unsigned s, unsigned min_ntrue, double K)
    {
      return sauvola_ms_split(input_1, w_1, s, min_ntrue,
			      K, K, K);
    }


    template <typename I>
    mln_ch_value(I, bool)
    sauvola_ms_split(const Image<I>& input_1, unsigned w_1,
		     unsigned s, unsigned min_ntrue)
    {
      return sauvola_ms_split(input_1, w_1, s, min_ntrue,
			      SCRIBO_DEFAULT_SAUVOLA_K);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_SAUVOLA_MS_SPLIT_HH
