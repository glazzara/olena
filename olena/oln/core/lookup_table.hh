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

  template <typename Key, typename Data>
  class lookup_table
  { 
    typedef lookup_table<Key, Data> self_t;

  public:
    typedef Key key_type;
    typedef Data data_type;
    typedef std::map<Key, Data> map_type;

  public:
    lookup_table();

    self_t& add (const key_type& k, const data_type& d);

    const data_type operator () (const key_type& key) const;

    const map_type& map() const;

  private:
    std::map<Key, Data> map_;
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename Key, typename Data>
  lookup_table<Key, Data>::lookup_table() :
    map_()
  {
  }

  template <typename Key, typename Data>
  lookup_table<Key, Data>& 
  lookup_table<Key, Data>::add (const Key& k, const Data& d)
  {
    map_.insert(std::make_pair(k, d));
    return *this;
  }

  template <typename Key, typename Data>
  const Data
  lookup_table<Key, Data>::operator () (const Key& key) const
  {
    typedef typename lookup_table<Key, Data>::map_type map_t;
    typename map_t::const_iterator i = map_.find(key);
    // FIXME: Is this the expected behavior when the LUT has no data
    // for \a key.
    assert(i != map_.end());
    return i->second;
  }

  template <typename Key, typename Data>
  const typename lookup_table<Key, Data>::map_type&
  lookup_table<Key, Data>::map() const
  {
    return map_;
  }

# endif


  /// Print a look-up table.
  template <typename Key, typename Data>
  std::ostream&
  operator<< (std::ostream& ostr, const lookup_table<Key, Data>& lut);


# ifndef OLN_INCLUDE_ONLY

  template <typename Key, typename Data>
  std::ostream&
  operator<< (std::ostream& ostr, const lookup_table<Key, Data>& lut)
  {
    typedef lookup_table<Key, Data> lut_t;
    const typename lut_t::map_type& map = lut.map();
    for (typename lut_t::map_type::const_iterator i = map.begin ();
	 i != map.end(); ++i)
      ostr << "  " << i->first << " -> " << i->second << std::endl;
    return ostr;
  }

# endif

} // end of namespace oln



#endif // ! OLN_CORE_LOOKUP_TABLE_HH
