// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_MIN_HH
# define MLN_ACCU_MIN_HH

/*! \file mln/accu/min.hh
 *
 * \brief Define an accumulator that computes a min.
 */

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/trait/value_.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {


    /*! \brief Generic min accumulator class.
     *
     * The parameter \c T is the type of values.
     */
    template <typename T>
    struct min : public mln::accu::internal::base< const T&, min<T> >
    {
      typedef T argument;

      min();

      /// Manipulators.
      /// \{
      void init();
      void take_as_init(const argument& t);
      void take(const argument& t);
      void take(const min<T>& other);
      /// \}

      /// Get the value of the accumulator.
      const T& to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      T t_;
    };


    template <typename I> struct min< util::pix<I> >;



    namespace meta
    {

      /// Meta accumulator for min.

      struct min : public Meta_Accumulator< min >
      {
	template <typename T>
	struct with
	{
	  typedef accu::min<T> ret;
	};
      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    min<T>::min()
    {
      init();
    }

    template <typename T>
    inline
    void
    min<T>::init()
    {
      t_ = mln_max(T);
    }

    template <typename T>
    inline
    void min<T>::take_as_init(const argument& t)
    {
      t_ = t;
    }

    template <typename T>
    inline
    void min<T>::take(const argument& t)
    {
      if (t < t_)
	t_ = t;
    }

    template <typename T>
    inline
    void
    min<T>::take(const min<T>& other)
    {
      if (other.t_ < t_)
	t_ = other.t_;
    }

    template <typename T>
    inline
    const T&
    min<T>::to_result() const
    {
      return t_;
    }

    template <typename T>
    inline
    bool
    min<T>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MIN_HH
