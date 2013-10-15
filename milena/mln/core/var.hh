// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE).
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

#ifndef MLN_CORE_VAR_HH
# define MLN_CORE_VAR_HH

/// \file
/// \brief Various macros (syntactic sugar) to shorten declarations of
/// variables.
///
/// \warning Theses macro rely on an extension of g++ (\c typeof) so it
/// is NOT standard C++.

# include <mln/core/macros.hh>

/// Definition of the macro mln_VAR.
/// Deduce the type of Expr, create a variable of that type and assign the
/// result to it.
#define mln_VAR(Var, Expr)			\
						\
  typedef typeof(Expr) Var##_t;                 \
  Var##_t Var = Expr

#define mln_const_VAR(Var, Expr)		\
						\
  typedef const typeof(Expr) Var##_t;           \
  Var##_t Var = Expr


// mln_PITER

/// Shortcuts to declare site iterators
/// \{
#define mln_PITER(p, obj)			\
 typedef typeof(obj) p##__obj_type;		\
 mln_piter(p##__obj_type) p(obj)

#define mln_PITER_(p, obj)			\
 typedef typeof(obj) p##__obj_type;		\
 mln_piter_(p##__obj_type) p(obj)
/// \}

/// Shortcuts to declare site forward iterators
/// \{
#define mln_FWD_PITER(p, obj)			\
 typedef typeof(obj) p##__obj_type;		\
 mln_fwd_piter(p##__obj_type) p(obj)

#define mln_FWD_PITER_(p, obj)			\
 typedef typeof(obj) p##__obj_type;		\
 mln_fwd_piter_(p##__obj_type) p(obj)
/// \}

/// Shortcuts to declare site backward iterators
/// \{
#define mln_BKD_PITER(p, obj)			\
 typedef typeof(obj) p##__obj_type;		\
 mln_bkd_piter(p##__obj_type) p(obj)

#define mln_BKD_PITER_(p, obj)			\
 typedef typeof(obj) p##__obj_type;		\
 mln_bkd_piter_(p##__obj_type) p(obj)
/// \}


// mln_NITER

/// Shortcuts to declare neighbor iterators
/// \{
#define mln_NITER(n, obj, p)			\
 typedef typeof(obj) n##__obj_type;		\
 mln_niter(n##__obj_type) n(obj, p)

#define mln_NITER_(n, obj, p)			\
 typedef typeof(obj) n##__obj_type;		\
 mln_niter_(n##__obj_type) n(obj, p)
/// \}

/// Shortcuts to declare neighbor forward iterators
/// \{
#define mln_FWD_NITER(n, obj, p)		\
 typedef typeof(obj) n##__obj_type;		\
 mln_fwd_niter(n##__obj_type) n(obj, p)

#define mln_FWD_NITER_(n, obj, p)		\
 typedef typeof(obj) n##__obj_type;		\
 mln_fwd_niter_(n##__obj_type) n(obj, p)
/// \}

/// Shortcuts to declare neighbor backward iterators
/// \{
#define mln_BKD_NITER(n, obj, p)		\
 typedef typeof(obj) n##__obj_type;		\
 mln_bkd_niter(n##__obj_type) n(obj, p)

#define mln_BKD_NITER_(n, obj, p)		\
 typedef typeof(obj) n##__obj_type;		\
 mln_bkd_niter_(n##__obj_type) n(obj, p)
/// \}


// mln_QITER

/// Shortcuts to declare neighbor iterators on windows
/// \{
#define mln_QITER(q, obj, p)			\
 typedef typeof(obj) q##__obj_type;		\
 mln_qiter(q##__obj_type) q(obj, p)

#define mln_QITER_(q, obj, p)			\
 typedef typeof(obj) q##__obj_type;		\
 mln_qiter_(q##__obj_type) q(obj, p)
/// \}

/// Shortcuts to declare neighbor forward iterators on windows
/// \{
#define mln_FWD_QITER(q, obj, p)		\
 typedef typeof(obj) q##__obj_type;		\
 mln_fwd_qiter(q##__obj_type) q(obj, p)

#define mln_FWD_QITER_(q, obj, p)		\
 typedef typeof(obj) q##__obj_type;		\
 mln_fwd_qiter_(q##__obj_type) q(obj, p)
/// \}

/// Shortcuts to declare neighbor backward iterators on windows
/// \{
#define mln_BKD_QITER(q, obj, p)		\
 typedef typeof(obj) q##__obj_type;		\
 mln_bkd_qiter(q##__obj_type) q(obj, p)

#define mln_BKD_QITER_(q, obj, p)		\
 typedef typeof(obj) q##__obj_type;		\
 mln_bkd_qiter_(q##__obj_type) q(obj, p)
/// \}


// mln_EITER

/// Shortcuts to declare element iterators
/// \{
#define mln_EITER(e, obj)			\
 typedef typeof(obj) e##__obj_type;		\
 mln_eiter(e##__obj_type) e(obj)

#define mln_EITER_(e, obj)			\
 typedef typeof(obj) e##__obj_type;		\
 mln_eiter_(e##__obj_type) e(obj)
/// \}

/// Shortcuts to declare element forward iterators
/// \{
#define mln_FWD_EITER(e, obj)			\
 typedef typeof(obj) e##__obj_type;		\
 mln_fwd_eiter(e##__obj_type) e(obj)

#define mln_FWD_EITER_(e, obj)			\
 typedef typeof(obj) e##__obj_type;		\
 mln_fwd_eiter_(e##__obj_type) e(obj)
/// \}

/// Shortcuts to declare element backward iterators
/// \{
#define mln_BKD_EITER(e, obj)			\
 typedef typeof(obj) e##__obj_type;		\
 mln_bkd_eiter(e##__obj_type) e(obj)

#define mln_BKD_EITER_(e, obj)			\
 typedef typeof(obj) e##__obj_type;		\
 mln_bkd_eiter_(e##__obj_type) e(obj)
/// \}


// mln_VITER

/// Shortcuts to declare value iterators
/// \{
#define mln_VITER(v, obj)			\
 typedef typeof(obj) v##__obj_type;		\
 mln_viter(v##__obj_type) v(obj)

#define mln_VITER_(v, obj)			\
 typedef typeof(obj) v##__obj_type;		\
 mln_viter_(v##__obj_type) v(obj)
/// \}

/// Shortcuts to declare value forward iterators
/// \{
#define mln_FWD_VITER(v, obj)			\
 typedef typeof(obj) v##__obj_type;		\
 mln_fwd_viter(v##__obj_type) v(obj)

#define mln_FWD_VITER_(v, obj)			\
 typedef typeof(obj) v##__obj_type;		\
 mln_fwd_viter_(v##__obj_type) v(obj)
/// \}

/// Shortcuts to declare value backward iterators
/// \{
#define mln_BKD_VITER(v, obj)			\
 typedef typeof(obj) v##__obj_type;		\
 mln_bkd_viter(v##__obj_type) v(obj)

#define mln_BKD_VITER_(v, obj)			\
 typedef typeof(obj) v##__obj_type;		\
 mln_bkd_viter_(v##__obj_type) v(obj)
/// \}


#endif // ! MLN_CORE_VAR_HH
