#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

// 一般的な賃金体系の給与を計算
double calculate_standard_wage(int hours)
{
    const double HOURLY_RATE = 1226.0; // 時給 1,226円
    return hours * HOURLY_RATE;
}

// 再帰的な賃金体系の給与を計算
double calculate_recursive_wage(int hours)
{
    if (hours <= 0)
        return 0.0;

    double total_wage = 0.0;
    double current_rate = 100.0; // 最初の1時間は100円
    total_wage += current_rate;

    for (int h = 2; h <= hours; ++h) {
        current_rate = current_rate * 2 - 50;
        total_wage += current_rate;
    }

    return total_wage;
}

int main()
{
    cout << "───────────────────────────────────────────────────────────────" << endl;
    cout << " 時間 │ 一般的な賃金(円) │ 再帰的な賃金(円) │ どちらが高い？" << endl;
    cout << "───────────────────────────────────────────────────────────────" << endl;

    cout << fixed << setprecision(0);

    int over_hour = -1;

    for (int h = 1; h <= 20; ++h) {
        double standard = calculate_standard_wage(h);
        double recursive = calculate_recursive_wage(h);

        string result;
        if (standard > recursive)
            result = "一般的の方が高い";
        else if (recursive > standard)
            result = "再帰的の方が高い";
        else
            result = "同額";

        // 桁を右揃えで整える
        cout << setw(2) << right << h << "時間 │ "
             << setw(15) << right << standard << " │ "
             << setw(15) << right << recursive << " │ "
             << left << result << endl;

        if (over_hour == -1 && recursive > standard)
            over_hour = h;
    }

    return 0;
}
