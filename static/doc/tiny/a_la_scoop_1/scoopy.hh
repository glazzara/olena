// Copyright (C) 2007 EPITA Research and Development Laboratory.
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


/* \file doc/tiny/a_la_scoop_1/scoopy.hh

   \brief Mini-equipment for SCOOP 1. */

#ifndef STC_DOC_TINY_A_LA_SCOOP_1_SCOOPY_HH
# define STC_DOC_TINY_A_LA_SCOOP_1_SCOOPY_HH


namespace stc
{

  class abstract;

  template <typename Exact>
  class any
  {
  public:
    Exact& exact() { return *(Exact*)(void*)this; }
    const Exact& exact() const  { return *(const Exact*)(const void*)this; }
  };
  
}


# define stc_typename(Type) typedef typename vtypes<Exact>::Type Type
# define stc_using(Type)    typedef typename super::Type Type


# define stc_equip_namespace(Namespace)		\
						\
namespace Namespace				\
{						\
  template <typename T> struct vtypes;		\
						\
  /* Any. */					\
						\
  template <typename Exact> class Any;		\
						\
  template <typename Exact>			\
  struct vtypes< Any<Exact> >			\
  {						\
    typedef Exact exact;			\
  };						\
						\
  template <typename Exact>			\
  class Any : public stc::any<Exact>		\
  {						\
  };						\
						\
}						\
						\
struct end_with_semicolumn


#endif // ! STC_DOC_TINY_A_LA_SCOOP_1_SCOOPY_HH
