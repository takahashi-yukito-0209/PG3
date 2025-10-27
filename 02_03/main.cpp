#include <chrono> 
#include <cstdlib> 
#include <ctime> 
#include <iostream> 
#include <thread>

using namespace std;

//------------------------------------------------------
// 判定関数：ShowResult()
// 出目(roll)とユーザー入力(userGuess)を比較し、結果を表示
//------------------------------------------------------
void ShowResult(int roll, int userGuess)
{
    cout << "サイコロの出目は「"<< roll <<"」でした！" << endl;

    // 出目が奇数なら roll % 2 == 1、偶数なら roll % 2 == 0
    int result = roll % 2;

    // 判定
    if (result == userGuess) {
        cout << "正解です！" << endl;
    } else {
        cout << "不正解です..." << endl;
    }
}

//------------------------------------------------------
// 遅延実行関数：DelayReveal()
// 指定時間（ミリ秒）待ってから関数を呼び出す
//------------------------------------------------------
void DelayReveal(void (*fn)(int, int), unsigned int delayMs, int roll, int userGuess)
{
    cout << "結果を発表します..." << endl;
    this_thread::sleep_for(chrono::milliseconds(delayMs)); // 指定時間待機
    fn(roll, userGuess); // 関数ポインタで呼び出し
}

//------------------------------------------------------
// メイン関数
//------------------------------------------------------
int main()
{
    // 乱数の初期化（現在時刻をシードとして使用）
    srand((unsigned int)time(NULL));

    int userGuess;

    // 入力案内
    cout << "【サイコロ奇数偶数当てゲーム】" << endl;
    cout << "奇数（半）なら 1、偶数（丁）なら 0 を入力してください: ";
    cin >> userGuess;

    // 出目の生成（1～6）
    int roll = rand() % 6 + 1;

    // 3秒待って結果を表示
    DelayReveal(ShowResult, 1000, roll, userGuess);

    return 0;
}
