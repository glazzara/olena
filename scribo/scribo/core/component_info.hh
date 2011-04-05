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

  typedef mln::util::object_id<scribo::ComponentId, unsigned> component_id_t;

  class component_info : public Serializable<component_info>
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



  std::ostream&
  operator<<(std::ostream& ostr, const component_info& info);


# ifndef MLN_INCLUDE_ONLY


  inline
  component_info::component_info()
    : id_(0), tag_(component::Ignored), type_(component::Undefined)
  {

  }


  inline
  component_info::component_info(const component_id_t& id,
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


  inline
  component_info::component_id_t
  component_info::id() const
  {
    return id_;
  }


  inline
  const mln::box2d&
  component_info::bbox() const
  {
    return bbox_;
  }


  inline
  const mln::point2d&
  component_info::mass_center() const
  {
    return mass_center_;
  }

  inline
  unsigned
  component_info::card() const
  {
    return card_;
  }

  inline
  bool
  component_info::has_features() const
  {
    return features_.valid;
  }

  inline
  void
  component_info::update_features(const component_features_data& features)
  {
    features_ = features;
  }

  inline
  const component_features_data&
  component_info::features() const
  {
    return features_;
  }

  inline
  component::Tag
  component_info::tag() const
  {
    return tag_;
  }


  inline
  void
  component_info::update_tag(component::Tag tag)
  {
    tag_ = tag;
  }


  inline
  component::Type
  component_info::type() const
  {
    return type_;
  }


  inline
  void
  component_info::update_type(component::Type type)
  {
    type_ = type;
  }


  inline
  bool
  component_info::is_valid() const
  {
    return tag_ != component::Ignored && bbox_.is_valid();
  }


  inline
  std::ostream&
  operator<<(std::ostream& ostr, const component_info& info)
  {
    return ostr << "component_info("
		<< "id=" << info.id()
		<< ", bbox=" << info.bbox()
		<< ", mass_center=" << info.mass_center()
		<< ", card=" << info.card()
		<< ", tag=" << info.tag()
		<< ", features=" << info.features()
		<< ")" << std::endl;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_COMPONENT_INFO_HH
