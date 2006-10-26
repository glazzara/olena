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

#ifndef OLN_CORE_LOOKUP_TABLE_HH
# define OLN_CORE_LOOKUP_TABLE_HH

# include <map>
# include <ostream>

namespace oln
{

  // FIXME: Document!

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare = std::less<Orig_Value>,
	    typename New_Value_Compare = std::less<New_Value> >
  class lookup_table
  { 
    typedef lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare> self_t;

  public:
    typedef Orig_Value orig_value_type;
    typedef New_Value new_value_type;
    typedef std::map<Orig_Value, New_Value, Orig_Value_Compare> orig_to_new_map_type;
    typedef std::multimap<New_Value, Orig_Value, New_Value_Compare> new_to_orig_map_type;

    typedef typename new_to_orig_map_type::const_iterator const_iterator;
    typedef typename new_to_orig_map_type::iterator iterator;

  public:
    lookup_table();

    self_t& add (const orig_value_type& orig_value, const new_value_type& new_value);

    const new_value_type operator() (const orig_value_type& orig_value) const;

    /// (Internal) iterators.
    /// \{
    const_iterator begin() const;
    iterator begin();

    const_iterator end() const;
    iterator end();
    /// \}

    const_iterator find(const new_value_type& val) const;
    iterator find(const new_value_type& val);

    /// Accessors.
    /// \{
    const orig_to_new_map_type& orig_to_new_map() const;
    orig_to_new_map_type& orig_to_new_map();
    const new_to_orig_map_type& new_to_orig_map() const;
    new_to_orig_map_type& new_to_orig_map();
    /// \}

  private:
    orig_to_new_map_type orig_to_new_map_;
    new_to_orig_map_type new_to_orig_map_;
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  lookup_table<Orig_Value, New_Value,
	       Orig_Value_Compare, New_Value_Compare>::lookup_table() :
    orig_to_new_map_()
  {
  }

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>& 
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::add (const Orig_Value& orig_value,
										   const New_Value& new_value)
  {
    orig_to_new_map_.insert(std::make_pair(orig_value, new_value));
    new_to_orig_map_.insert(std::make_pair(new_value, orig_value));
    return *this;
  }

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  const New_Value
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::operator () (const Orig_Value& orig_value) const
  {
    typedef typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::orig_to_new_map_type orig_to_new_map_t;
    typename orig_to_new_map_t::const_iterator i = orig_to_new_map_.find(orig_value);
    // FIXME: Is this the expected behavior when the LUT has no new_value
    // for \a orig_value?
    assert(i != orig_to_new_map_.end());
    return i->second;
  }


  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::const_iterator
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::begin() const
  {
    return new_to_orig_map_.begin();
  }

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::iterator
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::begin()
  {
    return new_to_orig_map_.begin();
  }

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::const_iterator
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::end() const
  {
    return new_to_orig_map_.end();
  }

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::iterator
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::end()
  {
    return new_to_orig_map_.end();
  }


  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::const_iterator
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::find(const New_Value& val) const
  {
    return new_to_orig_map_.find(val);
  }

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::iterator
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::find(const New_Value& val)
  {
    return new_to_orig_map_.find(val);
  }


  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  const typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::orig_to_new_map_type&
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::orig_to_new_map() const
  {
    return orig_to_new_map_;
  }

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::orig_to_new_map_type&
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::orig_to_new_map()
  {
    return orig_to_new_map_;
  }

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  const typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::new_to_orig_map_type&
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::new_to_orig_map() const
  {
    return new_to_orig_map_;
  }

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  typename lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::new_to_orig_map_type&
  lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>::new_to_orig_map()
  {
    return new_to_orig_map_;
  }

# endif


  /// Print a look-up table.
  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  std::ostream&
  operator<< (std::ostream& ostr,
	      const lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>& lut);


# ifndef OLN_INCLUDE_ONLY

  template <typename Orig_Value, typename New_Value,
	    typename Orig_Value_Compare, typename New_Value_Compare>
  std::ostream&
  operator<< (std::ostream& ostr,
	      const lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare>& lut)
  {
    typedef lookup_table<Orig_Value, New_Value, Orig_Value_Compare, New_Value_Compare> lut_t;
    const typename lut_t::orig_to_new_map_type& map = lut.orig_to_new_map();
    for (typename lut_t::orig_to_new_map_type::const_iterator i = map.begin ();
	 i != map.end(); ++i)
      ostr << "  " << i->first << " -> " << i->second << std::endl;
    return ostr;
  }

# endif

} // end of namespace oln



#endif // ! OLN_CORE_LOOKUP_TABLE_HH
