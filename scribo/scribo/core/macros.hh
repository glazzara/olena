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

#ifndef SCRIBO_CORE_MACROS_HH
# define SCRIBO_CORE_MACROS_HH

# define for_all_ncomponents(C, NCOMP) \
  for (unsigned C = 1; C <= NCOMP; ++C)

# define for_all_elements(E, S) \
  for (unsigned E = 0; E < S.nelements(); ++E)


// FIXME: we want to replace previous macros by these ones.
# define for_all_comps(C, S) \
  for (unsigned C = 1; C <= S.nelements(); ++C)

# define for_all_comp_data(E, S) \
  for (unsigned E = 1; E < S.nelements(); ++E)

# define for_all_links(E, S) \
  for_all_comp_data(E, S)

# define for_all_groups(E, S) \
  for_all_comp_data(E, S)

# define for_all_lines(E, S) \
  for_all_comps(E, S)

# define for_all_paragraphs(E, S) \
  for_all_comps(E, S)

# define for_all_line_comps(E, S) \
  for_all_elements(E, S)

# define for_all_lines_info(E, S) \
  for_all_comp_data(E, S)

# define for_all_anchors(E, S) \
  for_all_elements(E, S)

#endif // ! SCRIBO_CORE_MACROS_HH
