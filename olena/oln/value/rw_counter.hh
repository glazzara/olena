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
# include <set>
# include <oln/value/proxy.hh>
# include <oln/core/abstract/image/all.hh>


namespace oln
{


  namespace value
  {


    struct counter_rw
    {
      typedef std::map<unsigned, unsigned> map_t;
      typedef std::set<unsigned> set_t;
      static map_t n_rwc_;
      static map_t n_roc_;
      static map_t n_r_;
      static map_t n_w_;
      static set_t ids_;

      // read-write call
      template <typename I>
      static unsigned& n_readwrite_calls(const oln::abstract::image<I>& ima)
      {
	ids_.insert(ima.id());
	return n_rwc_[ima.id()];
      }
      // read-only call
      template <typename I>
      static unsigned& n_readonly_calls(const oln::abstract::image<I>& ima)
      {
	ids_.insert(ima.id());
	return n_roc_[ima.id()];
      }
      // read (effective)
      template <typename I>
      static unsigned& n_reads(const oln::abstract::image<I>& ima)
      {
	ids_.insert(ima.id());
	return n_r_[ima.id()];
      }
      // write (effective)
      template <typename I>
      static unsigned& n_writes(const oln::abstract::image<I>& ima)
      {
	ids_.insert(ima.id());
	return n_w_[ima.id()];
      }

      static void print(unsigned id)
      {
	ids_.insert(id);
	std::cout << "n_readwrite_calls = " << n_rwc_[id] << std::endl
		  << "n_readonly_calls  = " << n_roc_[id] << std::endl
		  << "n_reads           = " << n_r_[id]   << std::endl
		  << "n_writes          = " << n_w_[id]   << std::endl;
      }

      template <typename I>
      static void print(const oln::abstract::image<I>& ima)
      {
	print(ima.id());
      }

      static void print()
      {
	set_t::const_iterator it;
	for (it = ids_.begin(); it != ids_.end(); ++it)
	  print(*it);
      }
    };


# ifndef OLN_INCLUDE_ONLY

    counter_rw::map_t counter_rw::n_rwc_;
    counter_rw::map_t counter_rw::n_roc_;
    counter_rw::map_t counter_rw::n_r_;
    counter_rw::map_t counter_rw::n_w_;
    counter_rw::set_t counter_rw::ids_;

# endif


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
      I ima_;
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
      const I ima_;
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
      ++counter_rw::n_readwrite_calls(ima_);
    }

    // Read.
    template <typename I>
    template <typename V>
    rw_counter<I>::operator V() const
    {
      ++counter_rw::n_reads(ima_);
      V tmp = ima_(p_);
      return tmp;
    }

    // Explicit read.
    template <typename I>
    oln_rvalue(I)
    rw_counter<I>::value() const
    {
      ++counter_rw::n_reads(ima_);
      return ima_(p_);
    }
    
    // Write.

    template <typename I>
    template <typename V>
    rw_counter<I>&
    rw_counter<I>::operator=(const V& value)
    {
      ima_(p_) = value;
      ++counter_rw::n_writes(ima_);
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
      ++counter_rw::n_readonly_calls(ima_);
    }

    // Read.
    template <typename I>
    template <typename V>
    ro_counter<I>::operator V() const
    {
      ++counter_rw::n_reads(ima_);
      V tmp = ima_(p_);
      return tmp;
    }

    // Explicit read.
    template <typename I>
    oln_rvalue(I)
    ro_counter<I>::value() const
    {
      ++counter_rw::n_reads(ima_);
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
