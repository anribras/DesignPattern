#include <iostream>
#include <string.h>
#include <memory.h>
#include <memory>
#include <string>



#define MAX_TYPE_LEN (100)
typedef void *(func)(const std::shared_ptr<std::string> &);

class Base {
	public:
		Base(const char* type){
			 m_type = new char[MAX_TYPE_LEN];
			 strcat(m_type,type);
		}
		virtual ~Base() {
			if(m_type)
				delete m_type;
		}
		void set_type(const char* type){
			 memset(m_type,0,MAX_TYPE_LEN);
			 strcat(m_type,type);
		}
		char* get_type(){
			return m_type;
		}
	private:
		char *m_type; 

};
class Sender;
class Receiver;
class Mediator {
	public:
		Mediator() = default;
		virtual ~Mediator(){}
		void set_inter(Sender * sender, Receiver * recv){
			m_sender = sender;
			m_recv = recv;
		}
		virtual void interaction() { 
			std::cout << "interaction not implemented." << std::endl;
		}
	protected:
		Sender * m_sender;
		Receiver * m_recv;
};

class Sender : public Base {
	public:
		Sender(std::shared_ptr<std::string> & data, int len ,const char * type): Base(type) {
			m_data = data;
			m_len = len;
		}
		~Sender() = default;
		void set_mediator(Mediator * m){
			m_med = m;
		}
		void send() {
			if (m_med) m_med->interaction();
		}
		const std::shared_ptr<std::string> & get_data(){ 
			return m_data;
	   	}
	private:
		std::shared_ptr<std::string>  m_data;
		int m_len;
		Mediator * m_med;
};

class Receiver : public Base {
	public:
		Receiver(const char * type): Base(type) {}
		~Receiver()= default;
		void set_mediator(Mediator * m){
			m_med = m;
		}
		void set_recv_callback(func * f){
			m_fun = f;
		}
		std::shared_ptr<std::string> get_message(Base * b) {
          auto s  = dynamic_cast<Sender *>(b);
			if ( s != NULL)
				std::cout << this->get_type() <<  " got message from " << s->get_type() <<  std::endl;
				if (m_fun) (*m_fun)(s->get_data());
				else std::cout << "cb null" << std::endl;
			return 0;
	   	}
		
	private:
		Mediator * m_med;
		func *  m_fun ;
};


class ProtocolMediator: public Mediator{
	public:
		ProtocolMediator() = default;
		~ProtocolMediator() = default;
		void interaction() {
			if(m_sender && m_recv) {
				m_recv->get_message(m_sender);
			}
		}
	private: 
		
};

void some_cbs(const std::shared_ptr<std::string> & data)
{
	std::cout <<"on yeah use_counts " << data.use_count() <<  std::endl;

	std::cout << "Got: " << *data << std::endl;
}

/*
 *sender通过mediator连接recver,sender->send最终调用了recv里注册的callback
 *sender和recver不直接打交道,而是通过meditor
 *meditor通过set_inter绑定不同的sender和recver,并且定义交互interaction
 *meditor 可实现 1对多 多对1 多对多 多种mapping方式
 */
int main(int argc , char* argv[])
{
	auto flag = std::make_shared<std::string>("bliblibliblablabla I love you");

	std::cout << "Mediator pattern mode" << std::endl;

	Sender * sender = new Sender(flag,1,"Girl");
	Receiver* recver = new Receiver("Boy");
	ProtocolMediator * meditor = new ProtocolMediator();

	sender->set_mediator(meditor);
	recver->set_mediator(meditor);

	recver->set_recv_callback((func*)(&some_cbs));

	meditor->set_inter(sender,recver);

	sender->send();
	return 0;
}
