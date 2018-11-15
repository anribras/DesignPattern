#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <typeinfo>
#include <string.h> //itoa
#include <algorithm>


class Component {
	public:
		Component(std::string n = "default"): name(n){}
		virtual ~Component(){}
		virtual void add(const std::shared_ptr<Component> & c){}
		//虚函数不可以重载
		//virtual void add() ;
		virtual void remove(){}
		virtual void operation(){}
		const std::string & getName() {return name;}
	private:
		std::string name;
};

class Leaf :  public Component {
	public:
		Leaf(std::string n = "default in leaf" ) :Component(n) {}
		~Leaf(){}
		//~Leaf()= default;
		void set_content(const std::string & s) {contents = s;}

		void remove() {
			std::cout << getName() << " removed !" << std::endl;
		}
		
		void operation() {
			std::cout << "I am " << getName() << " " <<  contents << std::endl;
		}
	private:
		std::string contents;
};

class Composite : public Component {
	public:
		Composite(std::string n = "default in composite" ) :Component(n) {}
		~Composite(){}
		void add(const std::shared_ptr<Component> & c) {
			items.insert(std::make_pair(c->getName() ,c));
			std::cout << getName() << " added " << c->getName() <<  std::endl;
		}

		void remove() {
			//递归的remove items
			for(auto it:items) {
				it.second->remove();
				//erase by key name
				items.erase(it.first);
			}
			std::cout << getName() << " removed !" << std::endl;

		}

		void operation() {
			if (items.empty() == 1)
				std::cout << "Oper: empty dir" << std::endl;
			else {
				std::cout << "Oper: I am " << getName() << std::endl;
				// same as remove
				for(auto it:items) {
					it.second->operation();
				}
			}
		}
	private:
		std::map<std::string, std::shared_ptr<Component>> items;
};


int main(int argc, char* argv[])
{
	auto root =  new Composite("root");
	std::vector<std::shared_ptr<Composite>> dirs;
	std::vector<std::shared_ptr<Leaf>> files;

	for (unsigned long i = 0; i< 10 ; i ++) {
		dirs.push_back( 
				std::make_shared<Composite>(std::string("dir")+std::to_string(i) ) 
				);
		files.push_back( 
				std::make_shared<Leaf>(std::string("file")+std::to_string(i) ) 
				);
		files[i]->set_content(std::string("Your lucky number is "+std::to_string(i)));
		dirs[i]->add(files[i]);
	}

	root->add(dirs[0]);
	root->add(dirs[1]);

	dirs[0]->add(dirs[2]);
	dirs[0]->add(dirs[3]);
	dirs[0]->add(dirs[4]);
	dirs[0]->add(dirs[5]);

	dirs[1]->add(dirs[6]);
	dirs[1]->add(dirs[7]);
	dirs[1]->add(dirs[8]);
	dirs[1]->add(dirs[9]);


	std::cout << "Check all:" << std::endl;
	root->operation();

	std::cout << "Remove a branch:" << std::endl;
	dirs[1]->remove();

	std::cout << "Check again:" << std::endl;
	root->operation();

	std::cout << "Check removed branch:" << std::endl;
	dirs[1]->operation();


	return 0;
}
