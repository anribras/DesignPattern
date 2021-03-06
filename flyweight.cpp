#include <iostream>
#include <map>
#include <string>

// 棋子享元, 有内部数据, 提供访问内部方法, 无外部数据, 仅提供抽象方法
class Qizi {
	public:
		Qizi(std::string n) :color(std::move(n)) {} 
		virtual ~Qizi() = default;
		//获取内部状态的方法,
		std::string getColor() {return color;}
		//设置外部状态的方法
		virtual int setPos(int x, int y) {}
		virtual int setIdx(int idx) {}
	private:
		const std::string & color;
};

//具体实现了外部状态如何处理的享元

class ConcreteQizi : public Qizi {
	public:
		ConcreteQizi(std::string n): Qizi(std::move(n)) {}
		//virtual ~ConcreteQizi(){};
		int setPos(int x, int y) {
			std::cout << "pos:" << x << "," << y << std::endl;
			return 0;
		}
		int setIdx(int idx)  {
			std::cout << "idx:" << idx <<  std::endl;
			return 0;
		}
};



//享元工厂+单例
class QiziFactory {
	public:
		ConcreteQizi getQizi(std::string color) {
			if( m_qizi.find(color) == m_qizi.end()) {
				std::cout  << "create " << color << "qizi" << std::endl;
				m_qizi.emplace(color,ConcreteQizi(color));
			} else {
				std::cout  << "already have " << color << "qizi" << std::endl;
			}
			return m_qizi.at(color); 
		}
		//简单单例
		static QiziFactory*  getInstance(void) {
			if (qiziFactory == NULL) {
				qiziFactory =  new QiziFactory();
			}
			return qiziFactory;
		} 
	private:
		std::map<std::string,ConcreteQizi> m_qizi;
		static QiziFactory * qiziFactory;
};

QiziFactory * QiziFactory::qiziFactory  = NULL;

/**
 * @brief main 
 * @param argc
 * @param argv[]
 * @return 
 */
int main(int argc , char* argv[])
{

	auto qiziFactory = QiziFactory::getInstance();

	auto black = qiziFactory->getQizi("black");

	black.setIdx(0);
	black.setPos(1,3);

	auto white = qiziFactory->getQizi("white");

	white.setIdx(1);
	white.setPos(2,2);

	auto black1 = qiziFactory->getQizi("black");

	black1.setIdx(2);
	black1.setPos(3,3);

	auto white1 = qiziFactory->getQizi("white");

	white1.setIdx(3);
	white1.setPos(4,2);

	return 0;
}
