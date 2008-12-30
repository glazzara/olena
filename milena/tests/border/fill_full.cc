// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file tests/border/fill_full.cc
///
/// Tests on mln::border::fill.
///
/// \todo Rewrite this non-sense test file!!!

#include <mln/border/fill.hh>
#include <mln/data/fill.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/rgb16.hh>
#include <mln/value/float01_8.hh>
#include <mln/value/float01_16.hh>
#include <mln/debug/println_with_border.hh>

using namespace mln;


template <typename T>
void
check1d(unsigned row, unsigned border, T& value, T& v)
{
  image1d<T> ima(row, border);
  data::fill (ima, v);
  border::fill (ima, value);

  unsigned i = 0;
  for(i = 0; i < border; ++i)
    mln_assertion (ima.element(i) == value);
  unsigned bo = border + row;
  for(; i < bo; ++i)
    mln_assertion (ima.element(i) == v);
  bo += border;
  for(; i < bo; ++i)
    mln_assertion (ima.element(i) == value);
}

template <typename T>
void
check2d(unsigned row, unsigned col, unsigned border, T& value, T& v)
{
  image2d<T> ima(row, col, border);
  data::fill (ima, v);
  border::fill (ima, value);

  unsigned c = col + 2 * border;
  unsigned r = row + 2 * border;
  unsigned bo = c * border + border;
  unsigned i = 0;
  unsigned u = col + border;
  unsigned ww = r * c;

  for(i = 0; i < bo; ++i)
    mln_assertion (ima.element(i) == value);
  bo += c * row;
  for(; i < bo; ++i)
    {
      unsigned cur = i % c;
      if (cur < border || cur >= u)
	mln_assertion (ima.element(i) == value);
      else
	mln_assertion (ima.element(i) == v);
    }
  for(; i < ww; ++i)
    mln_assertion (ima.element(i) == value);
}

template <typename T>
void
check3d(unsigned sli, unsigned row, unsigned col, unsigned border, T& value, T& v)
{
  image3d<T> ima(sli, row, col, border);
  data::fill (ima, v);
  border::fill (ima, value);

  unsigned c = col + 2 * border;
  unsigned r = row + 2 * border;
  unsigned bo = c * border + border;
  unsigned i = 0;
  unsigned u = col + border;
  unsigned ww = r * c;

  for(i = 0; i < bo; ++i)
    mln_assertion (ima.element(i) == value);
  bo += c * row;
  for(; i < bo; ++i)
    {
      unsigned cur = i % c;
      if (cur < border || cur >= u)
	mln_assertion (ima.element(i) == value);
      else
	mln_assertion (ima.element(i) == v);
    }
  for(; i < ww; ++i)
    mln_assertion (ima.element(i) == value);
}


int
main (void)
{
  int limits = 10;

  {
    typedef int T;
    T value = (T) -1;
    T v = 42;

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }

  {
    typedef unsigned T;
    T value = (T) -1;
    T v = 42;

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }

  {
    typedef value::int_u8 T;
    T value = 255;
    T v = 42;

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }

  {
    typedef value::int_u16 T;
    T value = 65535;
    T v = 42;

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }

  {
    typedef value::int_s8 T;
    T value = 127;
    T v = 42;

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }

  {
    typedef value::int_s16 T;
    T value = 32767;
    T v = 42;

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }

  {
    typedef value::rgb8 T;
    T value = T(255, 255, 255);
    T v = T(42, 0, 0);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }

  {
    typedef value::rgb16 T;
    T value = T(65535, 65535, 65535);
    T v = T(42, 0, 0);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }


  {
    typedef value::float01_8 T;
    T value = static_cast<T>(0.9999f);
    T v = static_cast<T>(0.111f);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }

  {
    typedef value::float01_16 T;
    T value = static_cast<T>(0.9999f);
    T v = static_cast<T>(0.111f);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	    check1d(i, j, value, v);

    for (int i = 1; i < limits; ++i)
      for (int j = 1; j < limits; ++j)
	for (int k = 1; k < limits; ++k)
	    check2d(i, j, k, value, v);
  }


}
