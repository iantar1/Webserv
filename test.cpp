


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

class AbstractA
{
	protected:
		int v1;
		int v2;
	public:
		virtual void setv1(int v1)
		{
			this->v1 = v1;
		
		}
		virtual void setv2(int v2)
		{
			this->v2 = v2;
		}
		virtual int getv1()
		{
			return v1;
		}
		virtual int getv2()
		{
			return v2;
		}
};

class A : public AbstractA
{
	private:
		int v3;
	public:
		void setv3(int v3)
		{
			this->v3 = v3;
		}
		int getv3()
		{
			return v3;
		}
};

class B : public AbstractA
{
	private:
		typedef void (B::*seters)(int);
		std::map<std::string, seters> ops;
		int v4;
		std::vector<A> vec;
	public:
		B()
		{
			ops["v1"] = &B::setv1;
			ops["v2"] = &B::setv2;
			ops["v4"] = &B::setv4;
		}
		void setv4(int v4)
		{
			this->v4 = v4;
		}
		int getv4()
		{
			return v4;
		}
		void addA(A a)
		{
			vec.push_back(a);
		}
		void init(std::string context, int v)
		{
			A a;
			if (ops.find(context) == ops.end())
			{
				std::cout << "Invalid operation" << std::endl;
				return;
			}
			(this->*ops[context])(v);
			for (int i = 0; i < 2; i++)
			{
				a.setv1(v * (i + 1) / 10);
				a.setv2(v * (i + 2) / 10);
				a.setv3(v * (i + 3) / 10);
				vec.push_back(a);
				a = A();
			}
		}
		std::vector<A> getVec()
		{
			return vec;
		}
};

// << operator overloading for B
std::ostream& operator<<(std::ostream &os, B &b)
{
	os << "v1: " << b.getv1() << std::endl;
	os << "v2: " << b.getv2() << std::endl;
	os << "v4: " << b.getv4() << std::endl;
	std::vector<A> vec = b.getVec();
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		os << "########## A: " << i << " ##########"<< std::endl;
		os << "v1: " << vec[i].getv1() << std::endl;
		os << "v2: " << vec[i].getv2() << std::endl;
		os << "v3: " << vec[i].getv3() << std::endl;
	}
	return os;
}


int main()
{
	B b;
	b.init("v1", 100);
	b.init("v2", 200);
	b.init("v4", 400);
	std::cout << b << std::endl;
	std::vector<A> vec = b.getVec();
	std::cout << "Size of vec: " << vec.size() << std::endl;
	return 0;
}