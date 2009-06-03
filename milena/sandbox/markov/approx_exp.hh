// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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



#ifndef MLN_APPROX_EXP_HH
# define MLN_APPROX_EXP_HH

class approx_exp
{
public:

  approx_exp(float min, float max, unsigned nvalues)
    : min_(min),
      max_(max),
      nvalues_(nvalues),
      values_(nvalues),
      step_((max - min) / float(nvalues))
  {

    for (unsigned i = 0; i < nvalues_; ++i)
      values_[i] = exp(min_ + i * step_);
  }

  inline
  float
  get(float x)
  {
     if (x < min_)
       return values_[0];
     if (x > max_)
       return values_[nvalues_ - 1];

    return values_[unsigned(float(x - min_) / step_)];
  }

  inline
  float
  operator() (float x)
  {
    return get(x);
  }

private:
  const float    min_;
  const float    max_;
  const unsigned nvalues_;
  const float step_;

  std::vector<float> values_;
};

#endif // ! MLN_APPROX_EXP_HH
