#ifndef MINI_STD_FWDDECLS_HH
# define MINI_STD_FWDDECLS_HH


namespace abc
{


  //   Container
  //    |
  //    +-- Forward_Container
  //         |
  //         |
  //         +-- Reversible_Container
  //         |    |
  //         |    +-- Random_Access_Container
  //         |

  template <typename E> class Container;
  template <typename E> class Forward_Container;
  template <typename E> class Reversible_Container;
  template <typename E> class Random_Access_Container;

  //         |
  //         +-- Sequence
  //         |    |
  //         |    +-- Front_Insertion_Sequence
  //         |    |
  //         |    +-- Back_Insertion_Sequence
  //         |

  template <typename E> class Sequence;
  template <typename E> class Front_Insertion_Sequence;
  template <typename E> class Back_Insertion_Sequence;

  //         |
  //         +-- Associative_Container
  //              |
  //              +-- Simple_Associative_Container
  //              +-- Pair_Associative_Container
  //              |
  //              +-- Sorted_Associative_Container
  //              +-- Hashed_Associative_Container
  //              |
  //              +-- Unique_Associative_Container
  //              +-- Multiple_Associative_Container

  template <typename E> class Associative_Container;
  template <typename E> class Simple_Associative_Container;
  template <typename E> class Pair_Associative_Container;
  template <typename E> class Sorted_Associative_Container;
  template <typename E> class Hashed_Associative_Container;
  template <typename E> class Unique_Associative_Container;
  template <typename E> class Multiple_Associative_Container;



  //   Iterator
  //    |
  //    +-- Trivial_Iterator
  //    |          |
  //    |          +-- Input_Iterator
  //    |                   |
  //    +-- Output_Iterator |
  //              |         |
  //              +---------+-- Forward_Iterator
  //                             |
  //                             +-- Bidirectional_Iterator
  //                                  |
  //                                  +-- Random_Access_Iterator

  template <typename E> class Iterator;
  template <typename E> class Trivial_Iterator;
  template <typename E> class Input_Iterator;
  template <typename E> class Output_Iterator;
  template <typename E> class Forward_Iterator;
  template <typename E> class Bidirectional_Iterator;
  template <typename E> class Random_Access_Iterator;


} // end of namespace abc


#endif // ! MINI_STD_FWDDECLS_HH
