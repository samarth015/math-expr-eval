#include <iostream>
#include "math_expr_eval.cpp"

int main(){
	evalexpr calc ;
	std::string expression;
	getline( std::cin, expression);
	long double result { calc.evaluate(expression) };
	std::cout << result << std::endl;
}
