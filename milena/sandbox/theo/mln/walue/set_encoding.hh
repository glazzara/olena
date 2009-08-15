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

#ifndef MLN_WALUE_SET_ENCODING_HH
# define MLN_WALUE_SET_ENCODING_HH

/// \file
/// \brief Routine to set an encoding value.

# include <mln/core/concept/walue.hh>


namespace mln
{

  namespace walue
  {

    namespace internal
    {

      template < typename T0, typename T1, typename T2 >
      struct encoding_base
      {
	encoding_base(const T0& t0, const T1& t1, const T2& t2)
	  : t0(t0), t1(t1), t2(t2)
	{
	}
	const T0& t0;
	const T1& t1;
	const T2& t2;
      };

      template < typename T0, typename T1 >
      struct encoding_base< T0, T1, void >
      {
	encoding_base(const T0& t0, const T1& t1)
	  : t0(t0), t1(t1)
	{
	}
	const T0& t0;
	const T1& t1;
      };

      template < typename T0 >
      struct encoding_base< T0, void, void >
      {
	encoding_base(const T0& t0)
	  : t0(t0)
	{
	}
	const T0& t0;
	const T0& value() const { return t0; }
      };


      template < typename T0,
		 typename T1 = void,
		 typename T2 = void >
      struct encoding : encoding_base<T0,T1,T2>
      {

	template < typename U0 >
	encoding(const U0& u0)
	  : encoding_base<T0,T1,T2>(u0)
	{}

	template < typename U0, typename U1 >
	encoding(const U0& u0, const U1& u1)
	  : encoding_base<T0,T1,T2>(u0, u1)
	{}

	template < typename U0, typename U1, typename U2 >
	encoding(const U0& u0, const U1& u1, const U2& u2)
	  : encoding_base<T0,T1,T2>(u0, u1, u2)
	{}
      };


    } // end of namespace mln::walue::internal


    /// \brief Set an encoding value.
    //
    template <typename T>
    inline
    internal::encoding<T,void,void>
    set_encoding(const T& t)
    {
      internal::encoding<T,void,void> tmp(t);
      return tmp;
    }


    template <typename T0, typename T1>
    inline
    internal::encoding<T0,T1,void>
    set_encoding(const T0& t0, const T1& t1)
    {
      internal::encoding<T0,T1,void> tmp(t0,t1);
      return tmp;
    }

    template <typename T0, typename T1, typename T2>
    inline
    internal::encoding<T0,T1,T2>
    set_encoding(const T0& t0, const T1& t1, const T2& t2)
    {
      internal::encoding<T0,T1,T2> tmp(t0,t1,t2);
      return tmp;
    }


# ifndef MLN_INCLUDE_ONLY

//     namespace internal
//     {

//       template <typename T>
//       inline
//       encoding<T>::encoding(const T& t)
// 	: t_(t)
//       {
//       }

//       template <typename T>
//       inline
//       const T&
//       encoding<T>::value() const
//       {
// 	return t_;
//       }

//       // const T& operator[](unsigned i)) const;

//     } // end of namespace mln::walue::internal

//     template <typename T>
//     inline
//     internal::encoding<T>
//     set_encoding(const T& t)
//     {
//       internal::encoding<T> tmp(t);
//       return tmp;
//     }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::walue

} // end of namespace mln


#endif // ! MLN_WALUE_SET_ENCODING_HH
