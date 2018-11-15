#include <iostream>

using namespace std;
//定义两种访问者,分别访问不同的元素

class Element;
class Visitor {
	public:
		//定义可能要访问哪些元素
		virtual void visit(Element * v) = 0;
		virtual ~Visitor(){}
};
class Element {
	public:
		virtual ~Element(){}
		//定义可能有哪些访问者来访
		virtual void accept(Visitor* v) {
			v->visit(this);}
		virtual void jobA() = 0;
		virtual void jobB() = 0;
		virtual void jobC() = 0;
		//父类定义框架，模板模式，叫框架模式更好
		//void job_process1() { jobA();jobB();jobC();}
		//void job_process2() { jobC();jobB();jobA();}
};

class Strategy {
	public:
		Strategy(){}
		Strategy(Element* e):elem(e){}
		virtual void chooseStrategy(){}
		virtual ~Strategy(){}
		Element* getElem(){return elem;}
	private:
		Element* elem;
};

class StrategyA : public Strategy {
	public:
		StrategyA(Element* e):Strategy(e) {}
		void chooseStrategy(){
			getElem()->jobA();
			getElem()->jobB();
			getElem()->jobC();
		}
		StrategyA(){}
		virtual ~StrategyA(){}
};

class StrategyB : public Strategy {
	public:
		StrategyB(Element* e):Strategy(e) {}
		void chooseStrategy(){
			getElem()->jobC();
			getElem()->jobB();
			getElem()->jobA();
		}
		virtual ~StrategyB(){}
};

class Context  {
	public:
		Context(){}
		~Context(){}
		Context(Strategy * s):st(s) {}
		void setStrategy(Strategy * s) {st = s;}
		Strategy* getStrategy() {return st; }
		void excute(){st->chooseStrategy();}
	private:
		Strategy * st;
};


/** @brief ElementA
 * 某个元素的具体类,
 * 有3个工作，但是不想直接调用.通过访问者来间接使用，这样改变访问者，也改变了功能
 * */
class ElementA :public Element {

	void jobA() {cout << __func__ << "for ElementA\n" << endl;}
	void jobB() {cout << __func__ << "for ElementA\n" << endl;}
	void jobC() {cout << __func__ << "for ElementA\n" << endl;}
};

class ElementB :public Element {

	void jobA() {cout << __func__ << "for ElementB\n" << endl;}
	void jobB() {cout << __func__ << "for ElementB\n" << endl;}
	void jobC() {cout << __func__ << "for ElementB\n" << endl;}
};



int main(int argc , char* argv[])
{
	Element* ea = new ElementA();
	Element* eb = new ElementB();
	Context * context = new Context();

	StrategyA* sta = new StrategyA(ea);
	context->setStrategy(sta);
	context->excute();

	StrategyB* stb = new StrategyB(eb);
	context->setStrategy(stb);
	context->excute();

	return 0;
}
