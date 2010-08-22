#include <iostream>

using namespace std;

class Visitor;


class Base 
{
	public:

	virtual ~Base() {} 
	virtual void accept(Visitor *) = 0;
};


class SayingClass : public Base 
{ 
	public:

	void say_hey(void)
	{
		cout << "Hey" << endl;
	}

	virtual void accept(Visitor *);
};


class LOLingClasss : public Base 
{
	public:

	void lol(void)
	{
		cout << "LOL!!!" << endl;
	}

	virtual void accept(Visitor *);
};

class Visitor
{
	public:

	virtual ~Visitor() {}
	virtual void visit(SayingClass *)	= 0;
	virtual void visit(LOLingClasss *)	= 0;
};


class DoVisitor : public Visitor
{
	virtual void visit(SayingClass *e) 
	{
		e->say_hey();
	}

	virtual void visit(LOLingClasss *e)
	{
		e->lol();
	}	
};

void SayingClass::accept(Visitor *v)
{
	v->visit(this);
}

void LOLingClasss::accept(Visitor *v)
{
	v->visit(this);
}

Base *get_obj(void)
{
	return new SayingClass;


}

int main(void)
{
	Base *s = get_obj();

	DoVisitor *dv = new DoVisitor;

	s->accept(dv);
	

	return 0;
}
