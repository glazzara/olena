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

  template <typename Exact> class Container;
  template <typename Exact> class Forward_Container;
  template <typename Exact> class Reversible_Container;
  template <typename Exact> class Random_Access_Container;

  //         |
  //         +-- Sequence
  //         |    |
  //         |    +-- Front_Insertion_Sequence
  //         |    |
  //         |    +-- Back_Insertion_Sequence
  //         |

  template <typename Exact> class Sequence;
  template <typename Exact> class Front_Insertion_Sequence;
  template <typename Exact> class Back_Insertion_Sequence;

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

  template <typename Exact> class Associative_Container;
  template <typename Exact> class Simple_Associative_Container;
  template <typename Exact> class Pair_Associative_Container;
  template <typename Exact> class Sorted_Associative_Container;
  template <typename Exact> class Hashed_Associative_Container;
  template <typename Exact> class Unique_Associative_Container;
  template <typename Exact> class Multiple_Associative_Container;



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

  template <typename Exact> class Iterator;
  template <typename Exact> class Trivial_Iterator;
  template <typename Exact> class Input_Iterator;
  template <typename Exact> class Output_Iterator;
  template <typename Exact> class Forward_Iterator;
  template <typename Exact> class Bidirectional_Iterator;
  template <typename Exact> class Random_Access_Iterator;


} // end of namespace abc


#endif // ! MINI_STD_FWDDECLS_HH
