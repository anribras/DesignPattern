#include <iostream>

using namespace std;

class Target {
	public:
		virtual int sendData(const char* data) = 0;
		virtual ~Target(){}
};

class Adaptee {
	public:
		int sendCommandData(const char* cmd, const char* data){
			cout << "cmd = " << cmd << endl;
			cout << "data = " << data << endl;
			return 0;
		}
};

class Adapter : public Target {
	public:
		Adapter(Adaptee* t):tee(t) {}
		//适配Adaptee里的SendCommandData
		int sendData(const char* data)  {
			return tee->sendCommandData("default",data);
		}
	private:
		Adaptee* tee;
};

int main(int argc, char* argv[])
{
	Adapter* adapter =  new Adapter(new Adaptee());
	adapter->sendData("Adapter mode is running");
	return 0 ;
}
