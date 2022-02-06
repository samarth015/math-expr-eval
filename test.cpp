#include "math_expr_eval.cpp"
#include <array>
#include <iostream>

int main(){

	const std::size_t N{ 12 };    //Number of test cases.

	std::array<const char*,N>tests = {
		"1+ 2 + 3 "            	,            
		"1+3 + 7 * (9 - 10)"    ,   
		"10 + 10 * 10 / 10"     ,   
		"18-[6-{4-(8-(6+3))}]"  ,   
		"3*(4*5^2) / 6+7-8"     ,   
		"5^2"                   ,
		"10*-20"                ,
		"-(-1)"                 ,
		"2 ^ 2 ^ 3"             ,
		"6 / 2(1+2)"            ,
		"9 - 3 / 1 / 3 + 1"     ,
		"-10/(20/2^2*5/5)*8-2"  ,
	};

	std::array<const long double,N> answers = { 6, -3, 20, 17, 49, 25, -200, 1, 256, 9, 9, -18 };

	evalexpr calc ;
	std::string expression;

	std::cout << std::boolalpha ;
	for( size_t i{}; i < tests.size() ; i++ ) {
		auto result { calc.evaluate(tests[i]) };
		std::cout << (result == answers[i] ? "Passed" : "Failed" ) << " : " 
				  << tests[i] << "  =  " << result <<  std::endl;
	}

	std::array<const char*,4> fail_tests = {
	   "90 + 100 + 3 *(20+3))"     ,
	   "(())"                      ,
	   "100-"                      ,
	   "10-*10"
	   };

	std::cout << "\nTesting wrong expressions.\n";
		for( size_t i{}; i < fail_tests.size() ; i++ ) {
			bool detects_invalid_syntax {true};
			try{
				calc.evaluate(fail_tests[i]);
				detects_invalid_syntax = false;
			}
			catch(std::exception& e){
				std::cout << e.what() << " : ";
			}
			std::cout << (detects_invalid_syntax ? "Passed" : "Failed" ) 
					  << " : " << fail_tests[i] << std::endl;
		}
}
