#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <thread>

using namespace std;

//------------------------------------------------------
// 遅延実行関数：DelayReveal()
// 指定時間（ミリ秒）待ってからラムダ関数を呼び出す
//------------------------------------------------------
void DelayReveal(function<void()> fn, unsigned int delayMs)
{
    printf("結果を発表します...\n");
    this_thread::sleep_for(chrono::milliseconds(delayMs)); // 指定時間待機
    fn(); // 関数（ラムダ）を呼び出す
}

//------------------------------------------------------
// メイン関数
//------------------------------------------------------
int main()
{
    // 乱数の初期化（現在時刻をシードとして使用）
    srand((unsigned int)time(NULL));

    int userGuess = 0;

    // 入力案内
    printf("【サイコロ奇数偶数当てゲーム】\n");
    printf("奇数（半）なら 1、偶数（丁）なら 0 を入力してください: ");

    // 入力受付
    if (scanf_s("%d", &userGuess) != 1) {
        printf("入力エラーです。\n");
        return 1;
    }

    // 出目の生成（1～6）
    int roll = rand() % 6 + 1;

    // ラムダ式で結果表示関数を定義
    auto showResult = [=]() {
        printf("サイコロの出目は「%d」でした！\n", roll);

        int result = roll % 2;

        if (result == userGuess) {
            printf("正解です！\n");
        } else {
            printf("不正解です...\n");
        }
    };

    // 3秒待って結果を表示（ラムダ式を渡す）
    DelayReveal(showResult, 3000);

    return 0;
}
