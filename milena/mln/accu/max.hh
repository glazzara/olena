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

#ifndef MLN_ACCU_MAX_HH
# define MLN_ACCU_MAX_HH

/*! \file mln/accu/max.hh
 *
 * \brief Define an accumulator that computes a max.
 */

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/value/props.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic max accumulator class.
     *
     * The parameter \c V is the type of values.
     */
    template <typename V>
    struct max_ : public Accumulator< max_<V> >
    {
      typedef V value;
      typedef V result;

      max_();

      void init();
      void take_as_init(const value& v);
      void take(const value& v);
      void take(const max_<V>& other);

      V to_result() const;
      
    protected:

      V v_;
    };


    template <typename I> struct max_< util::pix<I> >;


    // FIXME: Doc!
    struct max : public Meta_Accumulator< max >
    {
      template <typename V>
      struct with
      {
	typedef max_<V> ret;
      };
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename V>
    max_<V>::max_()
    {
      init();
    }

    template <typename V>
    void
    max_<V>::init()
    {
      v_ = mln_min(V);
    }

    template <typename V>
    void
    max_<V>::take_as_init(const value& v)
    {
      v_ = v;
    }

    template <typename V>
    void
    max_<V>::take(const value& v)
    {
      if (v > v_)
	v_ = v;
    }

    template <typename V>
    void
    max_<V>::take(const max_<V>& other)
    {
      if (other.v_ > v_)
	v_ = other.v_;
    }

    template <typename V>
    V
    max_<V>::to_result() const
    {
      return v_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MAX_HH
