// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#ifndef SCRIBO_CORE_COMPONENT_INFO_HH
# define SCRIBO_CORE_COMPONENT_INFO_HH

/// \file
///
/// \brief Component information data structure.


# include <mln/core/alias/box2d.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/util/object_id.hh>

# include <scribo/core/component_features_data.hh>
# include <scribo/core/concept/serializable.hh>
# include <scribo/core/tag/component.hh>
# include <scribo/core/tag/line.hh>

namespace scribo
{

  // Forward declarations.
  template <typename L> class component_set;

  typedef mln::util::object_id<scribo::ComponentId, unsigned> component_id_t;


  template <typename L>
  class component_info : public Serializable<component_info<L> >
  {
    typedef mln::util::object_id<scribo::ComponentId, unsigned> component_id_t;

  public:
    component_info();
    component_info(const component_id_t& id,
		   const mln::box2d& bbox,
		   const mln::point2d& mass_center,
		   unsigned card,
		   component::Type type = component::Undefined);

    component_id_t id() const;
    const mln::box2d& bbox() const;
    const mln::point2d& mass_center() const;

    // The number of pixels in this component.
    unsigned card() const;


    bool has_features() const;
    void update_features(const component_features_data& features);

    const component_features_data& features() const;


    component::Tag tag() const;
    void update_tag(component::Tag tag);

    component::Type type() const;
    void update_type(component::Type type);

    bool is_valid() const;

  protected:
    component_id_t id_;
    mln::box2d bbox_;
    mln::point2d mass_center_;
    unsigned card_;

    component_features_data features_;

    component::Tag tag_;
    component::Type type_;
  };



  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const component_info<L>& info);

  template <typename L>
  bool
  operator==(const component_info<L>& lhs, const component_info<L>& rhs);


# ifndef MLN_INCLUDE_ONLY


  template <typename L>
  component_info<L>::component_info()
    : id_(0), tag_(component::Ignored), type_(component::Undefined)
  {

  }


  template <typename L>
  component_info<L>::component_info(const component_id_t& id,
				    const mln::box2d& bbox,
				    const mln::point2d& mass_center,
				    unsigned card,
				    component::Type type)
    : id_(id), bbox_(bbox), mass_center_(mass_center), card_(card),
      type_(type)
  {
    if (!bbox.is_valid())
      tag_ = component::Ignored;
    else
      tag_ = component::None;
  }


  template <typename L>
  typename component_info<L>::component_id_t
  component_info<L>::id() const
  {
    return id_;
  }


  template <typename L>
  const mln::box2d&
  component_info<L>::bbox() const
  {
    return bbox_;
  }


  template <typename L>
  const mln::point2d&
  component_info<L>::mass_center() const
  {
    return mass_center_;
  }

  template <typename L>
  unsigned
  component_info<L>::card() const
  {
    return card_;
  }

  template <typename L>
  bool
  component_info<L>::has_features() const
  {
    return features_.valid;
  }

  template <typename L>
  void
  component_info<L>::update_features(const component_features_data& features)
  {
    features_ = features;
  }

  template <typename L>
  const component_features_data&
  component_info<L>::features() const
  {
    return features_;
  }

  template <typename L>
  component::Tag
  component_info<L>::tag() const
  {
    return tag_;
  }


  template <typename L>
  void
  component_info<L>::update_tag(component::Tag tag)
  {
    tag_ = tag;
  }


  template <typename L>
  component::Type
  component_info<L>::type() const
  {
    return type_;
  }


  template <typename L>
  void
  component_info<L>::update_type(component::Type type)
  {
    type_ = type;
  }


  template <typename L>
  bool
  component_info<L>::is_valid() const
  {
    return tag_ != component::Ignored && bbox_.is_valid();
  }



  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const component_info<L>& info)
  {
    ostr << "component_info("
	 << "id=" << info.id()
	 << ", bbox=" << info.bbox()
	 << ", mass_center=" << info.mass_center()
	 << ", card=" << info.card()
	 << ", tag=" << info.tag();

    if (info.features().valid)
      ostr << ", features=" << info.features();
    else
      ostr << ", features=none";

    ostr << ")" << std::endl;

    return ostr;
  }

  template <typename L>
  bool
  operator==(const component_info<L>& lhs, const component_info<L>& rhs)
  {

    return
      lhs.id() == rhs.id()
      && lhs.bbox() == rhs.bbox()
      && lhs.mass_center() == rhs.mass_center()
      && lhs.card() == rhs.card()
      && lhs.features() == rhs.features()
      && lhs.tag() == rhs.tag()
      && lhs.type() == rhs.type();
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_COMPONENT_INFO_HH
