
// Container

Container : class =
{
  value_type : type;

  iterator : type;
  const_iterator : type;

  reference : type;
  const_reference : type;

  size_type : type;

  begin : () -> iterator;
  begin : const () -> const_iterator;

  end : () -> iterator;
  end : const () -> const_iterator;

  size : const () -> size_type;
  empty : const () -> bool;
}


// Forward_Container

Forward_Container : class < Container =
{
  "="  : const (rhs : @type) -> bool;
  "!=" : const (rhs : @type) -> bool;
  "<"  : const (rhs : @type) -> bool;
  ">"  : const (rhs : @type) -> bool;
  "<=" : const (rhs : @type) -> bool;
  ">=" : const (rhs : @type) -> bool;
}


// Reversible_Container

Reversible_Container : class < Forward_Container =
{
  reverse_iterator : type;
  const_reverse_iterator : type;

  rbegin : () -> reverse_iterator;
  rbegin : const () -> const_reverse_iterator;

  rend : () -> reverse_iterator;
  rend : const () -> const_reverse_iterator;

  /* Models: vector, list, deque */
}


// Random_Access_Container

Random_Access_Container : class < Reversible_Container =
{
  "[]" : (n : size_type) -> reference {
    precondition { 0 <= n and n <= @size() }
  }
  "[]" : const (n : size_type) -> const_reference {
    precondition { 0 <= n and n <= @size() }
  }
}



// Sequence

Sequence : class < Forward_Container =
{
  make : (n : size_type, t : value_type) {
    precondition  { n >= 0 }
    postcondition { @size() = n } // FIXME: and...
  }

  // FIXME: some ctors are missing.

  front : () -> reference =
    {
      precondition { not @empty() }
      return *(@first());
    }
  front : const () -> const_reference =
    {
      precondition { not @empty() }
      return *(@first());
    }

  insert : (p : iterator, t : value_type) -> iterator;

  erase : (p : iterator) -> iterator;
  erase : (p : iterator, q : iterator) -> iterator;

  clear : () -> void =
    {
      @erase(@begin(), @end());
    }

  /* Models: vector, deque, list */
}



// Front_Insertion_Sequence

Front_Insertion_Sequence : class < Sequence = 
{
  push_front : (t : value_type) -> void =
    {
      @insert(@begin(), t);
    }
  pop_front : () -> void =
    {
      precondition { not @empty() }
      @erase(@begin());
    }
  /* Models: deque, list */
}


// Back_Insertion_Sequence

Back_Insertion_Sequence : class < Sequence =
{
  back : () -> reference =
    {
      precondition { not @empty() }
      return *(--@end());
    }
  push_back : (t : value_type) -> void =
    {
      @insert(@end(), t);
    }
  pop_back : () -> void =
    {
      precondition { not @empty() }
      @erase(--@end());
    }
  /* Models: vector, deque, list */
}



// vector[T]

vector : [T : type] -> class < Back_Insertion_Sequence, Random_Access_Container =
{
  // FIXME: impl
}


// Associative_Container

Associative_Container : class < Forward_Container =
{
  key_type : type;

  erase : (k : key_type) -> size_type;
  erase : (p : iterator) -> void;
  erase : (p : iterator, q : iterator) -> void;

  clear : () -> void =
    {
      @erase(@begin(), @end());
    }

  find : (k : key_type) -> iterator;
  find : const (k : key_type) -> const_iterator;

  count : (k : key_type) -> size_type;

  // FIXME: equal_range

  /* Models: set, multiset, map, multimap */
}


// Simple_Associative_Container

Simple_Associative_Container : class < Associative_Container =
{
  check {
    key_type = value_type and iterator = const_iterator
  }
  /* Models: set, multiset */
}


// Pair_Associative_Container

Pair_Associative_Container : class < Associative_Container =
{
  data_type : type;
  value_type : type = pair[key_type const, data_type];
  /* Models: map, multimap */
}



// Sorted_Associative_Container

Sorted_Associative_Container : class < Associative_Container =
{
  key_compare : type < Strict_Weak_Ordering;
  value_compare : type < Strict_Weak_Ordering;

  // FIXME: ctors, key_comp, value_compare, lower_bound, upper_bound, equal_range
}



// FIXME: Hashed_Associative_Container
// FIXME: Unique_Associative_Container
// FIXME: Multiple_Associative_Container



// Trivial_Iterator

Trivial_Iterator : class =
{
  value_type : type;
  "*" : const () : value_type const&;
  "->" : const () : value_type const&;
  "=" : const (rhs : @type const&) : bool;
}

// Input_Iterator

Input_Iterator : class < Trivial_Iterator =
{
  "++@" : () -> @type &; // pre
  "@++" : () -> @type &; // post
}



// Output_Iterator

Output_Iterator : class < Trivial_Iterator =
{
  "*" : () : value_type &;
  "->" : () : value_type &;
}
