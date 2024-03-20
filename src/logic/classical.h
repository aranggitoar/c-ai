#ifndef LOGIC_CLASSICAL_H_INCLUDE
#define LOGIC_CLASSICAL_H_INCLUDE

#include <std.h>

bool not(bool a);
bool and(bool a, bool b);
bool or(bool a, bool b);
bool eitheror(bool a, bool b);
bool ifthen(bool a, bool b);
bool ifandonlyif(bool a, bool b);
bool notboth(bool a, bool b);
bool neitheror(bool a, bool b);
bool butnot(bool a, bool b);

#define negation(a) not(a)
#define conjunction(a, b) and(a, b)
#define disjunction(a, b) or(a, b)
#define non_equivalence(a, b) eitheror(a, b) // Alias exclusive disjunction.
// Add converse implication? If yes, implication is material implication.
#define implication(a, b) ifthen(a, b)
#define equivalence(a, b) ifandonlyif(a, b) // Alias biconditional.
#define non_conjunction(a, b) notboth(a, b) // Alias alternative denial.
#define non_disjunction(a, b) neithernor(a, b) // Alias joint denial.
#define non_implication(a, b) butnot(a, b) // Alias abjunction

#endif /* LOGIC_CLASSICAL_H_INCLUDE */
