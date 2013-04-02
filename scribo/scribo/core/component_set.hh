// Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
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

#ifndef SCRIBO_CORE_COMPONENT_SET_HH
# define SCRIBO_CORE_COMPONENT_SET_HH

/// \file
///
/// \brief Definition of a component set.
///
/// FIXME: component_set should always set a component type in order
/// to be fully supported by visitors.

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/function.hh>

# include <mln/data/fill.hh>

# include <mln/util/array.hh>
# include <mln/value/next.hh>

# include <mln/accu/pair.hh>
# include <mln/accu/center.hh>
# include <mln/accu/shape/bbox.hh>

# include <mln/labeling/compute.hh>
# include <mln/labeling/relabel.hh>

# include <mln/logical/or.hh>

# include <mln/convert/from_to.hh>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/pw/all.hh>

# include <mln/core/routine/duplicate.hh>

# include <mln/value/next.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/component_info.hh>
# include <scribo/core/concept/serializable.hh>


namespace scribo
{

  // Forward declaration.
  template <typename L> struct component_set;


  namespace internal
  {
    /*! \internal \brief Data structure for \c scribo::component_set.

      \ingroup grpstruct
     */
    template <typename L>
    struct component_set_data
    {
      typedef mln::accu::center<mln_site(L)> center_accu_t;
      typedef mln::accu::shape::bbox<mln_site(L)> bbox_accu_t;
      typedef mln::accu::pair<bbox_accu_t, center_accu_t> pair_accu_t;

      typedef std::pair<mln_box(L),
			std::pair<mln_site(L), unsigned> > pair_data_t;

      typedef mln_result(center_accu_t) center_t;

      component_set_data();
      component_set_data(const L& ima, const mln_value(L)& ncomps);
      component_set_data(const L& ima, const mln_value(L)& ncomps,
			 const mln::util::array<pair_accu_t>& attribs,
			 component::Type type = component::Undefined);
      component_set_data(const L& ima, const mln_value(L)& ncomps,
			 const mln::util::array<pair_data_t>& attribs,
			 component::Type type = component::Undefined);

      component_set_data(const L& ima, const mln_value(L)& ncomps,
			 const mln::util::array<scribo::component_info<L> >& infos);

      void fill_infos(const mln::util::array<pair_accu_t>& attribs,
		      component::Type type = component::Undefined);

      void fill_infos(const mln::util::array<pair_data_t>& attribs,
		      component::Type type = component::Undefined);

      // Useful while constructing incrementaly (XML loading).
      void soft_init(const mln_value(L) ncomps);

      L ima_;
      mln_value(L) ncomps_;
      mln::util::array<scribo::component_info<L> > infos_;

      mln_ch_value(L, bool) separators_;
    };

  } // end of namespace scribo::internal


  /*! \brief Represents all the components in a document image.

    \ingroup grpstruct
  */
  template <typename L>
  class component_set : public Serializable<component_set<L> >
  {
    typedef mln::accu::shape::bbox<mln_site(L)> bbox_accu_t;
    typedef mln::accu::center<mln_site(L)> center_accu_t;
    typedef mln::accu::pair<bbox_accu_t, center_accu_t> pair_accu_t;

    typedef std::pair<mln_box(L),
		      std::pair<mln_site(L), unsigned> > pair_data_t;

    typedef mln_result(center_accu_t) center_t;

    typedef internal::component_set_data<L> data_t;
    typedef data_t* id_t;

  public:

    // Constructors

    /// Default.
    component_set();

    /// Constructor from internal data.
    component_set(const mln::util::tracked_ptr<data_t>& data);

    /// Constructor from an image \p ima and the number of labels \p
    /// ncomps.
    component_set(const L& ima, const mln_value(L)& ncomps);

    /// Constructor from an image \p ima, the number of labels \p
    /// ncomps and attributes values (bounding box and mass center).
    component_set(const L& ima, const mln_value(L)& ncomps,
		  const mln::util::array<pair_accu_t>& attribs,
		  component::Type type = component::Undefined);

    /// Constructor from an image \p ima, the number of labels \p
    /// ncomps and attributes values (bounding box and mass center).
    component_set(const L& ima, const mln_value(L)& ncomps,
		  const mln::util::array<pair_data_t>& attribs,
		  component::Type type = component::Undefined);

    // Misc Members

    /// Return the component count.
    mln_value(L) nelements() const;

    /// Return component information for a given component id \p id.
    const component_info<L>& info(const mln_value(L)& id) const;

    /// Return component information for a given component id \p id.
    component_info<L>& info(const mln_value(L)& id);

    /// Return component information for a given component id \p id.
    component_info<L>& operator()(const component_id_t& id);

    /// Return component information for a given component id \p id.
    const component_info<L>& operator()(const component_id_t& id) const;


    /// Update component tag for which \p f returns 'False', with \p
    /// tag.
    template <typename F>
    void update_tags(const mln::Function_v2b<F>& f, component::Tag tag);

    /// Create a copy of this component_set<L>
    component_set<L> duplicate() const;

    /// Return the underlying labeled image
    const L& labeled_image() const;

    /// A component_set is considered as valid if it has been
    /// initialized (i.e. not instantiated with the default
    /// constructor).
    bool is_valid() const;


    // Separators components related routines.

    /// Return true if an image of separator exists.
    bool has_separators() const;

    /// Add a new separator binary image or perform a logical OR with
    /// the existing one.
    void add_separators(const mln_ch_value(L, bool)& ima);

    /// Return the binary image of separators.
    const mln_ch_value(L, bool)& separators() const;

    /// Clear separator image.
    void clear_separators();


    // Internal methods

    /// \internal
    /// Return all the component infos.
    //
    const mln::util::array<scribo::component_info<L> >& infos_() const;

    /// \internal
    /// Unique set Id.
    //
    id_t id_() const;


    /// \internal
    /// Read/Write access to the underlying labeled image.
    ///
    /// \warning Write in this image at your own risks! It may lead to
    /// non-synchronised related data.
    //
    L& labeled_image_();

    /// \internal
    /// Force replacing the underlying labeled image.  \warning Use it
    /// at your own risks... This image may not
    ///
    /// reflect the component information stored in this
    /// component_set.
    //
    void update_labeled_image_(const L& lbl);

    /// \internal
    /// Return the underlying labeled image where invalid components
    /// have been erased.
    ///
    /// \warning this image is computed on the fly...!
    //
    mln_concrete(L) valid_comps_image_() const;


  protected:
    /// Duplicate the underlying image and create a new component_set.
    void init_(const component_set<L>& model);

    mln::util::tracked_ptr<data_t> data_;
  };


  template <typename L>
  bool
  operator==(const component_set<L>& lhs, const component_set<L>& rhs);


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const component_set<L>& info);




# ifndef MLN_INCLUDE_ONLY


  // component_set_data<L> >

  namespace internal
  {

    // data< component_set<L> >


    template <typename L>
    inline
    component_set_data<L>::component_set_data()
    {
    }


    template <typename L>
    inline
    component_set_data<L>::component_set_data(const L& ima,
					      const mln_value(L)& ncomps)
      : ima_(ima), ncomps_(ncomps)
    {
    }


    template <typename L>
    inline
    void
    component_set_data<L>::soft_init(const mln_value(L) ncomps)
    {
      mln_precondition(infos_.nelements() == 0);

      ncomps_ = ncomps;
      infos_.reserve(ncomps_);
      infos_.append(component_info<L>()); // Component 0, i.e. the background.
    }


  } // end of namespace mln::internal




  template <typename L>
  inline
  component_set<L>::component_set()
  {
  }

  template <typename L>
  inline
  component_set<L>::component_set(const mln::util::tracked_ptr<data_t>& data)
  {
    data_ = data;
  }

  template <typename L>
  inline
  component_set<L>::component_set(const L& ima, const mln_value(L)& ncomps)
  {
    data_ = new internal::component_set_data<L>(ima, ncomps);

    initialize(data_->separators_, ima);  // FIXME: to be removed
    mln::data::fill(data_->separators_, false);

    typedef mln::accu::shape::bbox<mln_site(L)> bbox_accu_t;
    typedef mln::accu::center<mln_site(L)> center_accu_t;
    typedef mln::accu::pair<bbox_accu_t, center_accu_t> pair_accu_t;


    mln::util::array<pair_accu_t> attribs;
    mln::labeling::compute(attribs, ima, ncomps);


    typedef mln_site(L) P;

    data_->infos_.reserve(mln::value::next(ncomps));

    data_->infos_.append(component_info<L>()); // Component 0, i.e. the background.
    for_all_comp_data(i, attribs)
    {
      component_info<L> info(i, attribs[i].first(),
			     attribs[i].second(), attribs[i].second_accu().nsites());
      data_->infos_.append(info);
    }
  }


  template <typename L>
  inline
  component_set<L>::component_set(const L& ima, const mln_value(L)& ncomps,
				  const mln::util::array<pair_accu_t>& attribs,
				  component::Type type)
  {
    data_ = new internal::component_set_data<L>(ima, ncomps);

    initialize(data_->separators_, ima);  // FIXME: to be removed
    mln::data::fill(data_->separators_, false);

    typedef mln_site(L) P;

    data_->infos_.reserve(mln::value::next(ncomps));

    data_->infos_.append(component_info<L>()); // Component 0, i.e. the background.
    for_all_comp_data(i, attribs)
    {
      component_info<L> info(i, attribs[i].first(),
			     attribs[i].second(), attribs[i].second_accu().nsites(),
			     type);
      data_->infos_.append(info);
    }
  }


  template <typename L>
  inline

  component_set<L>::component_set(const L& ima, const mln_value(L)& ncomps,
				  const mln::util::array<pair_data_t>& attribs,
				  component::Type type)
  {
    data_ = new internal::component_set_data<L>(ima, ncomps);

    initialize(data_->separators_, ima);  // FIXME: to be removed
    mln::data::fill(data_->separators_, false);

    typedef mln_site(L) P;

    data_->infos_.reserve(mln::value::next(ncomps));

    data_->infos_.append(component_info<L>()); // Component 0, i.e. the background.
    for_all_comp_data(i, attribs)
    {
      component_info<L> info(i, attribs[i].first,
			     attribs[i].second.first, attribs[i].second.second,
			     type);
      data_->infos_.append(info);
    }
  }


  template <typename L>
  inline

  mln_value(L)
  component_set<L>::nelements() const
  {
    return data_->ncomps_;
  }

  template <typename L>
  inline
  const component_info<L>&
  component_set<L>::info(const mln_value(L)& id) const
  {
    return data_->infos_[id];
  }

  template <typename L>
  inline
  component_info<L>&
  component_set<L>::info(const mln_value(L)& id)
  {
    return data_->infos_[id];
  }

  template <typename L>
  inline
  const component_info<L>&
  component_set<L>::operator()(const component_id_t& id) const
  {
    return data_->infos_[id];
  }

  template <typename L>
  inline
  component_info<L>&
  component_set<L>::operator()(const component_id_t& id)
  {
    return data_->infos_[id];
  }


  template <typename L>
  template <typename F>
  inline
  void
  component_set<L>::update_tags(const mln::Function_v2b<F>& f_,
				component::Tag tag)
  {
    const F& f = exact(f_);

    for_all_comp_data(i, data_->infos_)
      if (!f(i))
	data_->infos_[i].update_tag(tag);
  }


  template <typename L>
  inline
  component_set<L>
  component_set<L>::duplicate() const
  {
    component_set<L> output;
    output.init_(*this);
    return output;
  }



  template <typename L>
  inline
  const L&
  component_set<L>::labeled_image() const
  {
    return data_->ima_;
  }


  template <typename L>
  inline
  void
  component_set<L>::update_labeled_image_(const L& lbl)
  {
    data_->ima_ = lbl;
  }


  template <typename L>
  inline
  bool
  component_set<L>::is_valid() const
  {
    return data_ != 0 && data_->ima_.is_valid();
  }


  template <typename L>
  inline
  typename component_set<L>::id_t
  component_set<L>::id_() const
  {
    return data_.ptr_;
  }


  template <typename L>
  inline
  L&
  component_set<L>::labeled_image_()
  {
    return data_->ima_;
  }


  template <typename L>
  inline
  mln_concrete(L)
  component_set<L>::valid_comps_image_() const
  {
    mln::util::array<bool> f(mln::value::next(data_->ncomps_));
    f(0) = true;

    for_all_comps(c, (*this))
      f(c) = (*this)(c).is_valid();

    mln_value(L) new_ncomps;
    mln_concrete(L)
      output = mln::labeling::relabel(data_->ima_,
				      data_->ncomps_,
				      new_ncomps,
				      f);

    return output;
  }


  template <typename L>
  inline
  bool
  component_set<L>::has_separators() const
  {
    return data_->separators_.is_valid();
  }


  template <typename L>
  inline
  void
  component_set<L>::add_separators(const mln_ch_value(L, bool)& ima)
  {
    if (! has_separators())
      data_->separators_ = ima;
    else
      mln::logical::or_inplace(data_->separators_, ima);
  }


  template <typename L>
  inline
  const mln_ch_value(L, bool)&
  component_set<L>::separators() const
  {
    return data_->separators_;
  }


  template <typename L>
  inline
  void
  component_set<L>::clear_separators()
  {
    data_->separators_.destroy();
  }


  template <typename L>
  inline
  const mln::util::array<scribo::component_info<L> >&
  component_set<L>::infos_() const
  {
    return data_->infos_;
  }

  template <typename L>
  inline
  void
  component_set<L>::init_(const component_set<L>& set)
  {
    data_ = new internal::component_set_data<L>();
    data_->ima_ = mln::duplicate(set.labeled_image());
    data_->ncomps_ = set.nelements();
    data_->infos_ = set.infos_();
    data_->separators_ = set.separators();
  }


  template <typename L>
  bool
  operator==(const component_set<L>& lhs, const component_set<L>& rhs)
  {
    if (lhs.id_() == rhs.id_())
      return true;

    if (! (lhs.labeled_image() == rhs.labeled_image()))
      return false;

    if (! (lhs.separators() == rhs.separators()))
      return false;

    if (! (lhs.nelements() == rhs.nelements()
	   && lhs.labeled_image() == rhs.labeled_image()
	   && lhs.separators() == rhs.separators()))
      return false;

    for_all_comps(c, lhs)
      if (! (lhs(c) == rhs(c)))
	return false;

    return true;
  }

  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const component_set<L>& info)
  {
    ostr << "component_set[" << std::endl;
    for_all_comps(i, info)
      ostr << info(i);
    ostr << "]" << std::endl;

    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_COMPONENT_SET_HH
