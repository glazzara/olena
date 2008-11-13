// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_COUNT_HH
# define MLN_ACCU_COUNT_HH

/// \file mln/accu/count.hh
///
/// Define an accumulator that counts.

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>


namespace mln
{

  namespace accu
  {

    /// Generic counter accumulator class.
    /// The parameter \a T is the type to be count.
    template <typename T>
    struct count : public mln::accu::internal::base< unsigned , count<T> >
    {
      typedef T argument;

      count();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument&);
      void take(const count<T>& other);

      void untake(const argument&);
      void untake(const count<T>& other);

      /// Force the value of the counter to \a c.
      void set_value(unsigned c);
      /// \}

      /// Get the value of the accumulator.
      unsigned to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:
      /// The value of the counter.
      unsigned count_;
    };


    namespace meta
    {

      /// Meta accumulator for count.
      struct count : public Meta_Accumulator< count >
      {
	template <typename T>
	struct with
	{
	  typedef accu::count<T> ret;
	};
      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    count<T>::count()
    {
      init();
    }

    template <typename T>
    inline
    void
    count<T>::init()
    {
      count_ = 0;
    }

    template <typename T>
    inline
    void
    count<T>::take(const argument&)
    {
      ++count_;
    }

    template <typename T>
    inline
    void
    count<T>::untake(const argument&)
    {
      mln_precondition(count_ > 0);
      --count_;
    }

    template <typename T>
    inline
    void
    count<T>::take(const count<T>& other)
    {
      count_ += other.count_;
    }

    template <typename T>
    inline
    void
    count<T>::untake(const count<T>& other)
    {
      mln_precondition(other.count_ <= count_);
      count_ -= other.count_;
    }

    template <typename T>
    inline
    unsigned
    count<T>::to_result() const
    {
      return count_;
    }

    template <typename T>
    inline
    void
    count<T>::set_value(unsigned c)
    {
      count_ = c;
    }

    template <typename T>
    inline
    bool
    count<T>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_COUNT_HH
