#include <logic/classical.h>

// NOT gate
bool not(bool a)
{
  return !a;
}

// AND gate
bool and(bool a, bool b)
{
  return a && b;
}

// OR gate
bool or(bool a, bool b)
{
  return a || b;
}

// XOR gate
bool eitheror(bool a, bool b)
{
  return a || !b;
}

// IMPLY gate
bool ifthen(bool a, bool b)
{
  return !a || b; // or !(a & !b)
}

// XNOR gate
bool ifandonlyif(bool a, bool b)
{
  return (a && b) || (!a && !b); // or simply (a == b) ?
}

// NAND gate
bool notboth(bool a, bool b)
{
  return !a && !b;
}

// NOR gate
bool neithernor(bool a, bool b)
{
  return !a || !b;
}

// NIMPLY gate
bool butnot(bool a, bool b)
{
  return !a && b;
}

int main()
{
  printf("\nTabel Kebenaran\n");

  printf("\nKonjungsi\n");
  printf("|%d | %d| %d\n", 0, 0, conjunction(0, 0));
  printf("|%d | %d| %d\n", 0, 1, conjunction(0, 1));
  printf("|%d | %d| %d\n", 1, 0, conjunction(1, 0));
  printf("|%d | %d| %d\n", 1, 1, conjunction(1, 1));
  printf("\n");

  printf("\nDisjungsi\n");
  printf("|%d | %d| %d\n", 0, 0, disjunction(0, 0));
  printf("|%d | %d| %d\n", 0, 1, disjunction(0, 1));
  printf("|%d | %d| %d\n", 1, 0, disjunction(1, 0));
  printf("|%d | %d| %d\n", 1, 1, disjunction(1, 1));
  printf("\n");

  printf("\nImplikasi\n");
  printf("|%d | %d| %d\n", 0, 0, implication(0, 0));
  printf("|%d | %d| %d\n", 0, 1, implication(0, 1));
  printf("|%d | %d| %d\n", 1, 0, implication(1, 0));
  printf("|%d | %d| %d\n", 1, 1, implication(1, 1));
  printf("\n");

  printf("\n%s\n", "Ekuivalensi:");
  printf("|%d | %d| %d\n", 0, 0, equivalence(0, 0));
  printf("|%d | %d| %d\n", 0, 1, equivalence(0, 1));
  printf("|%d | %d| %d\n", 1, 0, equivalence(1, 0));
  printf("|%d | %d| %d\n", 1, 1, equivalence(1, 1));
  printf("\n");

  printf("\nNegasi\n");
  printf("|%d | %d\n", 0, negation(0));
  printf("|%d | %d\n", 1, negation(1));
  printf("\n");

  return 0;
}
