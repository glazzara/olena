// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_DEBUG_TYPENAME_HH
# define OLN_DEBUG_TYPENAME_HH

# include <string>
# include <sstream>

# include <oln/core/fwd_decls.hh>
# include <oln/core/aliases.hh>
# include <oln/morpher/fwd_decls.hh>


# define oln_decl_typename_string(Type)		\
						\
  template <>					\
  struct typename_string <Type>			\
  {						\
    static std::string get()			\
    {						\
      return #Type;				\
    }						\
  }						\


# define oln_decl_typename_string_T(Type)				\
									\
  template <typename T>							\
  struct typename_string < Type<T> >					\
  {									\
    static std::string get()						\
    {									\
      std::ostringstream oss;						\
      oss << (#Type) << '<' << typename_string<T>::get() << '>';	\
      return oss.str();							\
    }									\
  }


# define oln_decl_typename_string_U(Type)	\
						\
  template <unsigned U>				\
  struct typename_string < Type<U> >		\
  {						\
    static std::string get()			\
    {						\
      std::ostringstream oss;			\
      oss << (#Type) << '<' << U << '>';	\
      return oss.str();				\
    }						\
  }


# define oln_decl_typename_string_TT(Type)				\
									\
  template <typename T1, typename T2>					\
  struct typename_string < Type<T1, T2> >				\
  {									\
    static std::string get()						\
    {									\
      std::ostringstream oss;						\
      oss << (#Type) << '<' << typename_string<T1>::get() << ','	\
	  << typename_string<T2>::get() << '>';				\
      return oss.str();							\
    }									\
  }



namespace oln
{

  namespace debug
  {

    namespace internal
    {

      template <typename T>
      struct typename_string
      {
	static std::string get()
	{
	  return "?";
	}
      };

      template <typename T>
      struct typename_string <const T>
      {
	static std::string get()
	{
	  return std::string("const ") + typename_string<T>::get();
	}
      };

      template <typename T>
      struct typename_string <T*>
      {
	static std::string get()
	{
	  return typename_string<T>::get() + '*';
	}
      };

    } // end of namespace oln::debug::internal


    template <typename T>
    struct type
    {
      static std::string to_string()
      {
	return internal::typename_string<T>::get();
      }
    };


    namespace internal
    {

      // Builtin.

      oln_decl_typename_string(void);
      oln_decl_typename_string(bool);

      oln_decl_typename_string(char);

      oln_decl_typename_string(unsigned char);
      oln_decl_typename_string(signed char);

      oln_decl_typename_string(unsigned short);
      oln_decl_typename_string(signed short);

      oln_decl_typename_string(unsigned long);
      oln_decl_typename_string(signed long);

      oln_decl_typename_string(int);
      oln_decl_typename_string(unsigned int);
      oln_decl_typename_string(float);
      oln_decl_typename_string(double);


      // From oln/core/1d/fwd_decls.hh

      oln_decl_typename_string_TT(oln::array1d);
      oln_decl_typename_string_T(oln::dpoint1d_);
      oln_decl_typename_string_T(oln::point1d_);
      oln_decl_typename_string_T(oln::image1d);

      oln_decl_typename_string(oln::grid1d);
      oln_decl_typename_string(oln::point1d);
      oln_decl_typename_string(oln::dpoint1d);
      oln_decl_typename_string(oln::neighb1d);
      oln_decl_typename_string(oln::window1d);
      oln_decl_typename_string(oln::bbox1d);
      oln_decl_typename_string(oln::topo1d);
      oln_decl_typename_string(oln::fwd_piter1d);
      oln_decl_typename_string(oln::bkd_piter1d);
      oln_decl_typename_string(oln::fwd_qiter1d);
      oln_decl_typename_string(oln::bkd_qiter1d);
      oln_decl_typename_string(oln::point1df);
      oln_decl_typename_string(oln::dpoint1df);


      // From oln/core/2d/fwd_decls.hh

      oln_decl_typename_string_TT(oln::array2d);
      oln_decl_typename_string_T(oln::dpoint2d_);
      oln_decl_typename_string_T(oln::point2d_);
      oln_decl_typename_string_T(oln::image2d);

      oln_decl_typename_string(oln::grid2d);
      oln_decl_typename_string(oln::point2d);
      oln_decl_typename_string(oln::dpoint2d);
      oln_decl_typename_string(oln::neighb2d);
      oln_decl_typename_string(oln::window2d);
      oln_decl_typename_string(oln::bbox2d);
      oln_decl_typename_string(oln::topo2d);
      oln_decl_typename_string(oln::fwd_piter2d);
      oln_decl_typename_string(oln::bkd_piter2d);
      oln_decl_typename_string(oln::fwd_qiter2d);
      oln_decl_typename_string(oln::bkd_qiter2d);
      oln_decl_typename_string(oln::point2df);
      oln_decl_typename_string(oln::dpoint2df);


      // From oln/core/3d/fwd_decls.hh

      oln_decl_typename_string_TT(oln::array3d);
      oln_decl_typename_string_T(oln::dpoint3d_);
      oln_decl_typename_string_T(oln::point3d_);
      oln_decl_typename_string_T(oln::image3d);

      oln_decl_typename_string(oln::grid3d);
      oln_decl_typename_string(oln::point3d);
      oln_decl_typename_string(oln::dpoint3d);
      oln_decl_typename_string(oln::neighb3d);
      oln_decl_typename_string(oln::window3d);
      oln_decl_typename_string(oln::bbox3d);
      oln_decl_typename_string(oln::topo3d);
      oln_decl_typename_string(oln::fwd_piter3d);
      oln_decl_typename_string(oln::bkd_piter3d);
      oln_decl_typename_string(oln::fwd_qiter3d);
      oln_decl_typename_string(oln::bkd_qiter3d);
      oln_decl_typename_string(oln::point3df);
      oln_decl_typename_string(oln::dpoint3df);


      // From oln/morpher/fwd_decl.hh

      oln_decl_typename_string_T(oln::morpher::identity);

      oln_decl_typename_string_TT(oln::morpher::add_neighborhood);
      oln_decl_typename_string_TT(oln::morpher::add_isubset);
      oln_decl_typename_string_TT(oln::morpher::thru_fun);
      oln_decl_typename_string_TT(oln::morpher::value_cast);


      // From oln/core/gen/fwd_decls.hh

      oln_decl_typename_string_U(oln::grid_);

      oln_decl_typename_string_T(oln::bbox_);
      oln_decl_typename_string_T(oln::bbox_fwd_piter_);
      oln_decl_typename_string_T(oln::bbox_bkd_piter_);

      oln_decl_typename_string_T(oln::fwd_piter_bbox_);
      oln_decl_typename_string_T(oln::bkd_piter_bbox_);

      oln_decl_typename_string_T(oln::window_);
      oln_decl_typename_string_T(oln::fwd_qiter_win_);
      oln_decl_typename_string_T(oln::bkd_qiter_win_);

      oln_decl_typename_string_T(oln::neighb_);
      oln_decl_typename_string_T(oln::fwd_niter_neighb_);
      oln_decl_typename_string_T(oln::bkd_niter_neighb_);

      oln_decl_typename_string_T(oln::topo_bbox_);
      oln_decl_typename_string_T(oln::topo_lbbox_);
      oln_decl_typename_string_TT(oln::topo_add_isubset);
      oln_decl_typename_string_TT(oln::topo_add_nbh);

      oln_decl_typename_string_TT(oln::mapimage);


    } // end of namespace oln::debug::internal

  } // end of namespace oln::debug

} // end of namespace oln


#endif // ! OLN_DEBUG_TYPENAME_HH
