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

#ifndef MLN_CORE_MACROS_HH
# define MLN_CORE_MACROS_HH

/*! \file mln/core/macros.hh
 *
 * \brief Definition of the set of milena macros.
 */


// b

/// Shortcut to access the bkd_niter type associated to T.
# define mln_bkd_niter(T)  typename T::bkd_niter

/// Shortcut to access the bkd_piter type associated to T.
# define mln_bkd_piter(T)  typename T::bkd_piter

/// Shortcut to access the bkd_qiter type associated to T.
# define mln_bkd_qiter(T)  typename T::bkd_qiter

/// Shortcut to access the bkd_viter type associated to T.
# define mln_bkd_viter(T)  typename T::bkd_viter

/// Shortcut to access the box type associated to T.
# define mln_box(T)        typename T::box


// c

/// Shortcut to access the coord type associated to T.
# define mln_coord(T)      typename T::coord


// d

/// Shortcut to access the dpoint type associated to T.
# define mln_dpoint(T)     typename T::dpoint


// e

/// Shortcut to access the encoding type associated to T.
# define mln_enc(T)        typename T::enc

/// Shortcut to access the equivalent type associated to T.
# define mln_equiv(T)      typename T::equiv


// f

/// Shortcut to access the fwd_niter type associated to T.
# define mln_fwd_niter(T)  typename T::fwd_niter

/// Shortcut to access the fwd_piter type associated to T.
# define mln_fwd_piter(T)  typename T::fwd_piter

/// Shortcut to access the bkd_piter type associated to T.
# define mln_bkd_piter(T)  typename T::bkd_piter

/// Shortcut to access the fwd_qiter type associated to T.
# define mln_fwd_qiter(T)  typename T::fwd_qiter

/// Shortcut to access the fwd_viter type associated to T.
# define mln_fwd_viter(T)  typename T::fwd_viter

// i

/// Shortcut to access the image type associated to T.
# define mln_image(T)      typename T::image


// l


/// Shortcut to access the lvalue type associated to T.
# define mln_lvalue(T)     typename T::lvalue


// p

/// Shortcut to access the type of point iterator (piter) associated to T.
# define mln_piter(T)      typename T::fwd_piter
# define mln_piter_(T)              T::fwd_piter

/// Shortcut to access the type of point set (pset) associated to T.
# define mln_pset(T)       typename T::pset

/// Shortcut to access the type of point site (psite) associated to T.
# define mln_psite(T)      typename T::psite

/// Shortcut to access the point type associated to T.
# define mln_point(T)      typename T::point


// q

/// Shortcut to access the qualified (const or mutable) value type associated to T.
# define mln_qlf_value(T)      typename mln::trait::qlf_value< T >::ret

/// Shortcut to access the qiter type associated to T.
# define mln_qiter(T)      typename T::fwd_qiter


// n

/// Shortcut to access the niter type associated to T.
# define mln_niter(T)      typename T::fwd_niter


// r

/// Shortcut to access the result type associated to T.
# define mln_result(T)     typename T::result

/// Shortcut to access the rvalue type associated to T.
# define mln_rvalue(T)     typename T::rvalue


// v

/// Shortcut to access the value type associated to T.
# define mln_value(T)      typename T::value

/// Shortcut to the kind of values for an image with type \c I.
# define mln_value_kind(T) typename mln::value::props< mln_value(I) >::kind

/// Shortcut to access the type of value set (vset) associated to T.
# define mln_vset(T)       typename T::vset

/// Shortcut to access the viter type associated to T.
# define mln_viter(T)      typename T::fwd_viter





/*! \brief Shortcut to change the value type of an image type.
 *
 * \c I is an image type
 * \c T is the new value type
 */
# define mln_ch_value(I, T) typename I ::template change_value< T >::ret



// FIXME: Doc!

# define mln_fwd_pixter(I)  typename mln::trait::fwd_pixter< I >::ret
# define mln_fwd_pixter_(I)          mln::trait::fwd_pixter< I >::ret

# define mln_bkd_pixter(I) typename mln::trait::bkd_pixter< I >::ret
# define mln_pixter(I)  mln_fwd_pixter(I)
# define mln_pixter_(I) mln_fwd_pixter_(I)


# define mln_fwd_qixter(I, W) typename mln::trait::fwd_qixter< I, W >::ret
# define mln_bkd_qixter(I, W) typename mln::trait::bkd_qixter< I, W >::ret
# define mln_qixter(I, W) mln_fwd_qixter(I, W)


# define mln_fwd_nixter(I, N) typename mln::trait::fwd_nixter< I, N >::ret
# define mln_bkd_nixter(I, N) typename mln::trait::bkd_nixter< I, N >::ret
# define mln_nixter(I, N) mln_fwd_nixter(I, N)



#endif // ! MLN_CORE_MACROS_HH
