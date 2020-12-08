#include<iostream>
using namespace std;

class AAA
{
    private:
        int num1;
    public:
        virtual void Func1() {
            cout<<"func1"<<endl;
        }
        void Func2() {
            cout<<"func2"<<endl;
        }
};

class BBB: public AAA{
    private:
        int num2;
    public:
        void Func1(){
            cout <<"BBB::FUnc1"<<endl;
        }
        void Func2(){
            cout <<"BBB::Func3"<<endl;
        }
};

int main()
{
    AAA * aptr = new AAA();
    aptr -> Func1();

    BBB * bptr = new BBB();
    bptr -> Func2();
    return 0;
}
// using namespace std;
 
// class parent {
// public :
//     virtual void v_print() {
//         cout << "parent" << "\n";
//     }
//     virtual void print() {
//         cout << "parent" << "\n";
//     }
// };
 
// class child : public parent {
// public :
//     void v_print() {
//         cout << "child" << "\n";
//     }
//     void print() {
//         cout << "child" << "\n";
//     }
// };
 
// int main() {
//     parent* p;
//     child c;
//     // p = &c;
 
//     // p->v_print();
//     // p->print();

//     c.v_print();
//     c.print();
 
//     return 0;
// }

