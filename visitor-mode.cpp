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
		//这是模板模式，叫框架模式更好
		void job_process1() { jobA();jobB();jobC();}
		void job_process2() { jobC();jobB();jobA();}
};



/** @brief ElementA
 * 某个元素的具体类,
 * 有3个工作，但是不想直接调用.通过访问者来间接使用，这样改变访问者，也改变了功能
 * */
class ElementA :public Element {

	void jobA() {cout << __func__ << endl;}
	void jobB() {cout << __func__ << endl;}
	void jobC() {cout << __func__ << endl;}
};

class ElementB :public Element {

	void jobA() {cout << __func__ << "for ElementB\n" << endl;}
	void jobB() {cout << __func__ << "for ElementB\n" << endl;}
	void jobC() {cout << __func__ << "for ElementB\n" << endl;}
};


class VisitorA :public Visitor{
	//通过visitor来扩展原来Element的功能,而不是改变Element
	void visit(Element* e) {
		e->job_process1();
	}
};

class VisitorB :public Visitor{
	void visit(Element* e) {
		e->job_process2();
	}
};

int main(int argc , char* argv[])
{
	Element* ea = new ElementA();
	Element* eb = new ElementB();
	Visitor* va = new VisitorA();
	Visitor* vb = new VisitorB();
	ea->accept(va); //实际调用了继承的job_process1,用访问者做了一层封装
	ea->accept(vb);

	eb->accept(va); 
	eb->accept(vb);
	return 0;
}
