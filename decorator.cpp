#include <string>
#include <iostream>

class Component {
	public :
		Component() = default;
		virtual ~Component(){}
		virtual void operation(){}
};

class ConcreteComponent : public Component {
	public:
		void operation() { std::cout << "ConcreteComponent operation." << std::endl;}
};

class Decorator : public Component {
	public:
		void set_deco_obj(Component *p) {ptr_component = p;}
	protected:
		Component* ptr_component;
};

class ConcreteDecorator : public Decorator {
	public:
		ConcreteDecorator(Component * c = NULL) {
			ptr_component = c;
		}
		void before() { std::cout << "deco before operation." << std::endl;}
		void after() { std::cout << "deco after operation." << std::endl;}
		void operation() { 
			before();
			ptr_component->operation();
			after();
		}
};

class ConcreteDecorator1 : public Decorator {
	public:
		ConcreteDecorator1(Component * c = NULL) {
			ptr_component = c;
		}
		void before() { std::cout << "deco1 before operation." << std::endl;}
		void after() { std::cout << "deco1 after operation." << std::endl;}
		void operation() { 
			before();
			ptr_component->operation();
			after();
		}
};

int main(int argc, char* argv[])
{
	std::cout << "Before decoration..." <<std::endl;
	ConcreteComponent c;
	c.operation();

	std::cout << "After decoration..." <<std::endl;
	//半透明模式,在具体抽象构件里,调用自己的方法,如after
	ConcreteDecorator deco;
	deco.set_deco_obj(&c);
	deco.operation();
	deco.after();

	//透明装饰模式
	//使用抽象构建来定义对象.只使用抽象构建里定义的方法,如operation
	std::cout << "Transparent decorator mode..." <<std::endl;
	Component * cc1  = new ConcreteDecorator(&c);
	cc1->operation();

	Component * cc  = new ConcreteDecorator1(&c);
	cc->operation();

	return 0;
}
