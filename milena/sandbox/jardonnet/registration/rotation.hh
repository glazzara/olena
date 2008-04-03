#ifndef ROTATION_HH
# define ROTATION_HH

# include <stdlib.h>
//# include "quat.hh"

//# include "mat.hh"

# include <mln/algebra/mat.hh>
# include <mln/algebra/vec.hh>
# include <mln/make/vec.hh>
# include <mln/make/mat.hh>
# include <mln/algebra/quat.hh>

# include "misc.hh"

// FIXME: rotation should be an abstract class
// and derived classes encapsulate either a quaternion or a algebra::matrix
namespace mln
{

  // FIXME : quat is not appriate here
  template <unsigned n>
  algebra::vec<n,float> rotate(const algebra::quat& q, const algebra::vec<n,float>& p)
  {
    return (q * algebra::quat(0. ,p) * q.inv()).v();
  }

  //FIXME : check if correct with n != 3
  template <unsigned n>
  bool check_rotation(const algebra::mat<n,n,float>& mat,
                      const algebra::quat& q)
  {
    assert(q.is_unit());
    algebra::vec<n,float>
      tmp = make::vec(rand(), rand(), rand()),
      p;
    float nl2= norm::l2(tmp);
    if(nl2 != 0)
      p = tmp / nl2;
    algebra::vec<n,float>
      p_rot_1 = rotate(q, p),
      p_rot_2 = mat * p;
    
    return about_equal(norm::l2(p_rot_1 - p_rot_2), 0.f);
  }


  //FIXME : switch to n dim.
  algebra::mat<3,3,float> quat2mat(const algebra::quat& q)
  {
    assert(q.is_unit());
    float
      w = q.to_vec()[0],
      x = q.to_vec()[1],  x2 = 2*x*x,  xw = 2*x*w,
      y = q.to_vec()[2],  y2 = 2*y*y,  xy = 2*x*y,  yw = 2*y*w,
      z = q.to_vec()[3],  z2 = 2*z*z,  xz = 2*x*z,  yz = 2*y*z,  zw = 2*z*w;
    float t[9] = {1.f - y2 - z2,  xy - zw,  xz + yw,
                  xy + zw,  1.f - x2 - z2,  yz - xw,
                  xz - yw,  yz + xw,  1.f - x2 - y2};
    algebra::mat<3,3,float> tmp = make::mat<3,3,9,float>(t);
    // postcondition
    assert(check_rotation(tmp, q));
    return tmp;
  }
}


#endif // ndef ROTATION_HH
