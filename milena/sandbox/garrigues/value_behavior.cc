# include <mln/value/int_u8.hh>
# include <mln/value/props.hh>

using mln::value::props;

/// Get the iterop type of a value type
# define mln_interop(I) typename mln::value::props<I>::interop

//        value_type    behavior
template<typename V, template<typename> class B >
struct behave : V
{
  typedef B<V> behavior;
  typedef V value;

  behave(const typename behavior::to_compute v)
  {
    this->v_ = behavior::compute(v);
  }

  V&
  operator= (const typename behavior::to_compute& v)
  {
    this->v_ = behavior::compute(v);
    return *this;
  }

  V&
  operator+= (const typename behavior::to_compute& v)
  {
    this->v_ = behavior::compute(this->v_ + v);
    return *this;
  }

  V&
  operator-= (const typename behavior::to_compute& v)
  {
    this->v_ = behavior::compute(this->v_ - v);
    return *this;
  }

};

template<typename V>
struct saturated
{
  typedef V val;

  typedef mln_interop(val) to_compute;

  static val compute(to_compute n)
    {
      if (mln_min(val) > n)
	return mln_min(val);
      if (mln_max(val) < n)
	return mln_max(val);
      return n;
    }

};

// template<typename V>
// struct round
// {
//   typedef V val;
//   // to_compute ? int or float?
//   typedef mln_interop(val) to_compute;
//   static val compute(to_compute n)
//     {
//       if (props<val>::min() > n)
// 	return props<val>::min();
//       if (props<val>::max() < n)
// 	return props<val>::max();
//       return n;
//     }

// };

int main()
{

  typedef behave<mln::value::int_u8, saturated> my_u8_sat;
  typedef behave<mln::value::int_u<3>, saturated> my_u3_sat;

  my_u8_sat i=259;
  std::cout << i << std::endl;
  i += 10;
  std::cout << i << std::endl;

  i -= 5;
  std::cout << i << std::endl;

  i -= 500;
  std::cout << i << std::endl;
  std::cout << std::endl;


  my_u3_sat j = -5000;
  std::cout << j << std::endl;
  j = 2;
  std::cout << j << std::endl;
  j = 3;
  std::cout << j << std::endl;
  j = 11;
  std::cout << j << std::endl;
}
