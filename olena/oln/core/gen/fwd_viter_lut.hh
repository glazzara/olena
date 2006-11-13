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

# include <oln/core/abstract/iterator_on_values.hh>
# include <oln/core/lookup_table.hh>

namespace oln
{

  // FIXME: Merge this file with oln/core/internal/fwd_viter_lut.hh?

  // Forward declaration.
  template <typename Lut> class fwd_viter_lut;


  /// Super type declaration.
  template <typename Lut>
  struct set_super_type< fwd_viter_lut<Lut> >
  {
    typedef fwd_viter_lut<Lut> self_t;
    typedef abstract::iterator_on_values<self_t> ret;
  };


  /// Virtual types associated to oln::fwd_viter_lut<Lut>.
  template <typename Lut>
  struct vtypes< fwd_viter_lut<Lut> >
  {
    // The look-up table is immutable.
    typedef const Lut lut_type;
    typedef typename Lut::const_iterator lut_iter_type;
    typedef typename Lut::new_value_type value_type;
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
    typedef oln_type_of(self_t, lut_iter) lut_iter_t;

  public:
    typedef oln_type_of(self_t, lut) lut_type;
    typedef oln_type_of(self_t, value) value_type;

  public:
    // Construct an uninitialized value iterator.
    fwd_viter_lut(const Lut& lut);

    /// Iterator manipulators.
    /// \{
    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    /// \}

    // Get the value pointed by this iterator (const version).
    value_type impl_to_value() const;

    void print(std::ostream& ostr) const;

    protected:
      /// Look-up table.
      lut_type& lut_;
      /// Iterator on \a lut_.
      lut_iter_t i_;
  };


  template <typename Lut>
  std::ostream& operator<<(std::ostream& ostr,
			   const fwd_viter_lut<Lut>& t);



# ifndef OLN_INCLUDE_ONLY

  template <typename Lut>
  fwd_viter_lut<Lut>::fwd_viter_lut(const Lut& lut) :
    super_t(),
    lut_(lut),
    i_()
  {
    // Initialize underlying iterator (i.e., \a i_.)
    this->invalidate();
  }

  template <typename Exact>
  void
  fwd_viter_lut<Exact>::impl_start()
  {
    i_ = lut_.begin();
  }

  template <typename Exact>
  void
  fwd_viter_lut<Exact>::impl_next()
  {
    /* Iterate until a different key is reached. In fact,
       std::multimap might not be the best choice to implement
       new_to_orig_map_.  Maybe a std::map binding orig_val to a
       std::set of new_val's would is better?.  */
    value_type val = i_->first;
    do
      ++i_;
    while (i_ != lut_.end() and i_->first == val);
  }

  template <typename Exact>
  void
  fwd_viter_lut<Exact>::impl_invalidate()
  {
    i_ = lut_.end();
  }

  template <typename Exact>
  bool
  fwd_viter_lut<Exact>::impl_is_valid() const
  {
    return (i_ != lut_.end());
  }

  template <typename Lut>
  typename fwd_viter_lut<Lut>::value_type
  fwd_viter_lut<Lut>::impl_to_value() const
  {
    precondition(this->is_valid());
    return this->i_->first; 
  }

  template <typename Lut>
  void fwd_viter_lut<Lut>::print(std::ostream& ostr) const
  {
    precondition(this->is_valid());
    ostr << "{ value = " << this->i_->first << " }";
  }


  template <typename Lut>
  std::ostream& operator<<(std::ostream& ostr, const fwd_viter_lut<Lut>& t)
  {
    t.print(ostr);
    return ostr;
  }

# endif  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_FWD_VITER_LUT_HH
