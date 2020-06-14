// #include <iostream>
// #include <map>
// #include <string>

// int main()
// {
//     std::map<std::string, int> m;

//     m.insert(std::make_pair("a", 6));   //inset 맵에 원소를 pair 형태로 추가
//     m.insert(std::make_pair("b", 9));
//     m.insert(std::make_pair("c", 3));
//     m.insert(std::make_pair("d", 4));
//     m.insert(std::make_pair("e", 5));
//     m["f"] = 6;

//     m.erase("d");   //map에서 key에 해당하는 원소 삭제
//     m.erase("e");
//     m.erase(m.find("f"));

//     if(!m.empty()) std::cout << "m size: " << m.size() << '\n';

//     std::cout << "a: " << m.find("a") -> second <<'\n';     //key에 해당하는 iterator를 반환
//     std::cout << "b: " << m.find("b") -> second << '\n';

//     std::cout << "a count: " << m.count("a") << '\n';   //key에 해당하는 원소들의 개수를 반환
//     std::cout << "b count: " << m.count("b") << '\n';

//     std::cout << "traverse" << '\n';

//     for(auto it = m.begin(); it != m.end(); it++)
//     {
//         std::cout << "key: " << it -> first << "" << "value: " << it -> second << '\n';
//     }

//     return 0;
// }

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

// template <template <typename> class P = std::less>
// struct compare_pair_second
// {
//     template <class T1, class T2>
//     bool operator()(const std::pair<T1, T2> &left, const std::pair<T1, T2> &right)
//     {
//         return P<T2>()(left.second, right.second);
//     }
// };

int main()
{
    int insert_n = 0;
    int num = 0;
    int index = 0;

    std::vector<int> a, b;
    std::map<int, int> b_index;
    std::map<int, int> a_index;

    std::cout << "inset num!!!\n";
    std::cin >> num;

    for (int i = 0; i < num; i++)
    {
        std::cin >> index;
        a.push_back(index);
    }
    sort(a.begin(), a.end(), std::less<int>());

    for (int i = 0; i < num; i++)
    {
        std::cin >> index;
        b.push_back(index);
    }

    for (int i = 0; i < (int)a.size(); i++)
    {
        a_index.insert(std::pair<int, int>(i, a[i]));
    }

    for (int i = 0; i < (int)b.size(); i++)
    {
        b_index.insert(std::pair<int, int>(i, b[i]));
    }
    sort(b_index.begin(), b_index.end(), compare);

    
    for (int i = 0; i < num; i++)
    {
        for (int j = i + 1; j < num; j++)
        {

        }
    }
}

bool compare(std::pair<int, int> a, std::pair<int, int> b)
{
    if (a.first == b.first)
    {
        return a.second < b.second;
    }
    else
    {
        return a.first < b.first;
    }
}