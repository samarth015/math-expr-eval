# Mathematical Expression Evaluator
A rudimentary expression evaluator which I wrote for fun while I was learning about the applications of stacks. Implemented a simple command line calculator using it. It can also be used in a GUI calculator.

###Example
```c++
#include <iostream>
#include "math_expr_eval.cpp"

int main(){

	evalexpr calc ;
	std::string expression = "18-[6-{4-(8-(6+3))}]";  
	auto result = calc.evaluate(expression);

	std::cout << "Result is --> " << result << std::endl;

}
```
> Result is --> 17
