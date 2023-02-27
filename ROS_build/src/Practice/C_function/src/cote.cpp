// #include <iostream>
// #include <vector>

// /*  
// 선행지식
//     class
//     class operator
//     template
//     std::for_each & lambda function
//     std::vector
//     std::thread
// */

// /*
// Mission 1. 아래 코드를 실행시키면 int, float, double 타입의 벡터가 0,0,0,0,0 이 출력된다. 
//            아래 Sort 함수를 채워 int, float, double 타입의 벡터가 1,2,3,4,5로 출력하게 만드시오.

// Mission 2. 아래 코드에 있는 int_v, float_v, double_v 안에 있는 1,2,3,4,5를 출력하는 쓰레드를 각각 구성하시오.
//            이때, thread 함수는 std::thread를 사용할 것.
//            ex) thread1 --> int, thread2 --> float, thread3 --> double 

// 코드 작성 중 Main문과 class내 함수 값을 바꾸지 말고 주석 처리된 Your code에만 작업 할 것.
// */


// class Define_type
// {
//     private:
//         template <typename T1>
//         void Sort(std::vector<T1>& input_v){
//             /*  Mission 1.
//                 Your code. 
//             */
//         }
//         template <typename T1>
//         void Print_vector(std::string& this_name, std::vector<T1>& input_v){
//             std::cout << this_name << " vector = ";
//             std::for_each(input_v.begin(), input_v.end(), [](const T1& value){
//                 std::cout << value << " ";
//             });
//             std::cout << std::endl << std::endl;
//         }                    
//     public:
//         void operator()(std::string& type_name){
//             if(type_name.compare("int") == 0){
//                 std::vector<int> int_v = {0,0,0,0,0};
//                 Print_vector<int>(type_name, int_v);  
//                 /* Mission 2.
//                     Your code
//                     thread 1
//                 */       
//             }
//             else if(type_name.compare("float") == 0){
//                 std::vector<float> float_v = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
//                 Print_vector<float>(type_name, float_v);  
//                 /* Mission 2.
//                     Your code
//                     thread 2
//                 */     
//             }
//             else if(type_name.compare("double") == 0){
//                 std::vector<double> double_v = {0.0, 0.0, 0.0, 0.0, 0.0};
//                 Print_vector<double>(type_name, double_v);  
//                 /* Mission 2.
//                     Your code
//                     thread 3
//                 */     
//             }
//             else{}
//         }
//         Define_type(){}
//         ~Define_type(){}
// };

// int main(int argc, char** argv)
// {
//     std::vector<std::string> my_string = {"int", "float", "double"};
//     Define_type Dt;

//     for(int i = 0; i < 3; i++)
//         Dt(my_string[i]);    
    
//     return 0;
// }