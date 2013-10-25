// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE).
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

/// \file
/// \brief Exercise the mln::interpolated and mln::tr_image morphers
/// together, using a composition of a rotation and a translation.

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/imorph/interpolated.hh>
#include <mln/core/image/imorph/safe.hh>
#include <mln/core/image/imorph/tr_image.hh>

#include <mln/fun/x2v/bilinear.hh>
#include <mln/fun/x2x/rotation.hh>
#include <mln/fun/x2x/translation.hh>
#include <mln/fun/x2x/composed.hh>

#include <mln/math/pi.hh>
#include <mln/math/sqrt.hh>

#include <mln/algebra/vec.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

/// Sugar macro for mln::debug::println.
#define PRINTLN(Ima) mln::debug::println(#Ima " = ", Ima)


int main()
{
  using namespace mln;

  typedef image2d<float> I;
  I ima(4, 4);
  debug::iota(ima);
  PRINTLN(ima);

  // Step-by-step approach.
  {
    /// Wrap `ima' into a safe_image morpher to prevent out-of-domain
    /// accesses due to the interpolation or the rotation below.
    typedef mln::safe_image<I> S;
    S safe(ima, 0.f);
    PRINTLN(safe);

    /// Add an on-the-fly bilinear interpolation.
    typedef interpolated<S, fun::x2v::bilinear> B;
    B inter(safe);
    PRINTLN(inter);

    /// Rotate and translate image on the fly.
    float angle = math::pi / 4;
    typedef fun::x2x::rotation<2, float> F;
    F rotate(angle);
    typedef fun::x2x::translation<2, float> G;
    /* FIXME: Compute these coordinates automatically from IMA's
       domain and ANGLE.  */
    G translate(make::vec(1.5f, 1.5f * (1.f - math::sqrt(2.f))));
    typedef fun::x2x::composed<G, F> H;
    H move(translate, rotate);
    typedef tr_image<mln_domain_(B), B, H> M;
    M moved(inter.domain(), inter, move);
    PRINTLN(moved);
  }

  // Equivalent ``one-liner''.
  {
    /* FIXME: mln/fun/x2x/rotation.hh and mln/fun/x2x/translation.hh
       should provide generators (helpers), as mln/fun/x2x/composed.hh
       does. */
    /* FIXME: We could also add a routine perfoming the centered
       rotation as shortcut.  */
    PRINTLN(apply_tr
            (interpolate<fun::x2v::bilinear>(safe(ima, 0.f).rw()),
             /* FIXME: Compute these coordinates automatically from
                IMA's domain and ANGLE.  */
             compose(fun::x2x::translation<2, float>
                     (make::vec(1.5f,
                                1.5f * (1.f - math::sqrt(2.f)))),
                     fun::x2x::rotation<2, float>(math::pi / 4))));
  }
}
