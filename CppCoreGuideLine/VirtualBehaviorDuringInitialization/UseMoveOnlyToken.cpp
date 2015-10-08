// This example was taken from https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md
//
// it was edited and expanded to try to address the problem that this is addressing differently:
// "Derived classes such as D must not expose a public constructor. Otherwise, D's users could create D objects that don't invoke PostInitialize."
//
// One of the disadvantage of this approach is that the base class B delegate the responsibility to ensure
// only B::Create can create a B to its derived class
//
// Here we address the issue by leaving derived class the freedom to select the visibility of the constructor,
// But we require in the constructor of the base class B, a token that could only have been created in B::Create, and only been moved
// to its destination from there.
//
// The benefit of this solution is that the requirement that object derived from B are constructed using B::Create are controlled
// by B, and creating an instance of D outside of B::Create require fairly forced code.

#include <iostream>
#include <memory>

using namespace std;

template< typename T >
class Token
{
public:
    // Could do something different in debug to ensure not moved twice.
    // or actually point to valid memory.
    Token( Token&& ) = default;
    ~Token() = default;

private:
    friend T;

    Token() = default;
    Token( const Token& ) = delete;
    Token& operator=( const Token& ) = delete;
    Token& operator=( const Token&& ) = delete;
};
    
class B {
public:
    typedef Token< B > TokenB;

public:
    B( TokenB&& ) { /* ... */ }
    virtual void f() = 0;

    template<class T>
    static shared_ptr<T> Create()    // interface for creating objects
    {
        auto p = make_shared<T>( TokenB() );
        p->PostInitialize();
        return p;
    }
    
protected:
    virtual void PostInitialize()    // called right after construction
    {
        /* ... */
        f();
        /*...*/
    }   // GOOD: virtual dispatch is safe
};

class D : public B {
public:
    D( TokenB&& tok ) : B( std::move( tok ) ) {}
    void f() override {};
};

int main()
{
    // D local( D::TokenB() ); // no access to TokenB constructor.
	
    shared_ptr<D> p = D::Create<D>();    // creating a D object
    cout << "Hello World" << p.get() << endl; 
}
