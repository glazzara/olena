#ifndef INTERPOL_HH
# define INTERPOL_HH

# include <vector>
# include "misc.hh"

# include <mln/value/quat.hh>

// misc

namespace mln
{

  inline
  value::quat slerp_2(const value::quat& p, const value::quat& q, float h)
  {
    assert(interpol_ok(p, q, h));
    value::quat tmp = p * pow(p.conj() * q, h);
    assert(about_equal(tmp, slerp(p, q, h)));
    return tmp;
  }

  inline
  value::quat slerp_3(const value::quat& p, const value::quat& q, float h)
  {
    assert(interpol_ok(p, q, h));
    value::quat tmp = pow(p * q.conj(), 1 - h) * q;
    assert(about_equal(tmp, slerp(p, q, h)));
    return tmp;
  }

  inline
  value::quat slerp_4(const value::quat& p, const value::quat& q, float h)
  {
    assert(interpol_ok(p, q, h));
    value::quat tmp = pow(q * p.conj(), h) * p;
    assert(about_equal(tmp, slerp(p, q, h)));
    return tmp;
  }

  inline
  value::quat slerp_5(const value::quat& p, const value::quat& q, float h)
  {
    assert(interpol_ok(p, q, h));
    value::quat tmp = q * pow(q.conj() * p, 1 - h);
    assert(about_equal(tmp, slerp(p, q, h)));
    return tmp;
  }


  // Spherical Spline Value::Quaternion Interpolation

  inline
  value::quat squad(const std::vector<value::quat>& q,
                    const std::vector<value::quat>& s,
                    unsigned i,
                    float h)
  {
    assert(i < q.size() && s.size() == q.size());
    return slerp(slerp(q[i], q[i+1], h),
                 slerp(s[i], s[i+1], h),
                 2 * h * (1 - h));
  }


  inline
  value::quat s_from_q(const std::vector<value::quat>& q, unsigned i)
  {
    assert(i < q.size());
    assert(q[i].is_unit());
    if (i == 0 || i == q.size() - 1)
      return q[i];
    assert(q[i-1].is_unit() && q[i+1].is_unit());
    return q[i] * exp(-(log(q[i].inv() * q[i+1]) + log(q[i].inv() * q[i-1]))
                      / 4);
  }


  inline
  std::vector<value::quat> do_squad(const std::vector<value::quat>& q,
                                    unsigned n)
  {
    assert(n > 1);
    std::vector<value::quat> s(q.size()), res;

    for (unsigned i = 0; i < q.size(); ++i)
      s[i] = s_from_q(q, i);

    res.push_back(q[0]);
    for (unsigned i = 0; i < q.size() - 1; ++i)
      {
        for (unsigned j = 1; j < n; ++j)
          res.push_back(squad(q, s, i, float(j) / n));
        res.push_back(q[i + 1]);
      }
    return res;
  }
}

#endif // ndef INTERPOL_HH
