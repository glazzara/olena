// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_SUP_HH
# define MLN_ACCU_SUP_HH

/// \file mln/accu/sup.hh
///
/// Define an accumulator that computes a sup.

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/trait/value_.hh>
# include <mln/util/pix.hh>
# include <mln/fun/math/sup.hh>

namespace mln
{

  namespace accu
  {


    /// Generic sup accumulator class.
    /*!
    * The parameter \c T is the type of values.
    */
    template <typename T>
    struct sup : public mln::accu::internal::base< const T&, sup<T> >
    {
      typedef T argument;

      sup();

      /// Manipulators.
      /// \{
      void init();
      void take_as_init(const argument& t);
      void take(const argument& t);
      void take(const sup<T>& other);
      /// \}

      /// Get the value of the accumulator.
      const T& to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      T t_;
      typename mln::fun::sup::with<T, T>::ret fun_;
    };


    template <typename I> struct sup< util::pix<I> >;



    namespace meta
    {

      /// Meta accumulator for sup.

      struct sup : public Meta_Accumulator< sup >
      {
	template <typename T>
	struct with
	{
	  typedef accu::sup<T> ret;
	};
      };

    } // end of namespace mln::accu::meta



    # ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    sup<T>::sup()
    {
      init();
    }

    template <typename T>
    inline
    void
    sup<T>::init()
    {
      t_ = mln_min(T);
    }

    template <typename T>
    inline
    void sup<T>::take_as_init(const argument& t)
    {
      t_ = t;
    }

    template <typename T>
    inline
    void sup<T>::take(const argument& t)
    {
      this->t_ = this->fun_(t_, t);
    }

    template <typename T>
    inline
    void
    sup<T>::take(const sup<T>& other)
    {
      this->t_ = this->fun_(t_, other.t_);
    }

    template <typename T>
    inline
    const T&
    sup<T>::to_result() const
    {
      return t_;
    }

    template <typename T>
    inline
    bool
    sup<T>::is_valid() const
    {
      return true;
    }

    # endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_SUP_HH
