// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_ACCU_MAJ_H_HH
# define MLN_ACCU_MAJ_H_HH

/// \file mln/accu/maj_h.hh
///
/// Define an accumulator that computes a maj_h.


# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>
# include <mln/trait/value_.hh>
# include <mln/util/pix.hh>
# include <mln/accu/histo.hh>
# include <vector>


namespace mln
{

  namespace accu
  {


    /// \brief Compute the majority value.
    /*!
     * It is based on a histogram.
     * The parameter \c T is the type of values.
     *
     * \ingroup modaccuvalues
     */
    template <typename T>
    struct maj_h : public mln::accu::internal::base< const T& , maj_h<T> >
    {
      typedef T argument;

      maj_h();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void untake(const argument& t);
      void take(const maj_h<T>& other);
      /// \}

      /// Get the value of the accumulator.
      const T& to_result() const;

      //operator T () const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

      void update_() const;

      mutable bool valid_;

      const accu::histo<T>& histo() const;

    protected:

      mutable argument t_;
      mutable accu::histo<T> h_;
    };


    template <typename I> struct maj_h< util::pix<I> >;


    namespace meta
    {

      /// Meta accumulator for maj_h.

      struct maj_h : public Meta_Accumulator< maj_h >
      {
	template <typename T>
	struct with
	{
	  typedef accu::maj_h<T> ret;
	};
      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    maj_h<T>::maj_h()
    {
      init();
      valid_ = true;
      //FIXME: Not generic
      t_ = literal::zero;
    }

    template <typename T>
    inline
    void
    maj_h<T>::init()
    {
      h_.init();
    }

    template <typename T>
    inline
    void
    maj_h<T>::take(const argument& t)
    {
      h_.take(t);

      //update return value
      if (h_(t) > h_(t_))
        t_ = t;
    }

    template <typename T>
    inline
    void
    maj_h<T>::untake(const argument& t)
    {
      h_.untake(t);

      if (valid_)
	valid_ = false;
    }

    template <typename T>
    inline
    void
    maj_h<T>::take(const maj_h<T>& other)
    {
      h_.take(other.h_);

      if (valid_)
        valid_ = false;
    }

    template <typename T>
    inline
    void
    maj_h<T>::update_() const
    {
      const std::vector<unsigned>& v = h_.vect();

      for(unsigned i = 0; i != v.size(); i++)
        {
          // if nb referents of occurrence i > nb referents of t_
          if (v[i] > h_(t_))
            t_ = h_.vset()[i]; // t_ <- current
        }
      valid_ = true;
    }

    template <typename T>
    inline
    const T&
    maj_h<T>::to_result() const
    {
      if (not valid_)
	update_();
      return t_;
    }

    template <typename T>
    inline
    bool
    maj_h<T>::is_valid() const
    {
      return true;
    }

    template <typename V>
    inline
    const accu::histo<V>&
    maj_h<V>::histo() const
    {
      return h_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MAJ_H _HH
