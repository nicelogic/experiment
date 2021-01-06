
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

class IncompleteType;

int main()
{
    cout << "smart point test" << endl;

    {
        // unique_ptr<IncompleteType> uniquePtr;
        shared_ptr<IncompleteType> sharedPtr;
    }

    {
        shared_ptr<int> sPtr = make_shared<int>(3);
        shared_ptr<int> anotherPtr = sPtr;
        cout << sPtr.use_count() << endl;
    }

    class B;
    class A
    {
    public:
        shared_ptr<B> b;
    };
    class B
    {
    public:
        shared_ptr<A> a;
    };
    weak_ptr<A> wspa;
    {
        shared_ptr<A> spa = make_shared<A>();
        wspa = spa;
        shared_ptr<B> spb = make_shared<B>();
        spa->b = spb;
        spb->a = spa;
    }
    cout << "wspa count: " << wspa.use_count() << endl;

    cout << "smart point test finished" << endl;

    //test for smart point with typeid
    class Atype
    {
    public:
        virtual ~Atype(){};
    };
    class Btype
    {
    };
    class Ctype : public Atype
    {
    };

    Atype a;
    Btype b;
    Ctype c;
    Atype &ca = c;
    shared_ptr<Atype> pa = make_shared<Ctype>();

    cout << "A type id: " << typeid(a).name() << "\n"
         << "B type id: " << typeid(b).name() << "\n"
         << "C type id: " << typeid(c).name() << "\n"
         << "ca: " << typeid(decltype(ca)).name() << "\n"
         << "pa: " << typeid(*pa).name() << "\n"
         << endl;


    //about thread
    class ThreadTest
    {
    public:
        ThreadTest() { cout << "threadtest contruct" << endl; }
        ~ThreadTest() { cout << "threadtest destruct" << endl; }
    };

    //这说明，传引用进来，结束是不会触发析构，因此传递引用和值都是可以都，因为是指针，性能也无差异
    {
        shared_ptr<ThreadTest> ts = make_shared<ThreadTest>(ThreadTest());
        thread t1([ts]() {
            cout << "ts count: " << ts.use_count() << endl;
        });
        t1.join();
        cout << "end" << endl;
        cout << "ts count: " << ts.use_count() << endl;
    }
    cout << "----" << endl;

    {
        class A
        {
        public:
            virtual shared_ptr<A> getPival()
            {
                return m_pIval;
            }
        protected:
            shared_ptr<A> m_pIval;
        };

        class B : public A{
        public:
            B(shared_ptr<A> a){}
            B(){}
            shared_ptr<A> getPival() override
            {
                cout << "hi" << endl;
                cout << "point: " << this << endl;
                return make_shared<B>(m_pIval);
            }
        };

        class C
        {
        public:


        };

        A a;
        cout << a.getPival().get() << endl;
        shared_ptr<A> pa = make_shared<B>();
        cout << pa.get() << endl;
        auto c = pa->getPival();
        cout << "c point:" << c.get() << endl;
    }

    return 0;
}