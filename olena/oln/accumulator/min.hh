// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_ACCUMULATOR_MIN_HH
# define OLN_ACCUMULATOR_MIN_HH

# include <oln/core/concept/accumulator.hh>
# include <oln/core/internal/max_value.hh>


namespace oln
{

  namespace accumulator
  {

    template <typename T>
    struct min_ : public Accumulator< min_<T> >
    {
      typedef T argument;
      typedef T result;

      min_();

      void init()  const;
      const T& value() const;

      void operator()(const T& val) const;

    private:
      mutable T val_;
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename T>
    min_<T>::min_()
    {
      this->init();
    }

    template <typename T>
    void
    min_<T>::init() const
    {
      this->val_ = oln_max(T);
    }

    template <typename T>
    const T&
    min_<T>::value() const
    {
      return this->val_;
    }

    template <typename T>
    void
    min_<T>::operator()(const T& val) const
    {
      if (val < this->val_)
	this->val_ = val;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::accumulator

} // end of namespace oln

#endif // ! OLN_ACCUMULATOR_MIN_HH
