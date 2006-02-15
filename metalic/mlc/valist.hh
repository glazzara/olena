// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef METALIC_VALIST_HH
# define METALIC_VALIST_HH

# include <mlc/bool.hh>
# include <mlc/cmp.hh>
# include <mlc/uint.hh>


# define mlc_internal_valist_decl_params_		\
   typename E1, typename E2, typename E3,		\
   typename E4, typename E5, typename E6, typename E7,	\
   typename E8, typename E9


# define mlc_internal_valist_params_ \
   E1, E2, E3, E4, E5, E6, E7, E8, E9


# define mlc_internal_valist_elt_spe(I)				\
   template < mlc_internal_valist_decl_params_ >		\
   struct valist_elt_ < mlc_internal_valist_params_, I >	\
     : private ensure_< neq_<E##I, internal::valist_none> >	\
   {								\
     typedef E##I ret;						\
   }



namespace mlc
{

  namespace internal
  {

    struct valist_none;

    template < mlc_internal_valist_decl_params_,
	       unsigned i >
    struct valist_elt_;

    template <typename list> struct valist_size_;

  } // end of namespace mlc::internal


  /*! \class mlc::valist_<E1,..>
  **
  ** This class is FIXME  */

  template < typename E1 = internal::valist_none,
	     typename E2 = internal::valist_none,
	     typename E3 = internal::valist_none,
	     typename E4 = internal::valist_none,
	     typename E5 = internal::valist_none,
	     typename E6 = internal::valist_none,
	     typename E7 = internal::valist_none,
	     typename E8 = internal::valist_none,
	     typename E9 = internal::valist_none >
  struct valist_ : public mlc::abstract::type
  {
    static const unsigned size_value = internal::valist_size_<
      valist_<mlc_internal_valist_params_> >::value;

    template <unsigned i>
    struct elt : private ensure_list_< uint_greater_or_equal_<i, 1>,
				       uint_less_or_equal_<i, size_value> >,
		 public internal::valist_elt_<mlc_internal_valist_params_, i>
    {
    };
  };



  namespace internal
  {

    mlc_internal_valist_elt_spe(1);
    mlc_internal_valist_elt_spe(2);
    mlc_internal_valist_elt_spe(3);
    mlc_internal_valist_elt_spe(4);
    mlc_internal_valist_elt_spe(5);
    mlc_internal_valist_elt_spe(6);
    mlc_internal_valist_elt_spe(7);
    mlc_internal_valist_elt_spe(8);
    mlc_internal_valist_elt_spe(9);


    template <>
    struct valist_size_ < valist_<> >
    { enum { value = 0 }; };

    template <class E1>
    struct valist_size_ < valist_<E1> >
    { enum { value = 1 }; };

    template <class E1, class E2>
    struct valist_size_ < valist_<E1,E2> >
    { enum { value = 2 }; };

    template <class E1, class E2, class E3>
    struct valist_size_ < valist_<E1,E2,E3> >
    { enum { value = 3 }; };

    template <class E1, class E2, class E3, class E4>
    struct valist_size_ < valist_<E1,E2,E3,E4> >
    { enum { value = 4 }; };

    template <class E1, class E2, class E3, class E4,
	      class E5>
    struct valist_size_ < valist_<E1,E2,E3,E4,E5> >
    { enum { value = 5 }; };

    template <class E1, class E2, class E3, class E4,
	      class E5, class E6>
    struct valist_size_ < valist_<E1,E2,E3,E4,E5,E6> >
    { enum { value = 6 }; };

    template <class E1, class E2, class E3, class E4,
	      class E5, class E6, class E7>
    struct valist_size_ < valist_<E1,E2,E3,E4,E5,E6,E7> >
    { enum { value = 7 }; };

    template <class E1, class E2, class E3, class E4,
	      class E5, class E6, class E7, class E8>
    struct valist_size_ < valist_<E1,E2,E3,E4,E5,E6,E7,E8> >
    { enum { value = 8 }; };

    template <class E1, class E2, class E3, class E4,
	      class E5, class E6, class E7, class E8,
	      class E9>
    struct valist_size_ < valist_<E1,E2,E3,E4,E5,E6,E7,E8,E9> >
    { enum { value = 9 }; };



  } // end of namespace mlc::internal
  
  

} // end of namespace mlc


# include <mlc/elt.hh>



#endif // ! METALIC_VALIST_HH
