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

#ifndef MLN_UTIL_COUPLE_HH
# define MLN_UTIL_COUPLE_HH

/// \file mln/util/couple.hh
///
/// Definition of a couple.
///
/// \todo revamp and refactor with util::site_pair or ord_pair.

# include <mln/core/concept/object.hh>
# include <mln/util/ord.hh>

namespace mln
{

  namespace util
  {

    template <typename T, typename U>
    class couple : public mln::Object< couple<T,U> >
    {
    public:
      couple();
      couple(const T& val1, const U& val2);

      /// Get the first member of the couple.
      /// \{
      const T& first() const;
      T& first();
      /// \}

      /// Get the second member of the couple.
      /// \{
      const U& second() const;
      U& second();
      /// \}

      /// Replace the first member of the couple by \a val.
      void change_first(const T& val);

      /// Replace the second member of the couple by \a val.
      void change_second(const U& val);

      /// Replace both members of the couple by \a val.
      void change_both(const T& first, const U& second);

    private:
      T first_;
      U second_;
    };


    template <typename T, typename U>
    bool operator==(const couple<T,U>& lhs, const couple<T,U>& rhs);

    template <typename T, typename U>
    bool operator< (const couple<T,U>& lhs, const couple<T,U>& rhs);

    template <typename T, typename U>
    bool operator<=(const couple<T,U>& lhs, const couple<T,U>& rhs);


    template <typename T, typename U>
    std::ostream& operator<<(std::ostream& ostr, const couple<T,U>& op);

  } // end of namespace mln::util


  namespace make
  {
    /// Construct an mln::util::couple on-the-fly.
    template <typename T, typename U>
    util::couple<T,U> couple(const T& val1, const T& val2);
  }


# ifndef MLN_INCLUDE_ONLY

  namespace util
  {

    /*---------------.
    | Construction.  |
    `---------------*/

    template <typename T, typename U>
    inline
    couple<T,U>::couple()
    {
    }

    template <typename T, typename U>
    inline
    couple<T,U>::couple(const T& val1, const U& val2)
    {
      change_both(val1, val2);
    }

    /*---------.
    | Access.  |
    `---------*/

    template <typename T, typename U>
    inline
    const T&
    couple<T,U>::first() const
    {
      return first_;
    }

    template <typename T, typename U>
    inline
    T&
    couple<T,U>::first()
    {
      return first_;
    }

    template <typename T, typename U>
    inline
    const U&
    couple<T,U>::second() const
    {
      return second_;
    }

    template <typename T, typename U>
    inline
    U&
    couple<T,U>::second()
    {
      return second_;
    }

    template <typename T, typename U>
    inline
    void
    couple<T,U>::change_first(const T& val)
    {
      first_ = val;
    }

    template <typename T, typename U>
    inline
    void
    couple<T,U>::change_second(const U& val)
    {
      second_ = val;
    }

    template <typename T, typename U>
    inline
    void
    couple<T,U>::change_both(const T& val1, const U& val2)
    {
      first_  = val1;
      second_ = val2;
    }

    /*-------------.
    | Comparison.  |
    `-------------*/

    template <typename T, typename U>
    inline
    bool operator==(const couple<T,U>& lhs, const couple<T,U>& rhs)
    {
      return lhs.first() == rhs.first() && lhs.second() == rhs.second();
    }

    template <typename T, typename U>
    inline
    bool operator< (const couple<T,U>& lhs, const couple<T,U>& rhs)
    {
      return
	util::ord_strict(lhs.first(), rhs.first()) ||
	(lhs.first() == rhs.first() &&
	 util::ord_strict(lhs.second(), rhs.second()));
    }

    template <typename T, typename U>
    inline
    bool operator<=(const couple<T,U>& lhs, const couple<T,U>& rhs)
    {
      return
	util::ord_strict(lhs.first(), rhs.first()) ||
	(lhs.first() == rhs.first() &&
	 util::ord_weak(lhs.second(), rhs.second()));
    }

    /*------------------.
    | Pretty-printing.  |
    `------------------*/

    template <typename T, typename U>
    inline
    std::ostream& operator<<(std::ostream& ostr, const couple<T,U>& op)
    {
      return ostr << '(' << op.first() << ',' << op.second() << ')';
    }

  } // end of namespace mln::util


  namespace make
  {

    template <typename T, typename U>
    inline
    util::couple<T,U>
    couple(const T& val1, const U& val2)
    {
      util::couple<T,U> tmp(val1, val2);
      return tmp;
    }

  } // end of namespace mln::make

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_UTIL_COUPLE_HH
