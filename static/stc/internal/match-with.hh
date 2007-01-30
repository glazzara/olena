// Copyright (C) 2005, 2006, 2007 EPITA Research and Development Laboratory
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


/* \file stc/match-with.hh
   \brief Internal helpers for stc/scoop-alt.hh.

   This file contains helpers that mimic a ``match-with'' (pattern
   matching) construct from many functional languages.  */


#ifndef STC_INTERNAL_MATCH_WITH_HH
# define STC_INTERNAL_MATCH_WITH_HH

/*-----------------.
| merge2 helpers.  |
`-----------------*/

/* Shortcuts macros.  */
#define stc_internal_match_pair_0p_with(Local, Super, Res)	\
  template <>							\
  struct match_with< Local, Super >				\
  {								\
    typedef Res ret;						\
  }

#define stc_internal_match_pair_1p_with(T1, Local, Super, Res)	\
  template < typename T1 >					\
  struct match_with< Local, Super >				\
  {								\
    typedef Res ret;						\
  }

#define stc_internal_match_pair_2p_with(T1, T2, Local, Super, Res)	\
  template < typename T1, typename T2 >					\
  struct match_with< Local, Super >					\
  {									\
    typedef Res ret;							\
  }

/* Erroneous cases.  */
#define stc_internal_match_pair_1p_with_error(T1, Local, Super, Msg)	\
  template < typename T1 >						\
  struct match_with< Local, Super > :					\
    mlc::abort_< T1, Msg >						\
  {									\
  }

#define stc_internal_match_pair_2p_with_error(T1, T2, Local, Super, Msg) \
  template < typename T1, typename T2 >					 \
  struct match_with< Local, Super > :					 \
    mlc::abort_< T2, Msg >						 \
  {									 \
  }

/*-----------------.
| merge3 helpers.  |
`-----------------*/

/* Shortcuts macros.  */
#define stc_internal_match_triple_0p_with(Local, Super, Deleg, Res)	\
  template <>								\
  struct match_with< Local, Super, Deleg >				\
  {									\
    typedef Res ret;							\
  }

#define stc_internal_match_triple_1p_with(T1, Local, Super, Deleg, Res)	\
  template < typename T1 >						\
  struct match_with< Local, Super, Deleg >				\
  {									\
    typedef Res ret;							\
  }

#define stc_internal_match_triple_2p_with(T1, T2, Local, Super, Deleg, Res) \
  template < typename T1, typename T2 >					    \
  struct match_with< Local, Super, Deleg >				    \
  {									    \
    typedef Res ret;							    \
  }

#define stc_internal_match_triple_3p_with(T1, T2, T3, Local, Super, Deleg, Res)	\
  template < typename T1, typename T2, typename T3 >				\
  struct match_with< Local, Super, Deleg >					\
  {										\
    typedef Res ret;								\
  }

/* Erroneous cases.  */
#define stc_internal_match_triple_2p_with_error(T1, T2, Local, Super, Deleg, Msg) \
  template < typename T1, typename T2 >						  \
  struct match_with< Local, Super, Deleg > :					  \
      mlc::abort_< T2, Msg >							  \
  {										  \
  }

#define stc_internal_match_triple_3p_with_error(T1, T2, T3, Local, Super, Deleg, Msg) \
  template < typename T1, typename T2, typename T3 >				      \
  struct match_with< Local, Super, Deleg > :					      \
      mlc::abort_< T3, Msg >							      \
  {										      \
  }

#endif // ! STC_INTERNAL_MATCH_WITH_HH
