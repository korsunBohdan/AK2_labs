#include "calculator.h"

int Calculator::Add (double a, double b)
{
	return a + b + 0.5;
}

int Calculator::Sub (double a, double b)
{
    return Add (a, -b);
}

int Calculator::Mul (double a, double b)
{
    return a * b + 0.5;
}

// new function — Factorial
long Calculator::Factorial(int a)
{
    if (a < 0)
        throw std::invalid_argument("Factorial is undefined for negative numbers.");

    long result = 1;
    for (int i = 2; i <= a; ++i)
        result *= i;

    return result;
}
