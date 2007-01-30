
// Point

Point : class =
{
}

point2d : class < Point =
{
  row : int;
  col : int;
}

point3d : class < Point =
{
  row : int;
  col : int;
  sli : int;
}


// Iterator

Iterator : class =
{
  point_t : typedef < Point;

  start : () -> void;
  next  : () -> void;
  is_valid : const () -> bool;

  auto : -> point_t;
}



// Image

Image : class =
{
  {
    point_t < Point;
    iter_t  < Iterator;
    value_t;
  }
    : typedef;

  "()" : const (p : point_t const&) -> value_t;
  has : const (p : point_t const&) -> bool;
}


// Image = Image_2D | Image_3D

Image_2D : class < Image =
{
  "()" : const (row : int, col : int) -> value_t;
  nrows : const () -> int;
  ncols : const () -> int;
}

Image_3D : class < Image =
{
  "()" : const (row : int, col : int, sli : int) -> value_t;
  nrows : const () -> int;
  ncols : const () -> int;
  nslis : const () -> int;
}


// Image = Image_with_Nbh | not

Image_with_Nbh : class < Image
{
  {
    niter_t < Iterator;
    nbh_t < Neighborhood;
  }
    : typedef;
  nbh : const () -> nbh_t;
}



// plugs !

image_dim_plug : [I : type] -> type = {
  switch (I.point_t) {
    case point2d: return Image_2D;
    case point3d: return Image_3D;
  }
}

image_nbh_plug : [I : type] -> type = {
  if (I.nbh_t)
    return Image_with_Nbh;
  else
    return Image;
}



// image_entry

image_base : class < image_dim_plug[@],
		     image_nbh_plug[@] =
{
  {
    point_t;
    iter_t;
    value_t;
  }
    : typedef;

  "()" : const (p : point_t const&) -> value_t;
  has : const (p : point_t const&) -> bool;
}



// iterator2d

iterator2d : class < Iterator =
{
  point_t : typedef := point2d;
  
  make : (ima : Image_2D) = {
    nrows := ima.nrows;
    ncols := ima.ncols;
  }

  start : () -> void = {
    p.row := 0;
    p.col := 0;
  }

  next  : () -> void = {
    p.col := p.col + 1;
    if (p.col = ncols) {
      p.col := 0;
      p.row := p.row + 1;
    }
  }

  is_valid : const () -> bool = {
    return p.row < p.nrows;
  }

  auto : -> point_t = 
  {
    return p;
  }

  protected {
    nrows : int;
    ncols : int;
    p : point_t;
  } 
}



// image2d[T]

image2d : [T : class] -> class < image_base =
{
  {
    point_t := point2d;
    iter_t  := iterator2d;
    value_t := T;
  }
    : typedef;

  make : (nrows : int, ncols : int) =
  {
    data.make(nrows * ncols);
  }
  
  "()" : const (p : point_t const&) -> value_t =
  {
    precondition { has(p) = true }
    return data.element(p.row * ncols + p.col);
  }

  "()" : const (row : int, col : int) -> value_t =
  {
    p : point2d;
    p.make(row, col);
    return @[p];
  }

  has : const (p : point_t const&) -> bool =
  {
    precondition { data != 0 }
    return p.row >= 0 and p.row < nrows and p.col >= 0 and p.col < ncols;
  }

  data  : vector[T];
  nrows : int;
  ncols : int;
}



// image_morpher

image_morpher : class < image_base =
{
  delegatee_t : typedef;
}



// fun

fun_ : [I : class < Image, F : type] where (F.arg_t = I.value_t) -> class =
{
  {
    delegatee_t := I;
    value_t := F.res_t;
  }
    : typedef;
 
  ima : I;
  f : F;

  "()" : const (p : point_t const&) -> value_t =
  {
    precondition { has(p) = true }
    return f(ima(p));
  }
}

fun1 : class =
{
  {
    arg_t;
    res_t;
  }
    : typedef;

  "()" : (ima : I < Image) -> fun_[I,@] =
  {
    fun_[I,@] tmp;    // '@' is exact type
    tmp.make(ima, @); // here '@' is *this
    return tmp;
  }
}

pow : class < fun1 =
{
  n : unsigned;

  {
    arg_t := double;
    res_t := double;
  }
    : typedef;
  
  "()" : (x : double) -> double =
  {
    return std::pow(x, n);
  }
}


// stack

vec : [n : unsigned, T : type] -> class =
{
  data : T[n];

  "[]" : (i : unsigned) -> T =
  {
    precondition { i < n }
    return data[i];
  }
}

stack_ : [n : unsigned, I : class < Image] -> class =
{
  {
    delegatee_t := I;
    value_t := vec[n, I.value_t];
  }
    : typedef;
 
  ima : vec[n,I];

  "()" : const (p : point_t const&) -> value_t =
  {
    precondition { has(p) = true }
    value_t tmp;
    for (unsigned i := 0; i < n; ++i)
      tmp[i] := ima[i](p);
    return tmp;
  }
}

stack : [I : class < Image] (ima_0 : I const&, ima_1 : I const&) -> stack_[2,I] =
{
  stack_[2,I] tmp;
  tmp.ima[0] := ima_0;
  tmp.ima[1] := ima_1;
  return tmp;
}



// "image + neighborhood"

plus : [I : class < Image, N : type] where N < Neighborhood
-> class < image_morpher =
{
  {
    delegatee_t := I;
  }
    : typedef;

  ima : I;
  nbh : N;
}


// operator plus

"+" : [I : class < Image, N : class < Neighborhood] (ima : I, nbh : N) -> plus[I, N] =
{
  plus[I, N] tmp;
  return tmp.make(ima, nbh);
}




// ----------------------------------------------------------------------




// "image + neighborhood"

plus : [I : class < Image, N : type] where N < Neighborhood
-> class < image_morpher =
{
  {
    delegatee_t := I;
  }
    : typedef;

  ima : I;
  nbh : N;
}


// operator plus

"+" : [I : class < Image,
       T : type] (lhs : I, rhs : T) -> plus[I, T] =
{
  plus[I, T] ima;
  return ima.make(lhs, rhs);
}




// unsub

unsubset : (ima : Image_with_Subset) = { FIXME }



// predicate

predicate : [I : class < Image] -> type = {
  return (I.point_t const &) -> I.value_t;
}



// sub[I,P]

sub : [I, P] where I < Iterator and P = predicate[I]
-> class < Iterator =
{
  point_t : typedef := I.point_t;

  iter : I;
  pred : P;
  p : point_t const&;

  make : (ima : Image) {
    iter.make(unsubset(ima));
    pred := ima.pred();
    p := iter.p;
  }

  invariant {
    iter.is_valid() => pred(p);
  }

  start : () -> void = {
    iter.start();
    while (iter.is_valid() and not pred(p))
      iter.next();
  }

  next : () -> void = {
    iter.next();
    while (iter.is_valid() and not pred(p))
      iter.next();
  }

  is_valid : const () -> bool = {
    return iter.is_valid();
  }

  auto : -> point_t = 
  {
    return p;
  }
}



// 'pipe' is "image | predicate -> sub-image"

pipe : [I : class < Image, P : type] where P = predicate[I]
-> class < image_morpher =
{
  {
    delegatee_t := I;
    iter_t : sub[I.iter_t, P];
  }
    : typedef;

  pred : P;

  unsubset : () -> I =
  {
    return ref_ima;
  }

  has : const (p : point_t const&) -> bool =
  {
    return ref_ima.has(p) and pred(p);
  }

}



// operator pipe

"|" : [I : class < Image,
       T : type] (lhs : I, rhs : T) -> pipe[I, T] =
{
  pipe[I, T] ima;
  return ima.make(lhs, rhs);
}
