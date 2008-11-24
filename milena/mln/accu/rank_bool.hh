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

#ifndef MLN_ACCU_RANK_BOOL_HH
# define MLN_ACCU_RANK_BOOL_HH

/// \file mln/accu/rank_bool.hh
///
/// Define an rank accumulator.
///
/// \todo There is no-arg-ctor so this accumulator does not support
/// deferred initialization!
///
/// \todo Add untake routines...

# include <mln/accu/internal/base.hh>


namespace mln
{

  namespace accu
  {

    // Fwd declaration.
    template <typename T> struct rank;

    /// rank accumulator class for Boolean.
    template <>
    struct rank<bool> : public mln::accu::internal::base< bool, rank<bool> >
    {
      typedef bool argument;

      rank();
      rank(unsigned k);

      /// Manipulators.
      /// \{
      void init();
      void take_as_init(const argument& t);
      void take(const argument& t);
      void take(const rank<bool>& other);
      void untake(const argument& t);
      void untake(const rank<bool>& other);
      /// \}

      /// Get the value of the accumulator.
      bool to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:
      unsigned nfalse_;
      unsigned k_; // 0 <= k_
    };


# ifndef MLN_INCLUDE_ONLY

    inline
    rank<bool>::rank()
    {
      init();
    }

    inline
    rank<bool>::rank(unsigned k)
      : k_(k)
    {
      init();
    }

    inline
    void
    rank<bool>::init()
    {
      nfalse_ = 0;
    }

    inline
    void rank<bool>::take_as_init(const argument& t)
    {
      nfalse_ = t ? 0 : 1;
    }

    inline
    void rank<bool>::take(const argument& t)
    {
      if (t == false)
	++nfalse_;
    }

    inline
    void rank<bool>::untake(const argument& t)
    {
      if (t == false)
	{
	  mln_assertion(nfalse_ > 0);
	  --nfalse_;
	}
    }

    inline
    void
    rank<bool>::take(const rank<bool>& other)
    {
      nfalse_ += other.nfalse_;
    }

    inline
    void
    rank<bool>::untake(const rank<bool>& other)
    {
      mln_precondition(other.nfalse_ <= nfalse_);
      nfalse_ -= other.nfalse_;
    }

    inline
    bool
    rank<bool>::to_result() const
    {
      return k_ >= nfalse_;
    }

    inline
    bool
    rank<bool>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_RANK_BOOL_HH
