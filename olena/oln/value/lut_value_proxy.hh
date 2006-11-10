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

#ifndef OLN_CORE_VALUE_LUT_VALUE_PROXY_HH
# define OLN_CORE_VALUE_LUT_VALUE_PROXY_HH

# include <iostream>
# include <map>
# include <set>
# include <oln/value/proxy.hh>
# include <oln/core/abstract/image/all.hh>


namespace oln
{

  namespace value
  {

    /* FIXME: What entity we use to keep a reference on the pointed
       value of the LUT:
         - a LUT raw iterator (current choice), or
         - a value iterator, or
         - the value itself.
       to keep a reference on the underlying value?

       The third choice (value) /seems/ the most elegant from the
       exterior of the proxy, but it's probably the most error-prone
       too.  */


    /*------------------.
    | lut_value_proxy.  |
    `------------------*/

    template <typename Lut>
    class lut_value_proxy :
      public abstract::proxy< lut_value_proxy<Lut> >
    {
    public:

      typedef typename Lut::new_value_type value_type;
      typedef typename Lut::const_iterator lut_iter_type;
      
      // Ctor.
      lut_value_proxy(const Lut& lut, const value_type& v);
      
      // Read.
      template <typename V>
      operator V() const;

      // Explicit read.
      value_type value() const;

      // Write.
      template <typename V>
      lut_value_proxy<Lut>& operator=(const V& value);

      // FIXME: Is this really necessary?
      bool is_valid() const;

    protected:
      const Lut& lut_;
      lut_iter_type i_;
    };


    template <typename Lut>
    std::ostream& operator<<(std::ostream& ostr,
			     const lut_value_proxy<Lut>& proxy);


# ifndef OLN_INCLUDE_ONLY

    // Ctor.
    template <typename Lut>
    lut_value_proxy<Lut>::lut_value_proxy(const Lut& lut,
					  const typename lut_value_proxy<Lut>::value_type& v)
      : lut_(lut)
    {
      i_ = lut_.find(v);
      postcondition(is_valid());
    }

    // Read.
    template <typename Lut>
    template <typename V>
    lut_value_proxy<Lut>::operator V() const
    {
      precondition(is_valid());
      V tmp = i_->second; 
      return tmp;
    }

    // Explicit read.
    template <typename Lut>
    typename lut_value_proxy<Lut>::value_type
    lut_value_proxy<Lut>::value() const
    {
      precondition(is_valid());
      precondition(i_ != lut_.end());
      return i_->second; 
    }

    template <typename Lut>
    bool
    lut_value_proxy<Lut>::is_valid() const
    {
      return i_ != lut_.end();
    }

    // Op <<.
    template <typename Lut>
    std::ostream& operator<<(std::ostream& ostr,
			     const lut_value_proxy<Lut>& proxy)
    {
      return ostr << proxy.value();
    }

# endif



    /*--------------------------.
    | mutable_lut_value_proxy.  |
    `--------------------------*/

    template <typename Lut>
    class mutable_lut_value_proxy :
      public abstract::proxy< mutable_lut_value_proxy<Lut> >
    {
    public:

      typedef typename Lut::new_value_type value_type;
      typedef typename Lut::iterator lut_iter_type;
      
       // Ctor.
      mutable_lut_value_proxy(Lut& lut, const value_type& v);
      
      // Read.
      template <typename V>
      operator V() const;

      // Explicit read.
      value_type value() const;

      // Write.
      template <typename V>
      mutable_lut_value_proxy<Lut>& operator=(const V& value);

      // FIXME: Is this really necessary?
      bool is_valid() const;

    protected:
      Lut& lut_;
      lut_iter_type i_;
    };


    template <typename Lut>
    std::ostream& operator<<(std::ostream& ostr,
			     const mutable_lut_value_proxy<Lut>& proxy);


# ifndef OLN_INCLUDE_ONLY

    // Ctor.
    template <typename Lut>
    mutable_lut_value_proxy<Lut>::mutable_lut_value_proxy(Lut& lut,
							  const typename mutable_lut_value_proxy<Lut>::value_type& v)
      : lut_(lut)
    {
      i_ = lut_.find(v);
      postcondition(is_valid());
    }

    // Read.
    template <typename Lut>
    template <typename V>
    mutable_lut_value_proxy<Lut>::operator V() const
    {
      // FIXME: Turn this into a is_valid method.
      precondition(is_valid());
      V tmp = i_->second; 
      return tmp;
    }

    // Explicit read.
    template <typename Lut>
    typename mutable_lut_value_proxy<Lut>::value_type
    mutable_lut_value_proxy<Lut>::value() const
    {
      // FIXME: Turn this into a is_valid method.
      precondition(is_valid());
      precondition(i_ != lut_.end());
      return i_->second; 
    }
    
    // Write.

    template <typename Lut>
    template <typename V>
    mutable_lut_value_proxy<Lut>&
    mutable_lut_value_proxy<Lut>::operator=(const V& value)
    {
      precondition(is_valid());

      typedef typename Lut::orig_value_type orig_value_t;
      typedef typename Lut::new_value_type new_value_t;

      // Current ``new'' value.
      new_value_t cur_new_val = i_->first;

      // Shortcuts.
      typedef lut_iter_type new_iter_t;
      typedef std::pair<new_iter_t, new_iter_t> new_iter_range_t;

      // Current ``original'' value(s) associated to the next ``new''
      // value VALUE.
      new_iter_range_t cur_orig_val_range =
	lut_.new_to_orig_map().equal_range(cur_new_val);
      // A temporary map where additional bindings will be stored.
      typename Lut::new_to_orig_map_type additional_bindings;

      for (new_iter_t j = cur_orig_val_range.first;
	   j != cur_orig_val_range.second; ++j)
	{
	  // Update the orig-to-new map (substitution).
	  orig_value_t orig_val = j->second;
	  lut_.orig_to_new_map()[orig_val] = value;
	  // Populate the temp map.
	  additional_bindings.insert(std::make_pair(value, orig_val));
	}
      // Update the new-to-orig map (deletion).
      lut_.new_to_orig_map().erase(cur_orig_val_range.first,
					 cur_orig_val_range.second);
      // Update the new-to-orig map (insertion).
      lut_.new_to_orig_map().insert(additional_bindings.begin(),
					  additional_bindings.end());
      return *this;
    }

    template <typename Lut>
    bool
    mutable_lut_value_proxy<Lut>::is_valid() const
    {
      return i_ != lut_.end();
    }

    // Op <<.
    template <typename Lut>
    std::ostream& operator<<(std::ostream& ostr,
			     const mutable_lut_value_proxy<Lut>& proxy)
    {
      return ostr << proxy.value();
    }

# endif

  } // end of namespace oln::value

} // end of namespace oln


#endif // ! OLN_CORE_VALUE_LUT_VALUE_PROXY_HH
