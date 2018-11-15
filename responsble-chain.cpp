#include <iostream> 

using namespace std;


class Ojbect {
	public:
		virtual void click() = 0;
		virtual int show() = 0; 
		virtual void process(int lev) = 0;
		virtual ~Ojbect(){}
		Ojbect* base;
		void setUpperPtr(Ojbect * b) {base = b;}
};

class View :public Ojbect{
	public:
		void click(){cout << "View click\n";}
		int show(){cout << "View show\n"; return 0;}
		void process(int lev) {
			if(lev < 5) {
				cout << "View handle process lvl < 5\n";
			} 
		}
		virtual ~View(){}
};

class MessageBox :public View{
	public:
		void click(){cout << "MessageBox click\n"; base->click();}
		int show(){cout << "MessageBox show\n"; return View::show();}
		void process(int lev) {
			if(lev > 5 && lev < 10) {
				cout << "MessageBox handle process  5 < lvl < 10\n";
			} else {
				//View::process(lev);
				base->process(lev);
			}
		}
		virtual ~MessageBox(){}
};

class PushMessageBox :public MessageBox{
	public:
		void click(){cout << "PushMessageBox click\n"; 
			if(base) base->click();}
		int show(){cout << "PushMessageBox show\n"; return MessageBox::show();}
		void process(int lev) {
			if(lev > 10 && lev < 100) {
				cout << "PushMessageBox handle process  10 < lvl < 100\n";
			} else {
				//MessageBox::process(lev);
				base->process(lev);
			}
		}
		virtual ~PushMessageBox(){}
};

int main(int argc , char* argv[])
{
	PushMessageBox pushBox;
	MessageBox msgBox;
	View view;
	//指针可灵活，也可以跳这层次的指向view
	pushBox.setUpperPtr(&msgBox);
	msgBox.setUpperPtr(&view);
	pushBox.click();
	//也可以指向同一层次的类
	PushMessageBox pb1,pb2,pb3,pb4;
	pb1.setUpperPtr(&pb2);
	pb2.setUpperPtr(&pb3);
	pb3.setUpperPtr(&pb4);
	//循环成链...
	//pb4.setUpperPtr(&pb1);
	
	pb1.click();


	//强制层次关系，跟基类指针无关
	pushBox.show();
	//把判断条件放在虚函数里
	pushBox.process(20);
	pushBox.process(8);
	pushBox.process(3);

	
	return 0;
}
