// This example was taken from https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md
//
// it was edited and expanded to follow the instruction in the discussion:
// "Derived classes such as D must not expose a public constructor. Otherwise, D's users could create D objects that don't invoke PostInitialize."
//
// This fill in the 'D' class and add friendship with B::Create.
// It also call the protected default constructor in B::Create instead of in std::make_shared.
//
// Note: One of the disadvantage of this approach is that the base class B delegate the responsability to ensure
// only B::Create can create a B to its derived class

#include <iostream>
#include <memory>

using namespace std;

class B {
protected:
    B() { /* ... */ }
    virtual void PostInitialize()    // called right after construction
    { /* ... */  f();  /*...*/ }   // GOOD: virtual dispatch is safe
public:
    virtual void f() = 0;

    template<class T>
    static shared_ptr<T> Create()    // interface for creating objects
    {
        auto p = make_shared<T>( T() ); // call T() in friend Create
        p->PostInitialize();
        return p;
    }
};

class D : public B {
public:
    void f() override { /* ...  */ };
    
protected:
	D() {}
	
	template<class T>
	friend shared_ptr<T> B::Create();
};

int main()
{
   shared_ptr<D> p = D::Create<D>();    // creating a D object
   cout << "Hello World" << p.get() << endl; 
}
