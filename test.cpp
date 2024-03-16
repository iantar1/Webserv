


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <fstream>


class calculator
{
	private:
		typedef int (calculator::*operation)(int, int);
		std::map<char, operation> ops;
		int add(int a, int b)
		{
			return a + b;
		}
		int subtract(int a, int b)
		{
			return a - b;
		}
		int multiply(int a, int b)
		{
			return a * b;
		}
		int divide(int a, int b)
		{
			if (b == 0)
				return 0;
			return a / b;
		}
	public:
		int calculate(int a, int b, char op)
		{
			ops['+'] = &calculator::add;
			ops['-'] = &calculator::subtract;
			ops['*'] = &calculator::multiply;
			ops['/'] = &calculator::divide;
			if (ops.find(op) == ops.end())
			{
				std::cout << "Invalid operation" << std::endl;
				return 0;
			}
			return (this->*ops[op])(a, b);      
		}
};

int main()
{
	calculator calc;
	std::cout << calc.calculate(5, 3, '.') << std::endl;
	return 0;
}