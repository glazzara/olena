#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/metal/vec.hh>

namespace mln
{
  namespace value
  {
    template <unsigned n>
    struct hsi
    {
    public:
      /// Constructor without argument.
      hsi<n>();
      
      /// Constructor from component values.
      hsi<n>(int h, int s, int i);
    
    private:
      int h_;
      int s_;
      int i_;
    };

    template <unsigned n>
    inline
    hsi::hsi<n>()
      :h_(0), s_(0), i_(0)
    {
    }

    template <unsigned n>
    inline
    hsi::hsi<n>(int h, int s, int i)
    {
      mln_precondition(h >= 0);
      mln_precondition(s >= 0);
      mln_precondition(i >= 0);
      this->h_ = h;
      this->s_ = s;
      this->i_ = i;
    }
  }
}
