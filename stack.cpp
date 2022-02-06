
//Implementation of stack using linked list.

#ifndef SAMARTH_STACK_FOR_CALC
#define SAMARTH_STACK_FOR_CALC

#include <utility>
#include <stdexcept>

namespace samarth {

	template<typename Object>
		class stack 
		{
			private:
				struct node 
				{
					Object data;
					node* next;

					node() = delete;
					node(const Object &data, node* next): data{data}, next{next} {}
					node(Object &&data, node* next): data{std::move(data)}, next{next} {}
					node(const node& oth) = delete;
					node(node&& oth) noexcept = delete;
					~node() = default;

				};

				void assert_underflow_safe() const {
					if(empty()) 
						throw std::out_of_range { "The stack is empty." };
				}

				std::size_t length;
				node* top_node;

			public:
				stack():length{}, top_node{nullptr} {}

				stack(const stack &oth): stack() {

					Object vals[oth.length];
					node* curr { oth.top_node };
					std::size_t i{};
					while( curr!= nullptr ){
						vals[i++] = curr->data;
						curr = curr->next;
					}

					for(std::size_t i{}; i < oth.length; i++)
						push( vals[ length - 1 - i ] ); 
				}
				stack& operator = ( const stack& oth ){
					stack tmp {oth};
					std::swap(*this,tmp);
					return *this;
				}

				stack(stack &&oth) noexcept:length{oth.length}, top_node{oth.top_node} {
					oth.length = 0;
					oth.top_node = nullptr;
				}

				stack& operator = (stack&& oth) noexcept{
					std::swap( oth.top_node, this->top_node);
					std::swap( oth.length, this->length);
					return *this;
				}

				~stack(){
					clear();
				}

				void clear(){
					node* tmp; 
					while(top_node != nullptr){
						tmp = top_node;
						top_node = top_node->next;
						delete tmp;
					}
					length = 0;
				}

				std::size_t size() const {
					return length;
				}

				bool empty() const {
					return length == 0;
				}

				void push( const Object &val ){
					top_node = new node{ val, top_node };
					length++;
				}
				void push( Object &&val ){
					top_node = new node{ std::move(val), top_node };
					length++;
				}

				Object& top() {
					assert_underflow_safe();
					return top_node->data;
				}

				const Object& top() const {
					assert_underflow_safe();
					return top_node->data;
				}

				void pop() {
					assert_underflow_safe();
					node *tmp {top_node};
					top_node = top_node->next;
					delete tmp;
					length--;
				}

		};
}

#endif // SAMARTH_STACK_FOR_CALC
