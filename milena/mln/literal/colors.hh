// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_LITERAL_COLORS_HH
# define MLN_LITERAL_COLORS_HH

/// \file
///
/// Definition of the colors literal.

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal red.
    struct red_t : public Literal<red_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      red_t();
    };

    /// Type of literal green.
    struct green_t : public Literal<green_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      green_t();
    };

    /// Type of literal blue.
    struct blue_t : public Literal<blue_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      blue_t();
    };

    /// Type of literal brown.
    struct brown_t : public Literal<brown_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      brown_t();
    };

    /// Type of literal lime.
    struct lime_t : public Literal<lime_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      lime_t();
    };

    /// Type of literal orange.
    struct orange_t : public Literal<orange_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      orange_t();
    };

    /// Type of literal pink.
    struct pink_t : public Literal<pink_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      pink_t();
    };

    /// Type of literal purple.
    struct purple_t : public Literal<purple_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      purple_t();
    };

    /// Type of literal teal.
    struct teal_t : public Literal<teal_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      teal_t();
    };

    /// Type of literal violet.
    struct violet_t : public Literal<violet_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      violet_t();
    };

    /// Type of literal cyan.
    struct cyan_t : public Literal<cyan_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      cyan_t();
    };

    /// Type of literal magenta.
    struct magenta_t : public Literal<magenta_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      magenta_t();
    };

    /// Type of literal yellow.
    struct yellow_t : public Literal<yellow_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      yellow_t();
    };

    /// Type of literal olive.
    struct olive_t : public Literal<olive_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      olive_t();
    };

    /// Literal red.
    extern const red_t red;

    /// Literal green.
    extern const green_t green;

    /// Literal blue.
    extern const blue_t blue;

    /// Literal brown.
    extern const brown_t brown;

    /// Literal lime.
    extern const lime_t lime;

    /// Literal orange.
    extern const orange_t orange;

    /// Literal pink.
    extern const pink_t pink;

    /// Literal purple.
    extern const purple_t purple;

    /// Literal teal.
    extern const teal_t teal;

    /// Literal violet.
    extern const violet_t violet;

    /// Literal cyan.
    extern const cyan_t cyan;

    /// Literal magenta.
    extern const magenta_t magenta;

    /// Literal yellow.
    extern const yellow_t yellow;

    /// Literal olive.
    extern const olive_t olive;

# ifndef MLN_INCLUDE_ONLY

    inline
    red_t::red_t()
    {
    }

    inline
    green_t::green_t()
    {
    }

    inline
    blue_t::blue_t()
    {
    }

    inline
    brown_t::brown_t()
    {
    }

    inline
    lime_t::lime_t()
    {
    }

    inline
    orange_t::orange_t()
    {
    }

    inline
    pink_t::pink_t()
    {
    }

    inline
    purple_t::purple_t()
    {
    }

    inline
    teal_t::teal_t()
    {
    }

    inline
    violet_t::violet_t()
    {
    }

    inline
    cyan_t::cyan_t()
    {
    }

    inline
    magenta_t::magenta_t()
    {
    }

    inline
    yellow_t::yellow_t()
    {
    }

    inline
    olive_t::olive_t()
    {
    }

#  ifndef MLN_WO_GLOBAL_VARS

    const red_t red;

    const green_t green;

    const blue_t blue;

    const brown_t brown;

    const lime_t lime;

    const orange_t orange;

    const pink_t pink;

    const purple_t purple;

    const teal_t teal;

    const violet_t violet;

    const cyan_t cyan;

    const magenta_t magenta;

    const yellow_t yellow;

    const olive_t olive;

#  endif

# endif

  } // end of namespace mln::literal

} // end of namespace mln

// White and black are color too.
# include <mln/literal/white.hh>
# include <mln/literal/black.hh>

#endif // ! MLN_LITERAL_COLORS_HH
