#ifndef MLN_CORE_CONTRACT_HH
# define MLN_CORE_CONTRACT_HH

/*! \file mln/core/contract.hh
 *
 * \brief Definition of the set of contracts.
 */

# include <cassert>


/// Assertion.
# define mln_assertion(expr)     assert(expr)

/// Invariant.
# define mln_invariant(expr)     assert(expr)

/// Precondition.
# define mln_precondition(expr)  assert(expr)

/// Postcondition.
# define mln_postcondition(expr) assert(expr)

/// Implication.
# define mln_implies(lexpr, repxr) assert(! (rexpr) || (lexpr))


#endif // ! MLN_CORE_CONTRACT_HH
