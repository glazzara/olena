// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef METALIC_ARRAY_3D_HXX
# define METALIC_ARRAY_3D_HXX

# include <mlc/array/nd.hh>

namespace mlc
{

  //
  //  mlc::array3d_info
  //
  ////////////////////////////////////////
    

  // nrows_	-> total number of rows (1-indexed)
  // ncols_	-> total number of columns (1-indexed)
  // nplanes_	-> total number of planes (1-indexed)

  // center_	-> position of the central element (0-indexed)
  //		   domain : [ 0, card [

  // i_	-> number of elements that have been eaten yet (0-indexed)
  //		   domain : [ 0, card ] -> watch out !!

  // Center is user-defined

  template < unsigned nplanes_,
	     unsigned nrows_, 
	     unsigned ncols_,
	     unsigned center_ = (ncols_ * nrows_) * (nplanes_ / 2) + ncols_ * (nrows_ / 2) + (ncols_ / 2),
	     unsigned i_ = nplanes_ * nrows_ * ncols_>
  struct array3d_info
  {
    enum {
      nplanes = nplanes_,
      nrows = nrows_,
      ncols = ncols_,
      center = center_,
      center_plane = center_ / (nrows_ * ncols_),
      center_row = (center_ % (nrows_ * ncols_)) / ncols_,
      center_col = (center_ % (nrows_ * ncols_)) % ncols_,
      i = i_,
      card = nplanes_ * nrows_ * ncols_,
      well_formed = (i_ % (ncols_ * nrows) == 0),
      get_real_center = center_
    };

    typedef array3d_info< nplanes_, nrows_, ncols_, center_, i + 1 > next_elt;

  };
    
  // Center is defined automatically, if nrows and ncols are both odd

  template < unsigned nplanes_, unsigned nrows_, unsigned ncols_, unsigned i_ >
  struct array3d_info < nplanes_, nrows_, ncols_, internal::unknown_, i_ >
  {
    enum {
      nplanes = nplanes_,
      nrows = nrows_,
      ncols = ncols_,
      center = internal::unknown_,
      i = i_,
      card = nplanes_ * nrows_ * ncols_,
      well_formed = (i_ % (ncols_ * nrows_) == 0),
      get_real_center = i_ / 2
    };

    typedef array3d_info< nplanes_, nrows_, ncols_, internal::unknown_, i + 1 > next_elt;

  };

  // fwd decl
  template<class, class> struct array3d;

  namespace internal 
  {

    // fwd decl

    template<class T, class Info>
    struct array3d_elt_;

    // for error messages

    template<class U>
    struct here_a_value_is_not_of_type_;


    //
    //  mlc::internal::array3d_start_ decl
    //
    ////////////////////////////////////////

    template<class T>
    struct array3d_start_ {

      array3d_elt_< T, array3d_info< unknown_, unknown_, unknown_, unknown_, 1 > > 
      operator=(T val);
	
      array3d_elt_< T, array3d_info< unknown_, unknown_, unknown_, 0, 1 > > 
      operator=(x_<T> val);
	
      array3d_elt_< T, array3d_info< unknown_, unknown_, unknown_, 0, 1 > > 
      operator=(x_<void> val);

      T ptr[max_card_]; // could be static
    };

    //
    //  mlc::internal::array3d_elt_
    //
    ////////////////////////////////////////

    template<class T, class Info>
    struct array3d_elt_
    {
      typedef array3d_elt_< T, typename Info::next_elt >
      next_elt_t_;

      typedef array3d_elt_< T, array3d_info< Info::nplanes, Info::nrows, Info::ncols, Info::i, Info::i + 1> >
      eat_center_t_;

      typedef array3d_elt_< T, array3d_info< Info::nplanes, Info::nrows, Info::i, Info::center, Info::i > >
      eat_lbrk_t_;

      typedef array3d_elt_< T, array3d_info< Info::nplanes, Info::i / Info::ncols, Info::ncols, Info::center, Info::i > >
      eat_pbrk_t_;

      typedef array3d< array3d_info< Info::i / (Info::ncols * Info::nrows), Info::nrows, Info::ncols, Info::get_real_center, Info::i > , T>
      array3d_t_;

    public:
	
      // Constructor

      array3d_elt_(T* ptr, array3d_start_<T>* arr) : ptr(ptr), arr(arr)
      {
      }


      //
      // Overloading "operator,"
      //


      // elt, elt

      next_elt_t_ operator,(T val)
      {
	is_true<Info::nplanes == unknown_>::ensure();
	*ptr = val;
	return next_elt_t_(ptr + 1, arr);
      }


      // elt, x(elt)		-> center

      eat_center_t_ operator,(x_<T> val)
      {
	is_true<Info::center == unknown_>::ensure();
	*ptr = val.ue; // FIXME : give a *name* to this variable !!
	return eat_center_t_(ptr + 1, arr);
      }


      // elt, x()		-> center

      eat_center_t_ operator,(x_<void>)
      {
	is_true<Info::center == unknown_>::ensure();
	*ptr = T(0);
	return eat_center_t_(ptr + 1, arr);
      }


      // elt, lbrk

      eat_lbrk_t_ operator,(lbrk_)
      {
	is_true<Info::ncols == unknown_>::ensure();
	return eat_lbrk_t_(ptr, arr);
      }


      // elt, pbrk
      eat_pbrk_t_ operator,(pbrk_)
      {
	is_true<Info::nplanes == unknown_>::ensure();
	return eat_pbrk_t_(ptr, arr);
      }

      // elt, end
	
      array3d_t_ operator,(end_type)
      {
	enum { nplanes = (Info::i / (Info::ncols * Info::nrows)) };
	  
	// array is well-formed :
	is_true<Info::well_formed>::ensure();
	// centering is automatic or user-defined :

	is_true<Info::ncols != unknown_>::ensure();
	is_true<Info::nrows != unknown_>::ensure();

	// all of nplanes, nrows and ncols are odd
	// or the center is user-defined

	is_true<	    
	  (Info::ncols % 2 == 1 && Info::nrows % 2 == 1 && nplanes % 2 == 1)
	  || (Info::center != unknown_)
	  >::ensure();
	  
	return array3d_t_(arr->ptr);
      }
	

      // else -> error

      template<class U>
      void operator,(array3d< Info, U >);

      template<class U>
      here_a_value_is_not_of_type_<T> operator,(U u) const;

      T* ptr;
      array3d_start_<T>* arr;
    };


    //
    //  mlc::internal::array3d_start_  impl
    //
    ////////////////////////////////////////

    template<class T> inline 
    array3d_elt_< T, array3d_info< unknown_, unknown_, unknown_, unknown_, 1 > >
    array3d_start_<T>::operator=(T val)
    {
      ptr[0] = val;
      return array3d_elt_< T, array3d_info< unknown_, unknown_, unknown_, unknown_, 1 > >(ptr+1,this);
    }

    template<class T> inline 
    array3d_elt_< T, array3d_info< unknown_, unknown_, unknown_, 0, 1 > >
    array3d_start_<T>::operator=(x_<T> val)
    {
      ptr[0] = val.ue;
      // center <- 0
      return array3d_elt_< T, array3d_info< unknown_, unknown_, unknown_, 0, 1 > >(ptr+1,this);
    }

    template<class T> inline 
    array3d_elt_< T, array3d_info< unknown_, unknown_, unknown_, 0, 1 > >
    array3d_start_<T>::operator=(x_<void> val)
    {
      ptr[0] = T(0);
      // center <- 0
      return array3d_elt_< T, array3d_info< unknown_, unknown_, unknown_, 0, 1 > >(ptr+1,this);
    }

  } // end of internal

} // end of mlc


#endif // ! METALIC_ARRAY_3D_HXX
