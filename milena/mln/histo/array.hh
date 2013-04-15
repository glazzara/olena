// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_HISTO_ARRAY_HH
# define MLN_HISTO_ARRAY_HH

/// \file
///
/// Define a generic histogram class.

# include <vector>
# include <algorithm>

# include <mln/value/set.hh>

// For conversion
# include <mln/convert/from_to.hh>
# include <mln/make/box1d.hh>


namespace mln
{

  // Forward declaration
  template <typename T> struct image1d;

  namespace histo
  {


    /// Generic histogram class over a value set with type \c T.
    template <typename T>
    struct array
    {
      typedef T value;

      array();

      array(const array& other);
      array& operator=(const array& other);

      void clear();

      unsigned operator()(const T& v) const;
      unsigned& operator()(const T& v);

      const std::vector<unsigned>& vect() const;
      const mln::value::set<T>& vset() const;
      unsigned operator[](unsigned i) const;
      unsigned& operator[](unsigned i);

      unsigned nvalues() const;

    protected:

      const mln::value::set<T>& s_;
      std::vector<unsigned> h_;
    };


    template <typename T>
    std::ostream& operator<<(std::ostream& ostr, const array<T>& h);

    /// \internal Conversion: histo::array -> image1d.
    template <typename V, typename T>
    void from_to_(const array<V>& from, image1d<T>& to);

# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    array<T>::array()
      : s_(mln::value::set<T>::the()),
	h_(s_.nvalues(), 0)
    {
      clear();
    }

    template <typename T>
    inline
    array<T>::array(const array& other)
      : s_(other.s_),
	h_(other.h_)
    {
    }

    template <typename T>
    inline
    array<T>&
    array<T>::operator=(const array& other)
    {
      if (&other == this)
	return *this;
      h_ = other.h_;
      return *this;
    }

    template <typename T>
    inline
    void
    array<T>::clear()
    {
      std::fill(h_.begin(), h_.end(), 0);
    }

    template <typename T>
    inline
    unsigned
    array<T>::operator()(const T& v) const
    {
      return h_[s_.index_of(v)];
    }

    template <typename T>
    inline
    unsigned&
    array<T>::operator()(const T& v)
    {
      return h_[s_.index_of(v)];
    }

    template <typename T>
    inline
    const mln::value::set<T>&
    array<T>::vset() const
    {
      return s_;
    }

    template <typename T>
    inline
    unsigned
    array<T>::operator[](unsigned i) const
    {
      mln_precondition(i < s_.nvalues());
      return h_[i];
    }

    template <typename T>
    inline
    unsigned&
    array<T>::operator[](unsigned i)
    {
      mln_precondition(i < s_.nvalues());
      return h_[i];
    }

    template <typename T>
    inline
    const std::vector<unsigned>&
    array<T>::vect() const
    {
      return h_;
    }

    template <typename T>
    inline
    unsigned array<T>::nvalues() const
    {
      return h_.size();
    }

    template <typename T>
    inline
    std::ostream& operator<<(std::ostream& ostr, const array<T>& h)
    {
      mln_viter(mln::value::set<T>) v(h.vset());
      for_all(v)
	if (h(v) != 0)
	  ostr << v << ':' << h(v) << ' ';
      return ostr;
    }


    // Conversions

    template <typename V, typename T>
    inline
    void
    from_to_(const array<V>& from, image1d<T>& to)
    {
      // FIXME: The code should looks like:

// 	box1d b(point1d(mln_min(V)), point1d(mln_max(V)));
// 	ima.init_(b, 0);
// 	for_all(v)
// 	  from_to(h(v), ima.at_( index_of(v) ));
      to.init_(make::box1d(from.nvalues()));
      for (unsigned i = 0; i < from.nvalues(); ++i)
	convert::from_to(from[i], to(point1d(i)));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::histo

} // end of namespace mln


#endif // ! MLN_HISTO_ARRAY_HH
