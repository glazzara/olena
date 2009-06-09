// Copyright (C) 2006, 2007, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_VALUE_SCALAR_HH
# define MLN_VALUE_SCALAR_HH
 
/// \file
///
/// \brief Definition of the basic mln::value::scalar_ class.

# include <mln/value/concept/scalar.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/if.hh>


namespace mln
{

  namespace value
  {

    template <typename T>
    class scalar_ : public Scalar< scalar_<T> >
    {
    public:

      /// Encoding associated type.
      typedef T enc; // FIXME

      /// Equivalent associated type.
      typedef T equiv; // FIXME


      /// Ctor.
      scalar_();

      /// Ctor.
      explicit scalar_(const T& val);

      /// Access to the scalar value.
      T to_equiv() const;

    protected:
      T val_; // FIXME: const&?
    };

    template <typename T> class scalar_< scalar_<T> >; // Safety: this type should not exist!


    /// Print a scalar \p s in an output stream \p ostr.
    template <typename T>
    std::ostream& operator<<(std::ostream& ostr, const scalar_<T>& s);



    // Routine scalar(T) -> scalar_<T>.

    namespace internal
    {

      template <typename T>
      struct helper_scalar_
      {
	typedef mlc_is_a(T, Scalar) is_scalar_;
	typedef mlc_if(is_scalar_, T, scalar_<T>) ret;
      };

    } // end of namespace mln::value::internal


    // Routine.
    template <typename T>
    typename internal::helper_scalar_<T>::ret
    scalar(const T& s);  // FIXME: if T already is a scalar_...




# ifndef MLN_INCLUDE_ONLY

    // scalar_<T>.

    template <typename T>
    inline
    scalar_<T>::scalar_()
    {
    }

    template <typename T>
    inline
    scalar_<T>::scalar_(const T& val)
      : val_(val)
    {
    }

    template <typename T>
    inline
    T
    scalar_<T>::to_equiv() const
    {
      return val_;
    }

    // Operator.

    template <typename T>
    inline
    std::ostream& operator<<(std::ostream& ostr, const scalar_<T>& s)
    {
      return ostr << s.to_equiv();
    }

    // Routine.

    template <typename T>
    inline
    typename internal::helper_scalar_<T>::ret
    scalar(const T& s)
    {
      typename internal::helper_scalar_<T>::ret tmp(s);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_SCALAR_HH
