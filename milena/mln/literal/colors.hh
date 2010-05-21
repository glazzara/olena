// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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
    };

    /// Type of literal green.
    struct green_t : public Literal<green_t>
    {
    };

    /// Type of literal blue.
    struct blue_t : public Literal<blue_t>
    {
    };

    /// Type of literal brown.
    struct brown_t : public Literal<brown_t>
    {
    };

    /// Type of literal lime.
    struct lime_t : public Literal<lime_t>
    {
    };

    /// Type of literal orange.
    struct orange_t : public Literal<orange_t>
    {
    };

    /// Type of literal pink.
    struct pink_t : public Literal<pink_t>
    {
    };

    /// Type of literal purple.
    struct purple_t : public Literal<purple_t>
    {
    };

    /// Type of literal teal.
    struct teal_t : public Literal<teal_t>
    {
    };

    /// Type of literal violet.
    struct violet_t : public Literal<violet_t>
    {
    };

    /// Type of literal cyan.
    struct cyan_t : public Literal<cyan_t>
    {
    };

    /// Type of literal magenta.
    struct magenta_t : public Literal<magenta_t>
    {
    };

    /// Type of literal yellow.
    struct yellow_t : public Literal<yellow_t>
    {
    };

    /// Type of literal olive.
    struct olive_t : public Literal<olive_t>
    {
    };

    /// Literal red.
    extern const red_t& red;

    /// Literal green.
    extern const green_t& green;

    /// Literal blue.
    extern const blue_t& blue;

    /// Literal brown.
    extern const brown_t& brown;

    /// Literal lime.
    extern const lime_t& lime;

    /// Literal orange.
    extern const orange_t& orange;

    /// Literal pink.
    extern const pink_t& pink;

    /// Literal purple.
    extern const purple_t& purple;

    /// Literal teal.
    extern const teal_t& teal;

    /// Literal violet.
    extern const violet_t& violet;

    /// Literal cyan.
    extern const cyan_t& cyan;

    /// Literal magenta.
    extern const magenta_t& magenta;

    /// Literal yellow.
    extern const yellow_t& yellow;

    /// Literal olive.
    extern const olive_t& olive;

# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

    const red_t& red = red_t();

    const green_t& green = green_t();

    const blue_t& blue = blue_t();

    const brown_t& brown = brown_t();

    const lime_t& lime = lime_t();

    const orange_t& orange = orange_t();

    const pink_t& pink = pink_t();

    const purple_t& purple = purple_t();

    const teal_t& teal = teal_t();

    const violet_t& violet = violet_t();

    const cyan_t& cyan = cyan_t();

    const magenta_t& magenta = magenta_t();

    const yellow_t& yellow = yellow_t();

    const olive_t& olive = olive_t();

#  endif

# endif

  } // end of namespace mln::literal

} // end of namespace mln

// White and black are color too.
# include <mln/literal/white.hh>
# include <mln/literal/black.hh>

#endif // ! MLN_LITERAL_COLORS_HH
