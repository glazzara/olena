
namespace abc
{


  Iterator : type =
  {
    value : type
    next : () -> void
    is_valid : const bool
    set : (v : value const&) -> void
  }


  array_iterator : [T: type] -> class =
  {
    value : type = T
    next : () -> void                 = { i_ := i_ + 1 }
    is_valid : const () -> bool       = { return i_ >= 0 and i_ < n_ }
    set : (v : value const&) -> void  = { v_ := v }

    make : (n : int)                  = { i_ := 0; n_ := n }
    protected {
      i_, n_ : int
      v_     : value
    }
  }


  algo : [I : type < Iterator] (iter : I&, val : I.value&) -> void =
  {
    if (iter.is_valid())
      {
	iter.set(val)
	iter.next()
      }
  }


} // abc


main : () -> void =
{
  i   : abc::array_iterator[int] := make(7)
  val : int := 51 

  abc::algo(i, val)
}
