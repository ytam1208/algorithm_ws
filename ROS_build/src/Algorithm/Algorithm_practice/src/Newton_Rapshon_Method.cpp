/*
    [23.01.01]
    Newton-Rapshon Method
    
    [Reference]
    https://wikidocs.net/150448
*/

#include <iostream>
#include <cmath>

class Solver
{
    public:        
        double Newton(double current_x, double input_error, double Max_iter){
            double y = function(current_x);
            for(int i = 0; i < Max_iter; i++){
                double Next_x = current_x - (y / diff_function(current_x));
                y = function(Next_x);
                std::cout << "iter[" << i << "] Next_X[" << Next_x << "]" << "f(Next_x)[" << y << "]" << std::endl;
                if(fabs(y) < input_error | fabs(Next_x-current_x) < input_error)
                    std::cout << "Solution = " << Next_x << std::endl;
                current_x = Next_x;
            }
        }

        inline double function(double input_x){
            return cos(input_x)-input_x;
        }
        inline double diff_function(double input_x){
            return -sin(input_x)-1;
        }
        
        Solver(){}
        Solver(double input_x, double input_error, int input_Max_iter){
            Newton(input_x, input_error, input_Max_iter);
        }
        ~Solver(){}
};

int main()
{
    /*
        example function --> y = cos(x) - x
    */
    Solver(0, 0.0001, 20);
    return 0;
}