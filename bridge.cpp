#include <iostream>


#include <string>
using std::string;
using std::cout;
//
class Encoder  {
	public:
		virtual void setEncoder(const string & s) = 0;
		virtual ~Encoder(){}
};

class Channel {
	public:
		virtual void connect() = 0;
		virtual void send() = 0;
		virtual ~Channel(){}
};

class JpegEncoder : public Encoder {
	public:
		void setEncoder(const string & s) {cout << "setup jpeg  encoder " << s << "\n";}
		virtual ~JpegEncoder(){}
};

class H264Encoder : public Encoder {
	public:
		void setEncoder(const string & s) {cout << "setup h264  encoder " << s << "\n";}
		virtual ~H264Encoder(){}
};

class WifiChannel : public Channel {
	public:
		void connect() {cout << "Wifi connect\n";}
		void send() {cout << "Wifi send\n";}
		virtual ~WifiChannel(){}
};

class UsbChannel : public Channel {
	public:
		void connect() {cout << "Usb connect\n";}
		void send() {cout << "Usb send\n";}
		virtual ~UsbChannel(){}
};

class Mirror {
	public:
		Mirror(Channel* c, Encoder* e):chn(c),encoder(e) {}
		void openChannel() { chn->connect();}
		void initEncoder(const string & s) {encoder->setEncoder(s);}
	private:
		//bridge的核心在这，通过基类指针指向不同的派生类，抽象了具体实现，
		//而且不同的变化间独立，可用不同的基类指针
		Channel* chn;
		Encoder* encoder;
};


int main(int argc, char* argv[])
{
	Channel* c = new WifiChannel();
	Encoder* e = new H264Encoder(); 
	Mirror m(c, e);
	m.openChannel();
	m.initEncoder("ex1");

	Channel* c1 = new UsbChannel();
	Encoder* e1 = new JpegEncoder(); 
	Mirror m1(c1, e1);
	m1.openChannel();
	m1.initEncoder("ex2");


	return 0;
}
