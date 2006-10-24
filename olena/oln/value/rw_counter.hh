// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_VALUE_RW_COUNTER_HH
# define OLN_CORE_VALUE_RW_COUNTER_HH

# include <iostream>
# include <map>
# include <oln/value/proxy.hh>
# include <oln/core/abstract/image/all.hh>


namespace oln
{


  namespace value
  {


    struct counter
    {
      typedef std::map<void*, unsigned> map_t;

      // read-write call
      static unsigned& n_readwrite_calls(const oln::type& ima)
      {
	static map_t n;
	return n[(void*)(&ima)];
      }
      // read-only call
      static unsigned& n_readonly_calls(const oln::type& ima)
      {
	static map_t n;
	return n[(void*)(&ima)];
      }
      // read (effective)
      static unsigned& n_reads(const oln::type& ima)
      {
	static map_t n;
	return n[(void*)(&ima)];
      }
      // write (effective)
      static unsigned& n_writes(const oln::type& ima)
      {
	static map_t n;
	return n[(void*)(&ima)];
      }

      static void print(const oln::type& ima)
      {
	std::cout << "n_readwrite_calls = " << n_readwrite_calls(ima) << std::endl
		  << "n_readonly_calls  = " << n_readonly_calls(ima)  << std::endl
		  << "n_reads           = " << n_reads(ima)           << std::endl
		  << "n_writes          = " << n_writes(ima)          << std::endl;
      }
    };


    template <typename I>
    class rw_counter : public abstract::proxy< rw_counter<I> >
    {
    public:
      
      typedef oln_value(I) value_type;

      // Ctor.
      rw_counter(oln::abstract::mutable_image<I>& ima,
		 const oln_psite(I)& p);
      
      // Read.
      template <typename V>
      operator V() const;

      // Explicit read.
      oln_rvalue(I) value() const;

      // Write.
      template <typename V>
      rw_counter<I>& operator=(const V& value);

    protected:
      I& ima_;
      const oln_psite(I)& p_;
    };


    template <typename I>
    std::ostream& operator<<(std::ostream& ostr, const rw_counter<I>& proxy);


    template <typename I>
    class ro_counter : public abstract::proxy< ro_counter<I> >
    {
    public:

      typedef oln_value(I) value_type;
      
      // Ctor.
      ro_counter(const oln::abstract::image<I>& ima,
		 const oln_psite(I)& p);
      
      // Read.
      template <typename V>
      operator V() const;

      // Explicit read.
      oln_rvalue(I) value() const;

    protected:
      const I& ima_;
      const oln_psite(I)& p_;
    };


    template <typename I>
    std::ostream& operator<<(std::ostream& ostr, const ro_counter<I>& proxy);



# ifndef OLN_INCLUDE_ONLY

    // Ctor.
    template <typename I>
    rw_counter<I>::rw_counter(oln::abstract::mutable_image<I>& ima,
			      const oln_psite(I)& p)
      : ima_(ima.exact()),
	p_(p)
    {
      ++counter::n_readwrite_calls(ima_);
    }

    // Read.
    template <typename I>
    template <typename V>
    rw_counter<I>::operator V() const
    {
      ++counter::n_reads(ima_);
      V tmp = ima_(p_);
      return tmp;
    }

    // Explicit read.
    template <typename I>
    oln_rvalue(I)
    rw_counter<I>::value() const
    {
      ++counter::n_reads(ima_);
      return ima_(p_);
    }
    
    // Write.

    template <typename I>
    template <typename V>
    rw_counter<I>&
    rw_counter<I>::operator=(const V& value)
    {
      ima_(p_) = value;
      ++counter::n_writes(ima_);
      return *this;
    }

    // Op <<.
    template <typename I>
    std::ostream& operator<<(std::ostream& ostr,
			     const rw_counter<I>& proxy)
    {
      return ostr << proxy.value();
    }


    // Ctor.
    template <typename I>
    ro_counter<I>::ro_counter(const oln::abstract::image<I>& ima,
			      const oln_psite(I)& p)
      : ima_(ima.exact()),
	p_(p)
    {
      ++counter::n_readonly_calls(ima_);
    }

    // Read.
    template <typename I>
    template <typename V>
    ro_counter<I>::operator V() const
    {
      ++counter::n_reads(ima_);
      V tmp = ima_(p_);
      return tmp;
    }

    // Explicit read.
    template <typename I>
    oln_rvalue(I)
    ro_counter<I>::value() const
    {
      ++counter::n_reads(ima_);
      return ima_(p_);
    }

    // Op <<.
    template <typename I>
    std::ostream& operator<<(std::ostream& ostr,
			     const ro_counter<I>& proxy)
    {
      return ostr << proxy.value();
    }

# endif

  } // end of namespace oln::value

} // end of namespace oln


#endif // ! OLN_CORE_VALUE_RW_COUNTER_HH
