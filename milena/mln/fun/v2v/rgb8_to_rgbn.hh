// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
// (LRDE)
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
///
/// \brief Convert rgb8 value to rgbn, n < 8.


namespace mln
{

  namespace fun
  {

    namespace v2v
    {


      /// \brief Convert a rgb8 value to a rgn, n < 8.
      ///
      /// \param n defines the output quantification used for the transformation.
      ///
      /// \ingroup modfunv2v
      //
      template <unsigned n>
      struct rgb8_to_rgbn : Function_v2v< rgb8_to_rgbn<n> >
      {
	typedef value::rgb8   argument;
	typedef value::rgb<n> result;

	/// \brief Convert a rgb8 value to a rgn, n < 8.
	///
	/// \param[in] c the rgb8 value to convert.
	///
	/// Conversion is done by computing the size by which we
	/// divide each rgb component.
	//
	result operator()(const argument& c) const;

      };


# ifndef MLN_INCLUDE_ONLY

      /// \brief Convert a rgb8 value to a rgn, n < 8.
      ///
      /// \param c defines the output quantification used for the transformation.
      ///
      /// \ingroup modfunv2v
      //
      template <unsigned n>
      typename rgb8_to_rgbn<n>::result
      rgb8_to_rgbn<n>::operator()(const argument& c) const
      {
	mln_precondition(8 > n);

	unsigned size = pow(2,(8-n));
	/*
	  std::cout << "c    : " << c << std::endl;
	  std::cout << "red  : " << c.red() << std::endl;
	  std::cout << "size : " << size << std::endl;
	  std::cout << "res  : " << (c.red() / size) << std::endl;
	  std::cout << "max  : " << (mln_max(mln::value::int_u<n>))<< std::endl;
	*/
	result   res(c.red() / size, c.green() / size, c.blue() / size);

	return res;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_RGB8_TO_RGBN_HH
