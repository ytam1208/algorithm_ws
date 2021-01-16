#include <iostream>

int main(int argc, char **argv)
{
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i)
    { //인자 개수(argc)만큼 출력
        std::cout << argv[i] << std::endl;
    }
}