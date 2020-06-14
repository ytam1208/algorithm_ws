#include <thread>
#include <iostream>
#include <chrono>
#include <ros/ros.h>

using_namespace std;
class func1 {
public:
    func1(int _id, int _length) : m_nid(_id), m_nlength(_length) {}
    void operator()() {
        for (int i = 0; i < m_nlength; ++i)
            cout << "id : " << m_nid << " value : " << i << endl;
    }
private:
    int m_nid;
    int m_nlength;
};
void func2(int id, int length) {
    for (int i = 0; i < length; ++i)
        cout << "id : " << id << " value : " << i << endl;
}
 
int main() {
    thread t1(func2, 1, 5);
    thread t2(func2, 2, 5);    // 함수를 넘기는 방법
 
    thread t3{ func1(3, 5)};
    func1 f1(4, 5);
    thread t4(f1);
    thread t5(func1(5, 5)); // 함수객체를 넘기는 방법
 
    thread t6([](int id, int length) {
        for( int i = 0 ; i < length ; ++i)
            cout << "id : " << id << " value : " << i << endl;
    }, 6, 5);    // 람다를 넘기는 방법
    
    thread t7;
    t7 = std::thread(func1(7, 5));    // 이런 식의 지연 시킨 뒤 사용도 가능
 
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
}
