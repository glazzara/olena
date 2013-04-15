// Copyright (C) 2008, 2009, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_MATH_JACOBI_HH
# define MLN_MATH_JACOBI_HH

/// \file

# include <cmath>

# include <mln/algebra/quat.hh>
# include <mln/algebra/mat.hh>

// from num. rec. in C
// FIXME: what about numrec licence?


namespace mln
{

  namespace math
  {

    /*! \brief Jacobi's method for matrix diagonalization.
      \ingroup mlnmath
    */
    algebra::quat
    jacobi(algebra::mat<4u,4u,float> a);


# ifndef MLN_INCLUDE_ONLY

  // FIXME: nD ?
#define rotateJacobi(a,i,j,k,l) g=a(i,j);h=a(k,l);a(i,j)=g-s*(h+g*tau); \
    a(k,l)=h+s*(g-h*tau);


    inline
    algebra::quat
    jacobi(algebra::mat<4u,4u,float> a)
    {
      float dd, d[4];
      algebra::mat < 4, 4, float >v(literal::zero);
      int j, iq, ip, i = 0;
      float tresh, theta, tau, t, sm, s, h, g, c, b[4], z[4];
      for (ip = 0; ip < 4; ip++)
        {
          for (iq = 0; iq < 4; iq++)
            v(ip, iq) = 0.0f;
          v(ip, ip) = 1.0f;
        }
      for (ip = 0; ip < 4; ip++)
        {
          b[ip] = d[ip] = a(ip, ip);
          z[ip] = 0.0f;
        }
      while (1)
        {
          sm = 0.0f;
          for (ip = 0; ip < 3; ip++)
            {
              for (iq = ip + 1; iq < 4; iq++)
                sm += std::fabs(a(ip, iq));
            }
          if (sm < 1e-12f)
            {                   // 1e-12
              dd = d[0];
              iq = 0;
              for (ip = 1; ip < 4; ip++)
                if (d[ip] > dd)
                  {
                    iq = ip;
                    dd = d[ip];
                  }
              algebra::quat q(v(0, iq), v(1, iq), v(2, iq), v(3, iq));
              q.set_unit();
              return q;
            }
          if (i < 4)
            {
              i++;
              tresh = 0.0125f * sm;
            }
          else
            tresh = 0.0;
          for (ip = 0; ip < 3; ip++)
            {
              for (iq = ip + 1; iq < 4; iq++)
                {
                  g = 100.0f * std::fabs(a(ip, iq));
                  if (i > 4 && (float)(std::fabs(d[ip]) + g) == (float)std::fabs(d[ip])
                      && (float)(std::fabs(d[iq]) + g) == (float)std::fabs(d[iq]))
                    a(ip, iq) = 0.0f;
                  else if (std::fabs(a(ip, iq)) > tresh)
                    {
                      h = d[iq] - d[ip];
                      if ((float)(std::fabs(h) + g) == (float)std::fabs(h)) // unsafe?
                        t = (a(ip, iq)) / h;
                      else
                        {
                          theta = 0.5f * h / (a(ip, iq));
                          t = 1.0f / (std::fabs(theta) + std::sqrt(1.0f +
							      theta * theta));
                          if (theta < 0.0f)
                            t = -t;
                        }
                      c = 1.0f / std::sqrt(1 + t * t);
                      s = t * c;
                      tau = s / (1.0f + c);
                      h = t * a(ip, iq);
                      z[ip] -= h;
                      z[iq] += h;
                      d[ip] -= h;
                      d[iq] += h;
                      a(ip, iq) = 0.0;

		      // DO *NOT* remove these semicolons!!
		      // rotateJacobi is a macro with 4 function calls.
                      for (j = 0; j <= ip - 1; j++)
		      {
                        rotateJacobi(a, j, ip, j, iq);
		      }
                      for (j = ip + 1; j <= iq - 1; j++)
		      {
                        rotateJacobi(a, ip, j, j, iq);
		      }
                      for (j = iq + 1; j < 4; j++)
		      {
                        rotateJacobi(a, ip, j, iq, j);
		      }
                      for (j = 0; j < 4; j++)
		      {
                        rotateJacobi(v, j, ip, j, iq);
		      }
                    }
                }
            }
          for (ip = 0; ip < 4; ip++)
            {
              b[ip] += z[ip];
              d[ip] = b[ip];
              z[ip] = 0.0f;
            }
        }
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace math

} // end of namespace mln


#endif // ! MLN_MATH_JACOBI_HH
