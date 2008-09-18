// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_ORDPAIR_HH
# define MLN_UTIL_ORDPAIR_HH

/// \file mln/util/ordpair.hh
/// \brief Definition of an ordered pair.

# include <mln/core/concept/object.hh>
# include <mln/util/ord.hh>


namespace mln
{

  namespace util
  {

    // FIXME: Rename as ord_pair.

    /// \brief  Ordered pair structure s.a. this->first <= this->second;
    /// ordered pairs are partially ordered using lexicographical
    /// ordering.
    template <typename T>
    struct ordpair_ : public mln::Object< ordpair_<T> >
    {
    public:
      ordpair_(const T& val1, const T& val2);
      
    public:
      /// Get the first (lowest) member of the pair.
      /// \{
      const T& first() const;
      T& first();
      /// \}

      /// Get the second (highest) member of the pair.
      /// \{
      const T& second() const;
      T& second();
      /// \}

      /// \brief Replace the first member of the pair by \a val, while
      /// keeping the relative order.
      ///
      /// \post \a first_ <= \a second_ (with <= being the
      /// mln::util::ord_weak relationship).
      void change_first(const T& val);

      /// \brief Replace the second member of the pair by \a val,
      /// while keeping the relative order.
      ///
      /// \post \a first_ <= \a second_ (with <= being the
      /// mln::util::ord_weak relationship).
      void change_second(const T& val);

      /// \brief Replace both members of the pair by \a val, while
      /// keeping the relative order.
      ///
      /// \post \a first_ <= \a second_ (with <= being the
      /// mln::util::ord_weak relationship).
      void change_both(const T& first, const T& second);

    private:
      T first_;
      T second_;
    };


    template <typename T>
    bool operator==(const ordpair_<T>& lhs, const ordpair_<T>& rhs);

    template <typename T>
    bool operator< (const ordpair_<T>& lhs, const ordpair_<T>& rhs);

    template <typename T>
    bool operator<=(const ordpair_<T>& lhs, const ordpair_<T>& rhs);


    template <typename T>
    std::ostream& operator<<(std::ostream& ostr, const ordpair_<T>& op);


    // FIXME: Rename as make::ord_pair.
    /// Routine to construct a ordpair on the fly.
    template <typename T>
    ordpair_<T> ordpair(const T& val1, const T& val2);



# ifndef MLN_INCLUDE_ONLY

    /*---------------.
    | Construction.  |
    `---------------*/

    template <typename T>
    inline
    ordpair_<T>::ordpair_(const T& val1, const T& val2)
    {
      change_both(val1, val2);
    }

    template <typename T>
    inline
    ordpair_<T>
    ordpair(const T& val1, const T& val2)
    {
      ordpair_<T> tmp(val1, val2);
      return tmp;
    }

    /*---------.
    | Access.  |
    `---------*/

    template <typename T>
    inline
    const T&
    ordpair_<T>::first() const
    {
      return first_;
    }

    template <typename T>
    inline
    T&
    ordpair_<T>::first()
    {
      return first_;
    }

    template <typename T>
    inline
    const T&
    ordpair_<T>::second() const
    {
      return second_;
    }

    template <typename T>
    inline
    T&
    ordpair_<T>::second()
    {
      return second_;
    }

    template <typename T>
    inline
    void
    ordpair_<T>::change_first(const T& val)
    {
      mln_precondition(util::ord_weak(first_, second_));

      if (util::ord_strict(val, second_))
	first_ = val;
      else
	second_ = val;

      mln_postcondition(util::ord_weak(first_, second_));
    }

    template <typename T>
    inline
    void
    ordpair_<T>::change_second(const T& val)
    {
      mln_precondition(util::ord_weak(first_, second_));

      if (util::ord_strict(first_, val))
	second_ = val;
      else
	first_ = val;

      mln_postcondition(util::ord_weak(first_, second_));
    }

    template <typename T>
    inline
    void
    ordpair_<T>::change_both(const T& val1, const T& val2)
    {
      if (util::ord_strict(val1, val2))
	{
	  first_  = val1;
	  second_ = val2;
	}
      else
	{
	  first_  = val2;
	  second_ = val1;
	}
      mln_postcondition(util::ord_weak(first_, second_));
    }

    /*-------------.
    | Comparison.  |
    `-------------*/

    template <typename T>
    inline
    bool operator==(const ordpair_<T>& lhs, const ordpair_<T>& rhs)
    {
      return lhs.first() == rhs.first() && lhs.second() == rhs.second();
    }

    template <typename T>
    inline
    bool operator< (const ordpair_<T>& lhs, const ordpair_<T>& rhs)
    {
      return
	util::ord_strict(lhs.first(), rhs.first()) ||
	(lhs.first() == rhs.first() &&
	 util::ord_strict(lhs.second(), rhs.second()));
    }

    template <typename T>
    inline
    bool operator<=(const ordpair_<T>& lhs, const ordpair_<T>& rhs)
    {
      return
	util::ord_strict(lhs.first(), rhs.first()) ||
	(lhs.first() == rhs.first() &&
	 util::ord_weak(lhs.second(), rhs.second()));
    }

    /*-------------------.
    | Pretty-printing..  |
    `-------------------*/

    template <typename T>
    inline
    std::ostream& operator<<(std::ostream& ostr, const ordpair_<T>& op)
    {
      return ostr << '(' << op.first() << ',' << op.second() << ')';
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_CORE_ROUTINE_EXACT_HH
