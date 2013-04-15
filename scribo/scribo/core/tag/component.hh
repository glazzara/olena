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

#ifndef SCRIBO_CORE_TAG_COMPONENT_HH
# define SCRIBO_CORE_TAG_COMPONENT_HH

/// \file
///
/// \brief Component tags.

# include <iostream>

namespace scribo
{

  /// Component id tag.
  struct ComponentId;


  namespace component
  {

    /*! \brief All possible tags that can be used for components.

      component::Tag values can be converted to String using
      operator<<(std::ostream &ostr, const Tag &tag).  String values
      can be converted to component::Tag using str2tag().

      \sa scribo::component_set scribo::component_info
     */
    enum Tag
    {
      None = 0, ///< No tag set.
      Ignored   ///< To be ignored in further processing.
    };


    /*! \brief All possible types of components.

      component::Type values can be converted to String using
      operator<<(std::ostream &ostr, const Type &type).  String values
      can be converted to component::Type using str2type().

      \sa scribo::component_set scribo::component_info str2type()
     */
    enum Type
    {
      Undefined = 0,           ///< No type defined.
      Character,               ///< Character.
      VerticalLineSeparator,   ///< Vertical line separator.
      HorizontalLineSeparator, ///< Horizontal line separator.
      WhitespaceSeparator,     ///< whitespace separator.
      Noise,                   ///< Noise.
      Punctuation,             ///< Punctuation.
      Image,                   ///< Image.
      DropCapital              ///< Drop capital.
    };


    /// \brief Operator allowing scribo::component::Tag to be printed
    /// out.
    std::ostream& operator<<(std::ostream& ostr, const Tag& tag);

    /// \brief Convert a std::string to a scribo::component::Tag.
    Tag str2tag(const std::string& str);

    /// \brief Operator allowing scribo::component::Type to be printed
    /// out.
    std::ostream& operator<<(std::ostream& ostr, const Type& type);
    /// \brief Convert a std::string to a scribo::component::Type.
    Type str2type(const std::string& str);


# ifndef MLN_INCLUDE_ONLY


    inline
    std::ostream&
    operator<<(std::ostream& ostr, const Tag& tag)
    {
      std::string str;
      switch(tag)
      {
	default:
	case None:
	  str = "None";
	  break;
	case Ignored:
	  str = "Ignored";
	  break;
      }

      return ostr << str;
    }


    inline
    Tag str2tag(const std::string& str)
    {
      if (str == "Ignored")
	return Ignored;

      return None;
    }


    inline
    std::ostream&
    operator<<(std::ostream& ostr, const Type& type)
    {
      std::string str;
      switch(type)
      {
	default:
	case Undefined:
	  str = "Undefined";
	  break;
	case Character:
	  str = "Character";
	  break;
	case HorizontalLineSeparator:
	  str = "HorizontalLineSeparator";
	  break;
	case VerticalLineSeparator:
	  str = "VerticalLineSeparator";
	  break;
	case WhitespaceSeparator:
	  str = "WhitespaceSeparator";
	  break;
	case Noise:
	  str = "Noise";
	  break;
	case Punctuation:
	  str = "Punctuation";
	  break;
	case Image:
	  str = "Image";
	case DropCapital:
	  str = "DropCapital";
	  break;
      }

      return ostr << str;
    }


    inline
    Type str2type(const std::string& str)
    {
      if (str == "Character")
	return Character;
      else if (str == "HorizontalLineSeparator")
	return HorizontalLineSeparator;
      else if (str == "VerticalLineSeparator")
	return VerticalLineSeparator;
      else if (str == "WhitespaceSeparator")
	return WhitespaceSeparator;
      else if (str == "Noise")
	return Noise;
      else if (str == "Punctuation")
	return Punctuation;
      else if (str == "Image")
	return Image;
      else if (str == "DropCapital")
	return DropCapital;

      return Undefined;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::component

} // end of namespace scribo


#endif // ! SCRIBO_CORE_TAG_COMPONENT_HH
