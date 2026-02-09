#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

using namespace std;

//------------------------------------------------------
// 遅延実行関数：DelayReveal()
// 指定時間（ミリ秒）待ってから C スタイルの関数ポインタを呼び出す
//------------------------------------------------------
void DelayReveal(void (*fn)(int, int), unsigned int delayMs, int roll, int userGuess)
{
    printf("結果を発表します...\n");
    Sleep(delayMs); // 指定時間待機（Windows API）
    fn(roll, userGuess); // 関数ポインタを呼び出す
}

// 結果表示関数（関数ポインタとして渡す）
void Reveal(int roll, int userGuess)
{
    printf("サイコロの出目は「%d」でした！\n", roll);

    int result = roll % 2;

    if (result == userGuess) {
        printf("正解です！\n");
    } else {
        printf("不正解です...\n");
    }
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

    // 3秒待って結果を表示（関数ポインタを渡す）
    DelayReveal(Reveal, 3000, roll, userGuess);

    return 0;
}
