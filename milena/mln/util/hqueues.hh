// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_HQUEUES_HH
# define MLN_UTIL_HQUEUES_HH

///
/// \brief  Generic class for hierarchical queues.
///
/// Hierarchical queues are often used with connected operators
/// (P. Salemebier's max tree algorithm relies on these queues). To be
/// efficient, the hiererachy is a static array and each are
/// preallocated using an histogram.
///
/// FIXME: consider hqueues as a site set ?


# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/histo/array.hh>
# include <mln/value/set.hh>


namespace mln
{

  namespace util
  {

    template <typename P, typename T>
    struct hqueues
    {
      enum {
	nvalues = mln_card(T)
      };

      hqueues(const histo::array<T>& h);

      const p_queue_fast<P>& operator[](unsigned i) const;
      p_queue_fast<P>& operator[](unsigned i);

      const p_queue_fast<P>& operator()(const T& v) const;
      p_queue_fast<P>& operator()(const T& v);

      const mln::value::set<T>& vset() const;

    protected:
      void pre_allocate_(unsigned i);

      const histo::array<T>& h_;
      const mln::value::set<T>& s_;
      std::vector<bool> allocated_;
      std::vector< p_queue_fast<P> >queues_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename T>
    inline
    hqueues<P,T>::hqueues(const histo::array<T>& h)
    : h_ (h),
      s_ (mln::value::set<T>::the()),
      allocated_ (nvalues, false),
      queues_ (nvalues)
    {
    }

    template <typename P, typename T>
    inline
    void
    hqueues<P,T>::pre_allocate_(unsigned i)
    {
      mln_precondition(i < nvalues);
      if (!allocated_[i])
	{
	  queues_[i].reserve(h_[i]);
	  allocated_[i] = true;
	}
    }


    template <typename P, typename T>
    inline
    const p_queue_fast<P>&
    hqueues<P,T>::operator[](unsigned i) const
    {
      mln_precondition(i < nvalues);
      pre_allocate_(i);
      return queues_[i];
    }

    template <typename P, typename T>
    inline
    p_queue_fast<P>&
    hqueues<P,T>::operator[](unsigned i)
    {
      mln_precondition(i < nvalues);
      pre_allocate_(i);
      return queues_[i];
    }

    template <typename P, typename T>
    inline
    const p_queue_fast<P>&
    hqueues<P,T>::operator()(const T& v) const
    {
      unsigned i = s_.index_of(v);
      pre_allocate_(i);
      return queues_[i];
    }

    template <typename P, typename T>
    inline
    p_queue_fast<P>&
    hqueues<P,T>::operator()(const T& v)
    {
      unsigned i = s_.index_of(v);
      pre_allocate_(i);
      return queues_[i];
    }


    template <typename P, typename T>
    inline
    const mln::value::set<T>&
    hqueues<P,T>::vset() const
    {
      return s_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // !MLN_UTIL_HQUEUES_HH
