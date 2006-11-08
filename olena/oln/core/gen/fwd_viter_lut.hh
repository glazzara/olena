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

#ifndef OLN_CORE_GEN_FWD_VITER_LUT_HH
# define OLN_CORE_GEN_FWD_VITER_LUT_HH

# include <oln/core/internal/fwd_viter_lut.hh>
# include <oln/core/lookup_table.hh>

namespace oln
{

  /*------------.
  | fwd_viter.  |
  `------------*/

  // Forward declaration.
  template <typename Lut> class fwd_viter_lut;


  /// Super type declaration.
  template <typename Lut>
  struct set_super_type< fwd_viter_lut<Lut> >
  {
    typedef fwd_viter_lut<Lut> self_t;
    typedef internal::fwd_viter_lut<self_t> ret;
  };


  /// Virtual types associated to oln::fwd_viter_lut<Lut>.
  template <typename Lut>
  struct vtypes< fwd_viter_lut<Lut> >
  {
    // The look-up table is immutable.
    typedef const Lut lut_type;
    typedef typename Lut::const_iterator lut_iter_type;
    typedef typename Lut::new_value_type rvalue_type;
  };


  /// Const forward value iterator on look-up table.
  template <typename Lut>
  class fwd_viter_lut :
    public stc_get_supers(fwd_viter_lut<Lut>)
    // Check that Lut is a look-up table.
    // FIXME: an abstract::lookup_table would be useful.
  {
    typedef fwd_viter_lut<Lut> self_t;
    typedef stc_get_super(self_t) super_t;

  public:
    typedef oln_type_of(self_t, lut) lut_t;
    typedef oln_type_of(self_t, rvalue) rvalue_type;

  public:
    // Construct an uninitialized value iterator.
    fwd_viter_lut(const Lut& lut);
    // Construct an iterator pointing to value \a val.
    fwd_viter_lut(const Lut& lut, const rvalue_type& val);

    // Get the value pointed by this iterator (const version).
    operator rvalue_type() const;

    void print(std::ostream& ostr) const;
  };


  template <typename Lut>
  std::ostream& operator<<(std::ostream& ostr,
			   const fwd_viter_lut<Lut>& t);



# ifndef OLN_INCLUDE_ONLY

  template <typename Lut>
  fwd_viter_lut<Lut>::fwd_viter_lut(const Lut& lut)
    : super_t(lut)
  {
    // Initialize underlying iterator (i.e., \a i_.)
    this->invalidate();
  }

  template <typename Lut>
  fwd_viter_lut<Lut>::fwd_viter_lut(const Lut& lut,
				    const typename fwd_viter_lut<Lut>::rvalue_type& val)
    : super_t(lut, val)
  {
    // The underlying iterator is initialized by the super class.
  }

  template <typename Lut>
  fwd_viter_lut<Lut>::operator typename fwd_viter_lut<Lut>::rvalue_type() const
  {
    precondition(this->is_valid());
    return this->i_->first;
  }

  template <typename Lut>
  void fwd_viter_lut<Lut>::print(std::ostream& ostr) const
  {
    precondition(this->is_valid());
    ostr << "fwd_viter_lut<Lut> { value = " << this->i_->first << " }";
  }


  template <typename Lut>
  std::ostream& operator<<(std::ostream& ostr,
			   const fwd_viter_lut<Lut>& t)
  {
    t.print(ostr);
    return ostr;
  }

# endif  



  /*--------------------.
  | mutable_fwd_viter.  |
  `--------------------*/

  // Forward declaration.
  template <typename Lut> class mutable_fwd_viter_lut;


  /// Super type declaration.
  template <typename Lut>
  struct set_super_type< mutable_fwd_viter_lut<Lut> >
  {
    typedef mutable_fwd_viter_lut<Lut> self_t;
    typedef internal::fwd_viter_lut<self_t> ret;
  };


  /// Virtual types associated to oln::mutable_fwd_viter_lut<Lut>.
  template <typename Lut>
  struct vtypes< mutable_fwd_viter_lut<Lut> >
  {
    typedef Lut lut_type;
    typedef typename Lut::iterator lut_iter_type;
    typedef typename Lut::new_value_type rvalue_type;
    typedef typename Lut::new_value_type& lvalue_type;
  };


  /// Const forward value iterator on look-up table.
  template <typename Lut>
  class mutable_fwd_viter_lut :
    public stc_get_supers(mutable_fwd_viter_lut<Lut>)
    // Check that Lut is a look-up table.
    // FIXME: an abstract::lookup_table would be useful.
  {
    typedef mutable_fwd_viter_lut<Lut> self_t;
    typedef stc_get_super(self_t) super_t;
    typedef oln_type_of(self_t, lut_iter) lut_iter_t;
    typedef typename Lut::orig_value_type orig_value_t;
    typedef typename Lut::new_value_type new_value_t;

  public:
    typedef Lut lut_t;
    typedef oln_type_of(self_t, rvalue) rvalue_type;
    typedef oln_type_of(self_t, lvalue) lvalue_type;

    self_t& operator=(const rvalue_type& rhs);

  public:
    // Construct an uninitialized value iterator.
    mutable_fwd_viter_lut(Lut& lut);
    // Construct an iterator pointing to value \a val.
    mutable_fwd_viter_lut(Lut& lut, const rvalue_type& val);

    // Get the value pointed by this iterator (const version).
    operator rvalue_type() const;
    // Get the value pointed by this iterator (mutable version).
    operator lvalue_type();

    void print(std::ostream& ostr) const;
  };


  template <typename Lut>
  std::ostream& operator<<(std::ostream& ostr,
			   const mutable_fwd_viter_lut<Lut>& t);



# ifndef OLN_INCLUDE_ONLY

  template <typename Lut>
  mutable_fwd_viter_lut<Lut>::mutable_fwd_viter_lut(Lut& lut)
    : super_t(lut)
  {
    // Initialize underlying iterator (i.e., \a i_.)
    this->invalidate();
  }

  template <typename Lut>
  mutable_fwd_viter_lut<Lut>::mutable_fwd_viter_lut(Lut& lut,
						    const typename mutable_fwd_viter_lut<Lut>::rvalue_type& val)
    : super_t(lut, val)
  {
    // The underlying iterator is initialized by the super class.
  }

  template <typename Lut>
  mutable_fwd_viter_lut<Lut>::operator typename mutable_fwd_viter_lut<Lut>::rvalue_type() const
  {
    precondition(this->is_valid());
    return this->i_->second;
  }

  template <typename Lut>
  mutable_fwd_viter_lut<Lut>::operator typename mutable_fwd_viter_lut<Lut>::lvalue_type() 
  {
    precondition(this->is_valid());
    return this->i_->first;
  }

  template <typename Lut>
  mutable_fwd_viter_lut<Lut>&
  mutable_fwd_viter_lut<Lut>::operator=(const typename mutable_fwd_viter_lut<Lut>::rvalue_type& rhs)
  {
    precondition(this->is_valid());

    // Current ``new'' value.
    new_value_t cur_new_val = this->i_->first;

    // Shortcuts.
    typedef lut_iter_t new_iter_t;
    typedef std::pair<new_iter_t, new_iter_t> new_iter_range_t;

    // Current ``original'' value(s) associated to the next ``new'' value RHS.
    new_iter_range_t cur_orig_val_range =
      this->lut_.new_to_orig_map().equal_range(cur_new_val);
    // A temporary map where additional bindings will be stored.
    typename Lut::new_to_orig_map_type additional_bindings;

    for (new_iter_t j = cur_orig_val_range.first;
	 j != cur_orig_val_range.second; ++j)
      {
	// Update the orig-to-new map (substitution).
	orig_value_t orig_val = j->second;
	this->lut_.orig_to_new_map()[orig_val] = rhs;
	// Populate the temp map.
	additional_bindings.insert(std::make_pair(rhs, orig_val));
      }
    // Update the new-to-orig map (deletion).
    this->lut_.new_to_orig_map().erase(cur_orig_val_range.first,
				       cur_orig_val_range.second);
    // Update the new-to-orig map (insertion).
    this->lut_.new_to_orig_map().insert(additional_bindings.begin(),
					      additional_bindings.end());
    return *this;
  }

  template <typename Lut>
  void mutable_fwd_viter_lut<Lut>::print(std::ostream& ostr) const
  {
    precondition(this->is_valid());
    ostr
      << "mutable_fwd_viter_lut<Lut> { value = " << this->i_->first << " }";
  }


  template <typename Lut>
  std::ostream& operator<<(std::ostream& ostr,
			   const mutable_fwd_viter_lut<Lut>& t)
  {
    t.print(ostr);
    return ostr;
  }

# endif  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_FWD_VITER_LUT_HH
