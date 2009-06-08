//						       		-*- C++ -*-
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file pgm.ixx
/// \brief A wrapper of mln::io::pgm::load and mln::io::pgm::save

%module pgm

%include std_string.i

%{
#include "mln/io/pgm/load.hh"
#include "mln/io/pgm/save.hh"
%}

// Wrap mln::io::pgm::load(const std::string&) by hand, to force swig
// to choose this overloading.
namespace mln
{
  namespace io
  {
    namespace pgm
    {

      template <typename V>
      mln::image2d<V> load(const std::string& filename);

    } // end of namespace mln::io::pgm

  } // end of namespace mln::io

} // end of namespace mln

%include "mln/io/pgm/save.hh"
