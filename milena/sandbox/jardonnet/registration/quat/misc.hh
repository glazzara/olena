#ifndef MISC_HH
# define MISC_HH


inline
float epsilon()
{
  static const float e = 1e-5;
  return e;
}

inline
bool about_equal(float val1, float val2)
{
  return fabs(val1 - val2) < epsilon();
}


#endif // ndef MISC_HH
