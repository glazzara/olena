// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_CORE_TAG_LINE_HH
# define SCRIBO_CORE_TAG_LINE_HH

# include <iostream>

/// \file
///
/// \brief Line tags.

namespace scribo
{

  // Line id tag.
  struct LineId;


  namespace line
  {

    enum Tag
    {
      None = 0,
      Ignored,
      Needs_Precise_Stats_Update,
      Merged,
      Pathological
    };



    // The following next enumerations are based on the page content
    // XML format.
    //
    // XSD:
    // http://schema.primaresearch.org/PAGE/gts/pagecontent/2009-03-16/pagecontent.xsd

    enum ReadingDirection
    {
      BottomToTop,
      LeftToRight,
      RightToLeft,
      TopToBottom
    };


    enum Type
    {
      Caption,
      Credit,
      DropCapital,
      Floating,
      Footer,
      Header,
      Heading,
      PageNumber,
      Paragraph,

      // These types are not supported by the XSD.
      Punctuation,
      Text,
      Undefined
    };


    std::ostream&
    operator<<(std::ostream& ostr, const Tag& tag);

    std::ostream&
    operator<<(std::ostream& ostr, const ReadingDirection& direction);

    std::ostream&
    operator<<(std::ostream& ostr, const Type& type);
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
	case Needs_Precise_Stats_Update:
	  str = "Needs_Precise_Stats_Update";
	  break;
	case Merged:
	  str = "Merged";
	  break;
	case Pathological:
	  str = "Pathological";
	  break;
      }

      return ostr << str;
    }



    inline
    std::ostream&
    operator<<(std::ostream& ostr, const ReadingDirection& direction)
    {
      std::string str;
      switch(direction)
      {
	case BottomToTop:
	  str = "bottom-to-top";
	  break;
	default:
	case LeftToRight:
	  str = "left-to-right";
	  break;
	case RightToLeft:
	  str = "right-to-left";
	  break;
	case TopToBottom:
	  str = "top-to-bottom";
	  break;
      }

      return ostr << str;
    }


    inline
    std::string type2str(const Type& type)
    {
      std::string str;
      switch(type)
      {
	case Caption:
	  str = "caption";
	  break;
	case Credit:
	  str = "credit";
	  break;
	case DropCapital:
	  str = "drop-capital";
	  break;
	case Floating:
	  str = "floating";
	  break;
	case Footer:
	  str = "footer";
	  break;
	case Header:
	  str = "header";
	  break;
	case Heading:
	  str = "heading";
	  break;
	case PageNumber:
	  str = "page-number";
	  break;
	case Paragraph:
	  str = "paragraph";
	  break;

	// Values unsupported by the XSD
	case Punctuation:
	  str = "punctuation";
	  break;
	case Text:
	  str = "text";
	  break;
	default:
	case Undefined:
	  str = "undefined";
	  break;
      }

      return str;
    }

    inline
    std::ostream&
    operator<<(std::ostream& ostr, const Type& type)
    {
      return ostr << type2str(type);
    }


    inline
    Type str2type(const std::string& str)
    {
      if (str == "caption")
	return Caption;
      else if (str == "credit")
	return Credit;
      else if (str == "drop-capital")
	return DropCapital;
      else if (str == "floating")
	return Floating;
      else if (str == "footer")
	return Footer;
      else if (str == "header")
	return Header;
      else if (str == "heading")
	return Heading;
      else if (str == "page-number")
	return PageNumber;
      else if (str == "paragraph")
	return Paragraph;

      // Values unsupported by the XSD
      else if(str == "punctuation")
	return Punctuation;
      else if (str == "text")
	return Text;

      return Undefined;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::line

} // end of namespace scribo


#endif // ! SCRIBO_CORE_TAG_COMP_HH
