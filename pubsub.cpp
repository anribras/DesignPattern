#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <set>
#include <list>

/**
 *
 *
 * 博客分不同主题,
 * 不同主题下有不同的订阅者.实现以下几个目标
 *
 * 1. 订阅者本身也是文章的发布者,可以发布一篇多个主题的文章;
 * 2. 当某个主题的文章更新时,通知所有相关订阅者有新的文章;
 * 3. 订阅者 也可以 订阅多个主题的文章.
 * 4. 订阅者可以关注其他订阅者, 当其他人发表新文章时,通知自己.
 *
 * 
 * */



class Observer;
class Subject;

class Article {
	public:

		void setId(int id) {article_id = id;}
		int getId() {return article_id;}

		void setTitle(std::string t) {title = t;}
		const std::string & getTitle() {return title;}

		void setAuthor(std::shared_ptr<Observer> ob) {author = ob;}
		std::shared_ptr<Observer> getAuthor() {return author;}

		void addSubjects(std::shared_ptr<Subject> subject) {
			subjects.insert(subject);
		}
		std::set<std::shared_ptr<Subject>> getSubjects() {
			return subjects;
		}

	private:
		//id
		int article_id;
		//标题
		std::string title;
		// 作者
		std::shared_ptr<Observer> author;
		//文章可以属于多个不同主题
		std::set<std::shared_ptr<Subject>> subjects;
};

class Observer;
class Subject {
	public :
		Subject(const std::string & topic): m_topic(topic){}
		virtual ~Subject(){}
		void setTopic(const std::string &  topic){
			m_topic =  topic;
		}
		//通知订阅者消息
		virtual void notifyAll() {}
		//在主题下添加了新文章
		virtual void addArticle(std::shared_ptr<Article> art) {
			m_articles.insert(
					std::make_pair(art->getId(),art)
					);
		}
		//在主题下去掉文章
		virtual void removeArticle(std::shared_ptr<Article> art) {
			auto found = m_articles.find(art->getId());
			if (found != m_articles.end()) {
				m_articles.erase(found);
			}
		}
		void addSubscriber(const Observer & ob){
			m_obs.push_back(ob);
		}
		//void removeSubscriber(const Observer & ob){
			//m_obs.remove(ob);
		//}
		const std::string & getTopic() {return m_topic;}
		std::list<Observer> getObservers() {
			return m_obs;
		}
	private:
		//谁订阅了该主题
		std::list<Observer> m_obs;
		//什么主题
		std::string m_topic;
		//发表在该主题下的文章
		std::map<int, std::shared_ptr<Article>> m_articles;
};

class Observer {
	public:
		Observer(std::string name):m_name(name) {}
		virtual ~Observer(){}
		//收到通知
		virtual void notified(std::shared_ptr<Article> art) {
			std::cout << m_name << " got new articles:(" <<
				art->getTitle() << ")" << std::endl;
		}
		//个人订阅什么主题
		virtual void subsTopic(std::shared_ptr<Subject> sub) {
			sub->addSubscriber(*this);

		}
		//个人关注的其他人
		virtual void watchSomebody(const std::shared_ptr<Observer>  & ob) {
			//把关注对象保存
			watch_objs.push_back(*ob);

			//还要把自己添加到别人的bewatched list里
			//auto & f  =  ob->getFollowLists();
			//注意下面引用方式的区别和用法
			auto & f = ob->followers;
			//auto f = ob->getFollowLists();
			std::cout << & ob->followers << std::endl;
			std::cout << & ob->getFollowLists() << std::endl;
			std::cout << &f << std::endl;
			f.push_back( 
					//std::shared_ptr<Observer>(this)
					std::make_shared<Observer>(*this)
					//*this
					);
			std::cout << ob->getName() << " add followers:" << 
				this->getName() << std::endl;
			std::cout << f.size() << std::endl;
		}

		void setName(const std::string & name) {m_name = name;}
		const std::string & getName() {return m_name;}

		const std::list<std::shared_ptr<Observer>> & getFollowLists() {return followers;}

		//个人被哪些人关注.
		std::list<std::shared_ptr<Observer>> followers;
		//个人关注的其他作者
		std::list<Observer> watch_objs;
	private:
		std::string m_name;
};

//一个write, 可以发布文章到主题上
class Writer : public Observer {
	public: Writer(std::string name) : Observer(name) {}
		void pubNewArticle(std::shared_ptr<Article> article) {
			std::cout << getName() << " published a new article." << std::endl;
			std::cout << "Title: " <<  article->getTitle() << std::endl;
			std::cout << "===============" <<std::endl;
			auto subjects = article->getSubjects();
			//遍历文章主题
			for (auto s : subjects) {
				auto obs = s->getObservers();
				//遍历主题下对应的订阅者
				for (auto o : obs) {
					//逐个通知
					o.notified(article);
				}
			}
			//遍历我的粉丝
			std::cout << "followers size:" << followers.size() << std::endl;
			if (followers.size()) {
				std::cout << getName() <<  " has followers.." << std::endl;
			}
			for (auto f: followers) {
				f->notified(article);
			}

		}
		
};

//一个reader,只订阅主题,  
class Reader : public Observer {
	public:
		Reader(std::string name) : Observer(name) {}
};

int main(int argc , char* argv[])
{

	auto topic_adventure =  std::make_shared<Subject>("Adventure");
	auto topic_love =  std::make_shared<Subject>("Love story");
	auto topic_travel =  std::make_shared<Subject>("Travel");

	auto hans = std::make_shared<Writer>("Mr Hans");

	auto tommy = std::make_shared<Reader>("Mr Tommy");
	tommy->subsTopic(topic_adventure);

	auto derk = std::make_shared<Reader>("Mr Derk");
	derk->subsTopic(topic_adventure);

	auto handsome = std::make_shared<Writer>("Mr Handsome");
	handsome->subsTopic(topic_adventure);
	handsome->subsTopic(topic_love);

	auto zombie = std::make_shared<Reader>("Miss Zombie");
	zombie->subsTopic(topic_adventure);
	zombie->subsTopic(topic_love);

	//僵尸小姐关注了handsome先生, 无论handsome的什么主题的文章都感兴趣..
	zombie->watchSomebody(handsome);

	std::cout << "Check followers ..." << std::endl;
	std::cout << & handsome->getFollowLists() << std::endl;
	std::cout << & handsome->followers  << std::endl;
	std::cout << handsome->getFollowLists().size() << std::endl;

	auto a = std::make_shared<Article>();
	a->setId(0);
	a->setTitle("Time travel to Mars");
	a->setAuthor(hans);
	a->addSubjects(topic_adventure);


	auto b = std::make_shared<Article>();
	b->setId(1);
	b->setTitle("A black hole in sky");
	b->setAuthor(hans);
	b->addSubjects(topic_adventure);

	hans->pubNewArticle(a);
	std::cout << std::endl;
	hans->pubNewArticle(b);

	auto c = std::make_shared<Article>();
	c->setId(1);
	c->setTitle("Senven days in Chengdu");
	c->setAuthor(handsome);
	c->addSubjects(topic_travel);

	std::cout << std::endl;
	handsome->pubNewArticle(c);

	return 0;
}

