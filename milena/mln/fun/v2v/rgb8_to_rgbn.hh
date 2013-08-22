// Copyright (C) 2010, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_FUN_V2V_RGB8_TO_RGBN_HH
# define MLN_FUN_V2V_RGB8_TO_RGBN_HH

# include <mln/core/concept/function.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/rgb.hh>
# include <mln/core/contract.hh>

/// \file
/// \brief Conversion from mln::value::rgb8 to mln::value::rgbn.


namespace mln
{

  namespace fun
  {

    namespace v2v
    {


      /// \brief Convert an mln::value::rgb8 to an mln::value::rgbn.
      ///
      /// \tparam n  The quantification of the output value (where n < 8).
      ///
      /// \ingroup modfunv2v
      template <unsigned n>
      struct rgb8_to_rgbn : Function_v2v< rgb8_to_rgbn<n> >
      {
	typedef value::rgb8 argument;
	typedef value::rgb<n> result;

	/// \brief Perform the conversion.
	///
	/// \pre The parameter (constant) n must be lower than 8.
	///
	/// \param[in] c  The input value.
	///
	/// Conversion is done by computing the size by which we
	/// divide each rgb component.
	result operator()(const argument& c) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <unsigned n>
      typename rgb8_to_rgbn<n>::result
      rgb8_to_rgbn<n>::operator()(const argument& c) const
      {
	mlc_bool(n < 8)::check();

	unsigned size = pow(2, (8 - n));
	result res(c.red() / size, c.green() / size, c.blue() / size);

	return res;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_RGB8_TO_RGBN_HH
