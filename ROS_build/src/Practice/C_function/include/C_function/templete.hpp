#include <iostream>

class Data
{
    public:
        template <typename TL> void Go(TL &input, TL &output);
        template <typename TL> void Turn(TL &input, TL &output);        
        
        Data(){};
        ~Data(){};
};

class A
{
    public:
        int x = 100;
        int y = 20;
};
