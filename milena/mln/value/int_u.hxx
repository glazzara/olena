// Copyright (C) 2007, 2008, 2009, 2010, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_VALUE_INT_U_HXX
# define MLN_VALUE_INT_U_HXX

/// \file
///
/// Definition of mln::value::int_u, a generic class for unsigned
/// integers.

# include <mln/value/int_u.hh>

# include <mln/debug/format.hh>


namespace mln
{

  namespace value
  {

    template <unsigned n>
    inline
    int_u<n>::int_u()
    {
    }

    template <unsigned n>
    inline
    int_u<n>::int_u(int i)
    {
      mln_precondition(i >= 0);
      mln_precondition(unsigned(i) <= mln_max(enc_));
      this->v_ = static_cast<enc_>(i);
    }

    template <unsigned n>
    inline
    int_u<n>::int_u(const mln::literal::zero_t&)
    {
      this->v_ = 0;
    }

    template <unsigned n>
    inline
    int_u<n>&
    int_u<n>::operator=(const mln::literal::zero_t&)
    {
      this->v_ = 0;
      return *this;
    }

    template <unsigned n>
    inline
    int_u<n>::int_u(const mln::literal::one_t&)
    {
      this->v_ = 1;
    }

    template <unsigned n>
    inline
    int_u<n>&
    int_u<n>::operator=(const mln::literal::one_t&)
    {
      this->v_ = 1;
      return *this;
    }

    template <unsigned n>
    inline
    int_u<n>::operator unsigned() const
    {
      return this->v_;
    }

    template <unsigned n>
    inline
    int
    int_u<n>::operator-() const
    {
      return - int(this->v_);
    }

    template <unsigned n>
    inline
    int_u<n>&
    int_u<n>::operator=(int i)
    {
      mln_precondition(i >= 0);
      mln_precondition(unsigned(i) <= mln_max(enc_));
      this->v_ = static_cast<enc_>(i);
      return *this;
    }

    template <unsigned n>
    inline
    int_u<n>
    int_u<n>::next() const
    {
      return this->v_ + 1;
    }

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const int_u<n>& i)
    {
      // FIXME: This code could be factored for almost every Value<*>...
      return ostr << debug::format(i.to_equiv()); // FIXME: is to_equiv OK?
    }

    template <unsigned n>
    inline
    std::istream& operator>>(std::istream& istr, int_u<n>& i)
    {
      return istr >> i.handle_();
    }


    // Conversions

    template <unsigned n>
    inline
    void
    from_to_(const int_u<n>& from, unsigned& to_)
    {
      to_ = from;
    }

    template <unsigned n>
    inline
    void
    from_to_(const int_u<n>& from, bool& to_)
    {
      to_ = (from != 0u);
    }

    template <unsigned n>
    inline
    void
    from_to_(const int_u<n>& from, float& to_)
    {
      to_ = static_cast<float>(from);
    }

    template <unsigned n>
    inline
    void
    from_to_(const int_u<n>& from, double& to_)
    {
      to_ = static_cast<double>(from);
    }

    template <unsigned m>
    inline
    void
    from_to_(const int_u<m>& from, qt::rgb32& to)
    {
      mlc_bool(m <= 8)::check();
      to = qt::rgb32(from, from, from);
    }

    template <unsigned m>
    inline
    void
    from_to_(const int_u<m>& from, rgb<m>& to)
    {
      to = rgb<m>(from, from, from);
    }

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_INT_U_HXX
