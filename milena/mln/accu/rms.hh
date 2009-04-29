// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_RMS_HH
# define MLN_ACCU_RMS_HH

/// \file mln/accu/rms.hh
///
/// Define an accumulator that computes the Root Mean Square.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>
# include <mln/math/sqrt.hh>


namespace mln
{

  namespace accu
  {


    /// \brief Generic root mean square accumulator class.
    ///
    /// The parameter \c T is the type of the root mean square value.
    ///
    /// \ingroup modaccuvalues
    //
    template <typename T, typename V>
    struct rms : public mln::accu::internal::base<V, rms<T,V> >
    {
      typedef T argument;

      rms();

      /// Manipulators.
      /// \{
      void init();
      void take_as_init_(const T& p);
      void take(const T& p);
      void take(const rms<T,V>& other);
      /// \}

      /// Get the value of the accumulator.
      V to_result() const;

      /// Return the underlying value used to compute the result.
      /// v = sum(t * t);
      V hook_value_() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      V v_;
      unsigned count_;
    };


    namespace meta
    {

      /// Meta accumulator for rms.
      struct rms : public Meta_Accumulator< rms >
      {
	template <typename T, typename V>
	struct with
	{
	  typedef accu::rms<T,V> ret;
	};
      };

    } // end of namespace mln::accu::meta


# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename V>
    inline
    rms<T,V>::rms()
    {
      init();
    }

    template <typename T, typename V>
    inline
    void
    rms<T,V>::init()
    {
      v_ = literal::zero;
      count_ = 0;
    }

    template <typename T, typename V>
    inline
    void
    rms<T,V>::take_as_init_(const T& t)
    {
      v_ += t * t;
      ++count_;
    }

    template <typename T, typename V>
    inline
    void
    rms<T,V>::take(const T& t)
    {
      v_ += t * t;
      ++count_;
    }

    template <typename T, typename V>
    inline
    void
    rms<T,V>::take(const rms<T,V>& other)
    {
      v_ += other.v_;
      count_ += other.count_;
    }

    template <typename T, typename V>
    inline
    V
    rms<T,V>::to_result() const
    {
      if (count_ == 0)
	return V(0);
      return math::sqrt<V>(v_ / count_);
    }

    template <typename T, typename V>
    inline
    V
    rms<T,V>::hook_value_() const
    {
      return v_;
    }

    template <typename T, typename V>
    inline
    bool
    rms<T,V>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_RMS_HH
