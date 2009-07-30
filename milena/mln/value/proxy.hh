// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_VALUE_PROXY_HH
# define MLN_VALUE_PROXY_HH

/// \file
///
/// Define a generic proxy class for an image pixel value.
///
/// \todo Maybe we should overload conversion operator with proxy<I>
///  or... (we have to choose) maybe we should add cpy ctors (with
///  proxy<I> and proxy<J>).
///
/// \todo Rename value::proxy as value::shell and fix the specific
/// code from Fred (the 2nd parameter, namely F, is useless)
///
/// \todo Checks in ~proxy eventually should be re-activated.
 
# include <mln/core/concept/proxy.hh>
# include <mln/trait/value_.hh>
# include <mln/metal/unconst.hh>


namespace mln
{

  // Forward declaration.
  namespace value {
    template <typename I> class proxy;
  }


  namespace trait
  {

    template <typename I>
    struct value_< mln::value::proxy<I> >
      :
      value_< mln_value(I) >
    {
    };

    template <typename I>
    struct value_< mln::value::proxy<const I> >
      :
      value_< mln_value(I) >
    {
    };

  } // end of namespace trait


  namespace value
  {

    /// \brief Generic proxy class for an image pixel value.
    ///
    /// The parameter \c I is an image type.
    //
    template <typename I>
    class proxy : public Proxy< proxy<I> >,
		  public mln::internal::proxy_impl< mln_value(I), proxy<I> >
    {
    public:

      /// Encoding associated type.
      typedef void enc; // FIXME

      /// Equivalent associated type.
      typedef mln_value(I) equiv;

      /// Constructor.
      proxy();

      /// Constructor.
      proxy(I& ima, const mln_psite(I)& p);

      /// Destructor.
      ~proxy();

      // Assignment (write access); "natural" version.
      proxy<I>& operator=(const mln_value(I)& v);

      /// Assignment (write access); replacement for default op.
      proxy<I>& operator=(const proxy<I>& rhs);

      /// Assignment (write access); with other proxy.
      template <typename J>
      proxy<I>& operator=(const proxy<J>& rhs);

//       /// Conversion (read access); "natural" version.
//       operator mln_value(I)() const;


//       /// Conversion (read access); toward a proxy.
//       template <typename J>
//       operator proxy<J>() const;


//       /// Conversion (read access); general version.
//       template <typename V>
//       operator V() const;


      /// Explicit read access.
      mln_value(I) to_value() const;


      // As a proxy:

      /// Return the proxy subject.
      mln_value(I) subj_();

    protected:
      I* ima_;
      mln_psite(I) p_;
    };




# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    proxy<I>::proxy()
      : ima_(0)
    {
    }

    template <typename I>
    inline
    proxy<I>::proxy(I& ima, const mln_psite(I)& p)
      : ima_(&ima),
	p_(p)
    {
    }

    template <typename I>
    inline
    proxy<I>::~proxy()
    {
//       mln_rvalue(I) (I::*mr)(const mln_psite(I)&) const = & I::read_;
//       mr = 0;
//       void (I::*mw)(const mln_psite(I)&, const mln_value(I)&) = & I::write_;
//       mw = 0;
    }

    template <typename I>
    inline
    proxy<I>&
    proxy<I>::operator=(const mln_value(I)& v)
    {
      mln_precondition(ima_ != 0);
      ima_->write_(p_, v);
      return *this;
    }

    template <typename I>
    inline
    proxy<I>&
    proxy<I>::operator=(const proxy<I>& rhs)
    {
      mln_precondition(ima_ != 0);
      if (&rhs == this)
	return *this; // No-op.
      this->operator=(rhs.to_value());
      return *this;
    }

    template <typename I>
    template <typename J>
    inline
    proxy<I>&
    proxy<I>::operator=(const proxy<J>& rhs)
    {
      mln_precondition(ima_ != 0);
      this->operator=(rhs.to_value());
      return *this;
    }

//     template <typename I>
//     template <typename V>
//     inline
//     proxy<I>::operator V() const
//     {
//       mln_precondition(ima_ != 0);
//       return ima_->read_(p_);
//     }

//     template <typename I>
//     inline
//     proxy<I>::operator mln_value(I)() const
//     {
//       mln_precondition(ima_ != 0);
//       return ima_->read_(p_);
//     }

    template <typename I>
    inline
    mln_value(I)
    proxy<I>::to_value() const
    {
      mln_precondition(ima_ != 0);
      return ima_->read_(p_);
    }

    template <typename I>
    inline
    mln_value(I)
    proxy<I>::subj_()
    {
      mln_precondition(ima_ != 0);
      return ima_->read_(p_);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_PROXY_HH
