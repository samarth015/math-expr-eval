#ifndef SAMARTH_SIMPLE_CALC
#define SAMARTH_SIMPLE_CALC

#include "stack.cpp"
#include "algorithm"
#include <cmath>
#include <string>
#include <string_view>
#include <stdexcept>

class evalexpr
{
	private:
		std::string expr;

		bool isdigit(const char ch) const {
			return '0' <= ch and ch <= '9';
		}

		bool symbols_are_balanced(){
			samarth::stack<char> st ;
			for( const char ch : expr ){
				if( is_opening_bracket( ch )){
					st.push( ch );
				}
				else if( is_closing_bracket( ch ) ) {
					if( !st.empty() and st.top() == corresponding_opening_bracket(ch) ) st.pop(); 
					else return false;
				}
			}
			return st.empty();
		}

		bool first_and_last_token_is_valid() const{
			const char first { expr.front() }, second{*(++expr.begin())}, last{ expr.back() } , second_last {*(expr.end()-2)};	

			bool first_is_valid = ( first == '-' 
								  or first == '+' 
								  or is_opening_bracket(first) 
								  or isdigit(first) 
								  or (first == '.' and isdigit(second)) ); 

			bool last_is_valid = ( (isdigit(last) and !is_opening_bracket(second_last))
								 or (is_closing_bracket(last) and !(is_opening_bracket(second_last) or is_operator(second_last)) )
								 or (last == '.' and isdigit(second_last)) );

			return first_is_valid and last_is_valid;
		}

		bool expression_syntax_is_correct() const {

			if(expr.size() == 1) return isdigit(expr[0]);

			const char first { expr.front() }, last{ expr.back() };

			if( ! first_and_last_token_is_valid()) return false;

			for( size_t i{1}; i < expr.size() - 1 ; i++){ 
				const char curr{expr[i]}, prev{expr[i-1]}, next{expr[i+1]};

				// dot cannot be followed or preceded by another dot and must have a digit to either its left or right
				bool dot_incorrect             = isdot(curr) 
									             and (!(isdigit(prev) or isdigit(next)) or isdot(prev) or isdot(next)) ;

				// () [] is incorrect so closing will not be preceded by opening and cannot be preceded by any operator. -) is wrong.
				bool closing_bracket_incorrect = is_closing_bracket(curr) 
										         and (is_opening_bracket(prev) or is_operator(prev));

				// minus cannot be preceded by itself or the ^(power) sign.
				bool minus_sign_incorrect      = curr == '-' 
										         and (prev == '^' or prev == '-');

				// Any non minus operator cannot be preceded by any other operator.
				bool non_minus_sign_incorrect  = (is_operator(curr) and curr!='-') 
												 and (is_operator(prev));

				if( dot_incorrect or closing_bracket_incorrect or minus_sign_incorrect or non_minus_sign_incorrect ) 
					return false;
			}

			return true;
		}

		bool expression_composed_of_valid_tokens() const{
			for(const char ch: expr){
				if(!(isdigit_or_dot(ch) or is_operator(ch) or is_opening_bracket(ch) or is_closing_bracket(ch)))
					return false;
			}
			return true;
		}
			
		void assert_expression_not_empty() const {
			if( expr.empty() ) throw std::invalid_argument { "Empty expression." };
		}

		void assert_expression_is_valid(){
			if( !expression_composed_of_valid_tokens() ) throw std::invalid_argument { "Expression uses invalid characters." };
			if( !symbols_are_balanced() ) throw std::invalid_argument { "Brackets are not balanced" };
			if( !expression_syntax_is_correct() ) throw std::invalid_argument { "Expression syntax incorrect" };
		}

		bool isdot( const char ch) const {
			return ch == '.';
		}

		bool isdigit_or_dot( const char ch ) const {
			return isdigit(ch) or ch =='.';
		}

		void format() {

			std::string formatted_expr {""};

			if( expr[0] == '-' or expr[0] == '+' ) formatted_expr.push_back('0');
			formatted_expr.push_back(expr[0]);

			for( size_t i{1}; i < expr.size() ; i++){

				const char curr{expr[i]}, prev {expr[i-1]};

				if( curr == '-' ) {
					if( is_opening_bracket( prev ) or prev == '+' )
						formatted_expr.append( "0-" );
					else if( prev == '*' or prev == '/' )
						formatted_expr.append( "(0-1)*" );
					else 
						formatted_expr.push_back('-');
				}
				else if( curr == '+' and is_opening_bracket(prev) ) {
					formatted_expr.append( "0+" );
				}
				//If curr is a ( preceded by a number's char or a ) or if curr is a number's char preceded by a ) 
				else if ((is_opening_bracket(curr) and ( isdigit_or_dot(prev) or is_closing_bracket(prev) )) or ( isdigit_or_dot(curr) and is_closing_bracket(prev) )){
					formatted_expr.push_back( '*' );
					formatted_expr.push_back( curr );
				}
				else {
					formatted_expr.push_back( curr );
				}
			}
			expr = std::move(formatted_expr);
		}

		bool is_opening_bracket( char bkt ) const {
			return bkt == '(' or bkt == '{' or bkt == '[' ;
		}

		bool is_closing_bracket( char bkt ) const {
			return bkt == ')' or bkt == '}' or bkt == ']' ;
		}

		bool is_operator( char symbol ) const {
			static std::string opers { "+-^*/" };
			return opers.find( symbol ) != std::string::npos ;
		}

		bool is_operator( std::string_view term ) const {
			return term.size() == 1 and is_operator(term[0]);
		}

		char corresponding_opening_bracket( char closing_bkt ) const {
			switch(closing_bkt){
				case ']':
					return '[';
				case '}':
					return '{';
				case ')':
					return '(';
			}
			throw std::invalid_argument { "In corresponding_opening_bracket. The arguments is not a closing bracket."};
		}

		std::uint8_t get_priority( const char oper ) const {
			if( is_opening_bracket( oper) ) return 0;
			switch(oper){
				case '+':
				case '-':
					return 1;
				case '*':
				case '/':
					return 2;
				case '^':
					return 3;
			}
			throw std::invalid_argument {"In get_priority. Invalid operator used as argument."};
		}

		void add_stacktop_to_postfix_expression( samarth::stack<char> &opers, std::string &postfix_expr){
			postfix_expr.push_back(opers.top());
			postfix_expr.push_back(' ');
			opers.pop();
		}

		void add_term_to_postfix_expression( std::string &term, std::string &postfix_expr ){
			if( term.empty() ) return;
			postfix_expr.append(term);
			postfix_expr.push_back(' ');
			term.clear();
		}

		bool both_are_power_opers( const char oper1, const char oper2 ){
			return oper1 == '^' and oper2 == '^';
		}


		void infix_to_postix_convert(){

			std::string postfix_expr {};
			samarth::stack<char> opers;
			std::string term {};

			for( const char ch : expr ){

				if(is_opening_bracket(ch)){
					add_term_to_postfix_expression( term, postfix_expr);
					opers.push( ch );
				}

				else if(  is_operator(ch) ) {
					add_term_to_postfix_expression( term, postfix_expr);

					while(!opers.empty() and get_priority(ch) <= get_priority(opers.top()) and !both_are_power_opers(ch, opers.top())){
						add_stacktop_to_postfix_expression( opers, postfix_expr); 
					}

					opers.push( ch );
				}



				else if(is_closing_bracket(ch)) {
					add_term_to_postfix_expression( term, postfix_expr);

					while( !is_opening_bracket(opers.top()) ){
						add_stacktop_to_postfix_expression( opers, postfix_expr); 
					}

					opers.pop();
				}
				else {
					term.push_back(ch);
				}
			}
			if(!term.empty()){ 
				add_term_to_postfix_expression( term, postfix_expr);
			}
			while( !opers.empty() ){
				add_stacktop_to_postfix_expression( opers, postfix_expr); 
			}	
			expr = postfix_expr;
		}


		long double eval_binary_operation( long double left_op, char Operator, long double right_op ) {
			switch(Operator){
				case '+':
					return left_op + right_op;
				case '-':
					return left_op - right_op;
				case '*':
					return left_op * right_op;
				case '/':
					return left_op / right_op;
				case '^':
					return std::pow(left_op, right_op);
			}
			throw std::invalid_argument {"Invalid Operator used for eval_binary_operation"};
		}

		long double evaluate_postfix_expression(){
			samarth::stack<long double> st {};
			std::string term;

			for( const char ch : expr ){
				if( ch == ' '  ){
					if( is_operator( term ) ){

						char Operator { term[0] };
						long double right_op{ st.top() }; 
						st.pop();
						long double left_op{ st.top() }; 
						st.pop();
						long double result { eval_binary_operation( left_op, Operator, right_op)} ;

						st.push( result );
					}
					else {
						st.push( std::stold(term) );
					}

					term.clear();
				}
				else{
					term.push_back(ch);
				}
			}

			return st.top();
		}

		void preprocessing() {
			auto it { std::remove_if( expr.begin(), expr.end(), [](auto ch){return ch ==' ';} ) } ;
			expr.erase( it, expr.end() );
		}

	public:
		long double evaluate( std::string expression ) {
			expr = std::move(expression);

			assert_expression_not_empty();
			preprocessing();
			assert_expression_is_valid();
			format();
			infix_to_postix_convert();
			auto res =  evaluate_postfix_expression();

			return res;
		}
};

#endif  //SAMARTH_SIMPLE_CALC
