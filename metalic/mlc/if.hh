// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef METALIC_IF_HH
# define METALIC_IF_HH

# include <mlc/bexpr.hh>
# include <mlc/assert.hh>
# include <mlc/is_a.hh>


/** \def mlc_if(Cond, Then, Else)
 ** \brief Static `if' on types.
 **
 ** If \a Cond is true, evaluate to type \a Then, otherwise evaluate to
 ** type \a Else.
 **
 ** \note \a Then and \a Else must be valid types, since they are both
 ** evaluated, whatever the result of \a Cond.
 **
 */
# define mlc_if(Cond, Then, Else)  typename mlc::if_<Cond, Then, Else>::ret
/// Likewise, without the leading \c typename keyword.
# define mlc_if_(Cond, Then, Else)          mlc::if_<Cond, Then, Else>::ret


namespace mlc
{

  namespace ERROR
  {
    struct FIXME;

  } // end of mlc::ERROR

  namespace internal
  {

    /// \brief Declare a static `if'.
    /// \note Internal class, don't use it directly.
    template <bool b, typename then_type, typename else_type>
    struct if_;

    /// \brief Specialization of mlc::internal::if_ when condition is true.
    /// \note Internal class, don't use it directly.
    template <typename then_type, typename else_type>
    struct if_ <true, then_type, else_type>
    {
      typedef then_type ret;
    };
    
    /// \brief Specialization of mlc::internal::if_ when condition is false.
    /// \note Internal class, don't use it directly.
    template <typename then_type, typename else_type>
    struct if_ <false, then_type, else_type>
    {
      typedef else_type ret;
    };
    
  } // end of namespace mlc::internal


/** \brief Static `if' on types.
 **
 ** If \a Cond is true, evaluate to type \a Then, otherwise evaluate to
 ** type \a Else.  The \a ret member holds the result.
 **
 ** \note \a Then and \a Else must be valid types, since they are both
 ** evaluated, whatever the result of \a Cond.
 */
  template <typename cond_type, typename then_type, typename else_type>
  struct if_ :
    // FIXME: enable the static assertion below!!!
    private assert_< mlc_is_a(cond_type, mlc::abstract::bexpr),
		     mlc::ERROR::FIXME >,
    public internal::if_ < mlc_bool(cond_type), then_type, else_type >
  {
  };

} // end of namespace mlc


#endif // ! METALIC_IF_HH
