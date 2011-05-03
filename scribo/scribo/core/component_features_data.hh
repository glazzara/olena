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

#ifndef SCRIBO_CORE_COMPONENT_FEATURES_DATA_HH
# define SCRIBO_CORE_COMPONENT_FEATURES_DATA_HH

/// \file
///
/// \brief Component features data structure.


# include <scribo/core/def/color_type.hh>

namespace scribo
{

  struct component_features_data
  {
    component_features_data();

    bool valid;
    scribo::def::color_type color;
    float boldness;
  };


  std::ostream&
  operator<<(std::ostream& ostr, const component_features_data& data);

  bool
  operator==(const component_features_data& lhs,
	     const component_features_data& rhs);


# ifndef MLN_INCLUDE_ONLY

  inline
  component_features_data::component_features_data()
    : valid(false)
  {
  }


  inline
  std::ostream&
  operator<<(std::ostream& ostr, const component_features_data& data)
  {
    return ostr << "features["
		<< "valid=" << data.valid
		<< ", color=" << data.color
		<< ", boldness=" << data.boldness
		<< "]" << std::endl;
  }


  bool
  operator==(const component_features_data& lhs,
	     const component_features_data& rhs)
  {
    return
      lhs.valid == rhs.valid
      && lhs.color == rhs.color
      && lhs.boldness == rhs.boldness;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_COMPONENT_FEATURES_DATA_HH
