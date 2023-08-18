#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <string.h>
#include <chrono>

constexpr unsigned int HashCode(const char* str)
{
    return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * HashCode(str+1): 8603;
}

unsigned int generateHash(const char *string, size_t len)
{
  unsigned int hash = 0;
  for(size_t i = 0; i < len; ++i)
  {
     hash = 65599 * hash + string[i];
  }
  return hash ^ (hash >> 16);
}

unsigned int m_generateHash(const char *string, size_t len)
{
  unsigned int hash = 0;
  for(size_t i = 0; i < len; ++i)
  {
     hash = 65599 * hash + string[i];
  }
  return hash;
}

void CASE(std::string& method)
{
    switch (HashCode(method.c_str()))
    {
    case HashCode("1"):
        std::cout << "me1";
        break;
    case HashCode("2"):
        std::cout << "me2";
        break;    
    case HashCode("3"):
        std::cout << "me3";
        break;  
    case HashCode("4"):
        std::cout << "me4";
        break;  
    case HashCode("5"):
        std::cout << "me5";
        break;  
    default:
        break;
    }
}

void IFELSE(std::string& method)
{
    if(method.compare("1") == 0)
        std::cout << "me1";
    else if(method.compare("2") == 0)
        std::cout << "me2";
    else if(method.compare("3") == 0)
        std::cout << "me3";
    else if(method.compare("4") == 0)
        std::cout << "me4";
    else if(method.compare("5") == 0)
        std::cout << "me5";
}

int main() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    for(int i = 0; i < 1000000; i++)
        HashCode("query");
    std::chrono::duration<double>sec = std::chrono::system_clock::now() - start;
    std::cout << "HashCode 돌리는데 걸리는 시간(초) : " << sec.count() <<"seconds"<< std::endl;

    std::chrono::system_clock::time_point start2 = std::chrono::system_clock::now();
    for(int i = 0; i < 1000000; i++)
        generateHash("query", strlen("query"));
    std::chrono::duration<double>sec2 = std::chrono::system_clock::now() - start2;
    std::cout << "HashCode 돌리는데 걸리는 시간(초) : " << sec2.count() <<"seconds"<< std::endl;

    std::chrono::system_clock::time_point start3 = std::chrono::system_clock::now();
    for(int i = 0; i < 1000000; i++)
        m_generateHash("query", strlen("query"));
    std::chrono::duration<double>sec3 = std::chrono::system_clock::now() - start3;
    std::cout << "HashCode 돌리는데 걸리는 시간(초) : " << sec3.count() <<"seconds"<< std::endl;

    std::string command = "5";

    std::chrono::system_clock::time_point start4 = std::chrono::system_clock::now();
    for(int i = 0; i < 1000000; i++)
        CASE(command);
    std::chrono::duration<double>sec4 = std::chrono::system_clock::now() - start4;
    std::cout << std::endl;
    std::chrono::system_clock::time_point start5 = std::chrono::system_clock::now();
    for(int i = 0; i < 1000000; i++)
        IFELSE(command);
    std::chrono::duration<double>sec5 = std::chrono::system_clock::now() - start5;
    std::cout << "Switch 돌리는데 걸리는 시간(초) : " << sec4.count() <<"seconds"<< std::endl;
    std::cout << "IF 돌리는데 걸리는 시간(초) : " << sec5.count() <<"seconds"<< std::endl;

    return 0;
}