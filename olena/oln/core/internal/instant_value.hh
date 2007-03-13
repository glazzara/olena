// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_INSTANT_VALUE_HH
# define OLN_CORE_INTERNAL_INSTANT_VALUE_HH


# define oln_decl_instant_value(Name)				\
								\
template <typename V>						\
struct Name##_t : public internal::instant_value_< Name##_t, V>	\
{								\
  Name##_t(const V& v) { this->value = v; }			\
};								\
								\
template <typename V>						\
Name##_t<V> Name(const V& v)					\
{								\
  return Name##_t<V>(v);					\
}								\
								\
struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



namespace oln
{

  namespace internal
  {

    /// Class internal::instant_value_<M,V>.

    template <template<class> class M, typename V>
    struct instant_value_
    {
      V value;

      template <typename W>
      operator M<W>() const;
    };


# ifndef OLN_INCLUDE_ONLY

    template <template<class> class M, typename V>
    template <typename W>
    instant_value_<M,V>::operator M<W>() const
    {
      M<W> tmp(this->value);
      return tmp;
    }

# endif // OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_INSTANT_VALUE_HH
