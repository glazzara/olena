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

#ifndef MLN_VALUE_PROXY_HH
# define MLN_VALUE_PROXY_HH

/*! \file mln/value/proxy.hh
 *
 * \brief Define a generic proxy class for an image pixel value.
 */

# include <mln/core/concept/value.hh>
# include <mln/trait/value_.hh>
# include <mln/metal/unconst.hh>


namespace mln
{

  // Fwd decl.
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

    /*! \brief Generic proxy class for an image pixel value.
     *
     * The parameter \c I is an image type.
     */
    template <typename I>
    class proxy : public Value< proxy<I> >
    {
    public:

      /// Encoding associated type.
      typedef void enc; // FIXME

      /// Equivalent associated type.
      typedef mln_value(I) equiv;

      /// Constructor.
      proxy(I& ima, const mln_psite(I)& p);

      /// Destructor.
      ~proxy();

      /// Assignment (write access); general version.
      template <typename V>
      proxy<I>& operator=(const V& v);

      /// Assignment (write access); replacement for default op.
      proxy<I>& operator=(const proxy<I>& rhs);

      /// Assignment (write access); with other proxy.
      template <typename II>
      proxy<I>& operator=(const proxy<II>& rhs);

      /// Conversion (read access); general version.
      template <typename V>
      operator V() const;

      /// Conversion (read access); precise version.
      operator mln_value(I)() const;

      /// Explicit read access.
      mln_value(I) to_value() const;

    protected:
      I& ima_;
      mln_psite(I) p_;
    };



    /*! \brief Generic proxy class for an image pixel value.
     *
     * The parameter \c I is an image type.
     */
    template <typename I>
    class proxy< const I > : public Value< proxy<const I> >
    {
    public:

      /// Encoding associated type.
      typedef void enc; // FIXME

      /// Equivalent associated type.
      typedef mln_value(I) equiv;

      /// Constructor.
      proxy(const I& ima, const mln_psite(I)& p);

      /// Destructor.
      ~proxy();

      /// Conversion (read access); general version.
      template <typename V>
      operator V() const;

      /// Conversion (read access); precise version.
      operator mln_value(I)() const;

      /// Explicit read access.
      mln_value(I) to_value() const;

    protected:
      const I& ima_;
      mln_psite(I) p_;
    };



    template <typename I>
    bool operator==(const proxy<I>& lhs, const mln_value(I)& rhs);

    template <typename I>
    bool operator==(const mln_value(I)& lhs, const proxy<I>& rhs);

    template <typename I, typename J>
    bool operator==(const proxy<I>& lhs, const proxy<J>& rhs);


    template <typename I>
    bool operator<(const proxy<I>& lhs, const mln_value(I)& rhs);

    template <typename I>
    bool operator<(const mln_value(I)& lhs, const proxy<I>& rhs);

    template <typename I, typename J>
    bool operator<(const proxy<I>& lhs, const proxy<J>& rhs);


    // FIXME: Ops such as +=, etc.



    /*! \brief Print a value proxy \p x into the output stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] x A value proxy.
     *
     * \return The modified output stream \p ostr.
     */
    template <typename I>
    std::ostream& operator<<(std::ostream& ostr, const proxy<I>& x);


# ifndef MLN_INCLUDE_ONLY

    // proxy<I>

    template <typename I>
    inline
    proxy<I>::proxy(I& ima, const mln_psite(I)& p)
      : ima_(ima),
	p_(p)
    {
    }

    template <typename I>
    inline
    proxy<I>::~proxy()
    {
      mln_rvalue(I) (I::*mr)(const mln_psite(I)&) const = & I::read_;
      mr = 0;
      void (I::*mw)(const mln_psite(I)&, const mln_value(I)&) = & I::write_;
      mw = 0;
    }

    template <typename I>
    template <typename V>
    inline
    proxy<I>&
    proxy<I>::operator=(const V& v)
    {
      ima_.write_(p_, v);
      return *this;
    }

    template <typename I>
    inline
    proxy<I>&
    proxy<I>::operator=(const proxy<I>& rhs)
    {
      this->operator=(rhs.to_value());
      return *this;
    }

    template <typename I>
    template <typename II>
    inline
    proxy<I>&
    proxy<I>::operator=(const proxy<II>& rhs)
    {
      this->operator=(rhs.to_value());
      return *this;
    }

    template <typename I>
    template <typename V>
    inline
    proxy<I>::operator V() const
    {
      return ima_.read_(p_);
    }

    template <typename I>
    inline
    proxy<I>::operator mln_value(I)() const
    {
      return ima_.read_(p_);
    }

    template <typename I>
    inline
    mln_value(I)
    proxy<I>::to_value() const
    {
      return ima_.read_(p_);
    }

    // proxy<const I>

    template <typename I>
    inline
    proxy<const I>::proxy(const I& ima, const mln_psite(I)& p)
      : ima_(ima),
	p_(p)
    {
    }

    template <typename I>
    inline
    proxy<const I>::~proxy()
    {
      mln_rvalue(I) (I::*mr)(const mln_psite(I)&) const = & I::read_;
      mr = 0;
    }

    template <typename I>
    template <typename V>
    inline
    proxy<const I>::operator V() const
    {
      return ima_.read_(p_);
    }

    template <typename I>
    inline
    proxy<const I>::operator mln_value(I)() const
    {
      return ima_.read_(p_);
    }

    template <typename I>
    inline
    mln_value(I)
    proxy<const I>::to_value() const
    {
      return ima_.read_(p_);
    }

    // operator <<

    template <typename I>
    inline
    std::ostream& operator<<(std::ostream& ostr, const proxy<I>& x)
    {
      return ostr << x.to_value();
    }

    // operator ==

    template <typename I>
    inline
    bool operator==(const proxy<I>& lhs, const mln_value(I)& rhs)
    {
      return lhs.to_value() == rhs;
    }

    template <typename I>
    inline
    bool operator==(const mln_value(I)& lhs, const proxy<I>& rhs)
    {
      return lhs == rhs.to_value();
    }

    template <typename I, typename J>
    inline
    bool operator==(const proxy<I>& lhs, const proxy<J>& rhs)
    {
      return lhs.to_value() == rhs.to_value();
    }

    // operator <

    template <typename I>
    inline
    bool operator<(const proxy<I>& lhs, const mln_value(I)& rhs)
    {
      return lhs.to_value() < rhs;
    }

    template <typename I>
    inline
    bool operator<(const mln_value(I)& lhs, const proxy<I>& rhs)
    {
      return lhs < rhs.to_value();
    }

    template <typename I, typename J>
    inline
    bool operator<(const proxy<I>& lhs, const proxy<J>& rhs)
    {
      return lhs.to_value() < rhs.to_value();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_PROXY_HH
