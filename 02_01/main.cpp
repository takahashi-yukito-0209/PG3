#include <iostream> 

template <typename T> 
T Min(T a, T b)
{
    return (a < b) ? a : b;
}


int main()
{
    // int型の比較
    int intA = 81;
    int intB = 20;
    std::cout << "int型: Min(" << intA << ", " << intB << ") = " << Min(intA, intB) << std::endl;

    // float型の比較
    float floatA = 2.5f;
    float floatB = 0.2f;
    std::cout << "float型: Min(" << floatA << ", " << floatB << ") = " << Min(floatA, floatB) << std::endl;

    // double型の比較
    double doubleA = 0.29;
    double doubleB = 2.09;
    std::cout << "double型: Min(" << doubleA << ", " << doubleB << ") = " << Min(doubleA, doubleB) << std::endl;

    return 0;
}
