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

#ifndef OLN_CORE_MACROS_HH
# define OLN_CORE_MACROS_HH

// --------------------- //
// Virtual type access.  //
// --------------------- //

/// \def oln_type_of(OlnType, Alias)
///
/// Macro to retrieve an associated type \a Alias from the exact type of
/// an oln type \a OlnType whose category is not specified (version to be
/// used inside a template).
# define oln_type_of(OlnType, Alias)		\
   stc_direct_type_of(oln, void, OlnType, Alias)
// FIXME: Remove direct_ above.


/// FIXME: Temporary macro to be able to perform "oln_type_of(oln_type_of(A, B), C)".
# define oln_deduce_type_of(OlnType, Alias1, Alias2)							\
typename oln::direct_type_of_<void,										\
                       typename oln::direct_type_of_<void, OlnType, oln::typedef_::Alias1##_type >::ret,	\
                       oln::typedef_::Alias2##_type >::ret
// FIXME: Remove direct_ *twice* above.


# define oln_deduce_direct_type_of(OlnType, Alias1, Alias2)							\
typename oln::direct_type_of_<void,										\
                       typename oln::direct_type_of_<void, OlnType, oln::typedef_::Alias1##_type >::ret,	\
                       oln::typedef_::Alias2##_type >::ret


/// \def oln_type_of(OlnType, Alias)
///
/// Macro to retrieve an associated type \a Alias from the exact type of
/// an oln type \a OlnType whose category is not specified (version to be
/// used outside a template).
# define oln_type_of_(OlnType, Alias)		\
   stc_direct_type_of_(oln, void, OlnType, Alias)
// FIXME: Remove direct_ above.


/// \def oln_direct_type_of(OlnType, Alias)
///
/// Macro to retrieve an associated type \a Alias from an oln type \a
/// OlnType directly, and whose category is not specified (version to
/// be used inside a template).
# define oln_direct_type_of(OlnType, Alias)		\
   stc_direct_type_of(oln, void, OlnType, Alias)

/// \def oln_direct_type_of_(OlnType, Alias)
///
/// Macro to retrieve an associated type \a Alias from an oln type \a
/// OlnType directly, and whose category is not specified (version to
/// be used inside a template).
# define oln_direct_type_of_(OlnType, Alias)		\
   stc_direct_type_of_(oln, void, OlnType, Alias)


// ------------------------------- //
// Virtual type check and access.  //
// ------------------------------- //

/* These macros are the same as the previous ones, but they use
   stc_check_type_of{_,} (resp. stc_check_direct_type_of{_,}) instead of
   stc_type_of{_,} (resp. stc_direct_type_of{_,}).  */

/// \def oln_check_type_of(OlnType, Alias)
///
/// Macro to retrieve an associated type \a Alias from the exact type of
/// an oln type \a OlnType whose category is not specified (version to be
/// used inside a template).
///
/// If the virtual type is not found, raise a static error.
# define oln_check_type_of(OlnType, Alias)	\
   stc_check_direct_type_of(oln, void, OlnType, Alias)
// FIXME: Remove direct_ above.

/// \def oln_check_type_of(OlnType, Alias)
///
/// Macro to retrieve an associated type \a Alias from the exact type of
/// an oln type \a OlnType whose category is not specified (version to be
/// used outside a template).
///
/// If the virtual type is not found, raise a static error.
# define oln_check_type_of_(OlnType, Alias)		\
   stc_check_direct_type_of_(oln, void, OlnType, Alias)
// FIXME: Remove direct_ above.


/// \def oln_check_direct_type_of(OlnType, Alias)
///
/// Macro to retrieve an associated type \a Alias from an oln type \a
/// OlnType directly, and whose category is not specified (version to
/// be used inside a template).
///
/// If the virtual type is not found, raise a static error.
# define oln_check_direct_type_of(OlnType, Alias)	\
   stc_check_direct_type_of(oln, void, OlnType, Alias)

/// \def oln_check_direct_type_of_(OlnType, Alias)
///
/// Macro to retrieve an associated type \a Alias from an oln type \a
/// OlnType directly, and whose category is not specified (version to
/// be used inside a template).
///
/// If the virtual type is not found, raise a static error.
# define oln_check_direct_type_of_(OlnType, Alias)	\
   stc_check_direct_type_of_(oln, void, OlnType, Alias)


#endif // ! OLN_CORE_MACROS_HH
