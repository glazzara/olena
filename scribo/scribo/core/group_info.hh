// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_CORE_GROUP_INFO_HH
# define SCRIBO_CORE_GROUP_INFO_HH

/// \file
///
/// \brief Object groups representation.

# include <mln/util/array.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/component_set.hh>

# include <scribo/core/internal/sort_comp_ids.hh>
# include <scribo/core/concept/serializable.hh>


namespace scribo
{

  using namespace mln;

  class group_info
  {
  public:
    group_info();
    group_info(unsigned id, const mln::util::array<component_id_t>& comps,
	       unsigned pixel_area, const box2d& bbox);
    // used for incremental construction (xml loading).
    group_info(unsigned id, unsigned pixel_area, const box2d& bbox,
	       bool valid = false);

    const mln::util::array<component_id_t>& component_ids() const;

    // Used for incremental construction (xml loading)
    mln::util::array<component_id_t>& component_ids_();

    bool is_valid() const;
    void invalidate();

    unsigned card() const;
    unsigned id() const;
    unsigned pixel_area() const;
    const box2d& bbox() const;

  private:
    unsigned id_;
    mln::util::array<component_id_t> comps_;
    bool valid_;
    unsigned pixel_area_;
    box2d bbox_;
  };

  bool operator==(const group_info& lhs, const group_info& rhs);

# ifndef MLN_INCLUDE_ONLY


  group_info::group_info()
    : valid_(false)
  {
  }

  group_info::group_info(unsigned id, const mln::util::array<component_id_t>& comps,
			 unsigned pixel_area, const box2d& bbox)
    : id_(id), comps_(comps), valid_(true),
      pixel_area_(pixel_area), bbox_(bbox)
  {
  }

  group_info::group_info(unsigned id, unsigned pixel_area, const box2d& bbox, bool valid)
    : id_(id), valid_(valid),
      pixel_area_(pixel_area), bbox_(bbox)
  {
  }

  const mln::util::array<component_id_t>&
  group_info::component_ids() const
  {
    return comps_;
  }

  mln::util::array<component_id_t>&
  group_info::component_ids_()
  {
    return comps_;
  }

  bool group_info::is_valid() const
  {
    return valid_;
  }

  void
  group_info::invalidate()
  {
    valid_ = false;
  }

  unsigned
  group_info::card() const
  {
    return comps_.nelements();
  }

  unsigned
  group_info::id() const
  {
    return id_;
  }

  unsigned
  group_info::pixel_area() const
  {
    return pixel_area_;
  }

  const box2d&
  group_info::bbox() const
  {
    return bbox_;
  }

  inline
  std::ostream&
  operator<<(std::ostream& ostr, const group_info& group_info)
  {
    ostr << "group_info[";

    ostr << "id=" << group_info.id() << ", "
	 << "valid=" << group_info.is_valid() << ", "
	 << "pixel_area=" << group_info.pixel_area() << ", "
	 << "bbox=" << group_info.bbox() << ", "
	 << "component_ids=" << group_info.component_ids();

    ostr << "]";

    return ostr;
  }

  inline
  bool
  operator==(const group_info& lhs, const group_info& rhs)
  {
    return
      lhs.id() == rhs.id()
      && lhs.component_ids() == rhs.component_ids()
      && lhs.is_valid() == rhs.is_valid()
      && lhs.pixel_area() == rhs.pixel_area()
      && lhs.bbox() == rhs.bbox();
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_GROUP_INFO_HH
