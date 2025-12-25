#include <iostream>
#include <type_traits> 

// 2つの異なる型を扱えるクラステンプレート
template <typename T1, typename T2>
class MinCalculator {
public:
    // 2つの引数を比較して小さい方を返す
    std::common_type_t<T1, T2> Min(T1 a, T2 b)
    {
        // a と b を比較し、小さい方を返す
        return (a < b) ? a : b;
    }
};

int main()
{
    // int × float の組み合わせ
    MinCalculator<int, float> minIF;
    std::cout << "int と float : " << minIF.Min(10, 3.5f) << std::endl;

    // int × double の組み合わせ
    MinCalculator<int, double> minID;
    std::cout << "int と double: " << minID.Min(8, 12.3) << std::endl;

    // float × int の組み合わせ
    MinCalculator<float, int> minFI;
    std::cout << "float と int : " << minFI.Min(5.2f, 4) << std::endl;

    // float × double の組み合わせ
    MinCalculator<float, double> minFD;
    std::cout << "float と double: " << minFD.Min(9.8f, 2.1) << std::endl;

    // double × int の組み合わせ
    MinCalculator<double, int> minDI;
    std::cout << "double と int : " << minDI.Min(6.7, 10) << std::endl;

    // double × float の組み合わせ
    MinCalculator<double, float> minDF;
    std::cout << "double と float: " << minDF.Min(1.2, 3.4f) << std::endl;

    return 0;
}
