#ifndef MLN_CORE_CONTRACT_HH
# define MLN_CORE_CONTRACT_HH

/*! \file mln/core/contract.hh
 * This file defines the set of contracts.
 */

# include <cassert>


# define mln_assertion(expr)     assert(expr)

# define mln_invariant(expr)     assert(expr)
# define mln_precondition(expr)  assert(expr)
# define mln_postcondition(expr) assert(expr)

# define mln_implies(lexpr, repxr) assert(not (rexpr) or (lexpr))


#endif // ! MLN_CORE_CONTRACT_HH
