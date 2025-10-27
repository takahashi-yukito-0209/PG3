#include <chrono> 
#include <cstdio> 
#include <cstdlib> 
#include <ctime> 
#include <thread> 

using namespace std;

//------------------------------------------------------
// 判定関数：ShowResult()
// 出目(roll)とユーザー入力(userGuess)を比較し、結果を表示
//------------------------------------------------------
void ShowResult(int roll, int userGuess)
{
    printf("サイコロの出目は「%d」でした！\n", roll);

    // 出目が奇数なら roll % 2 == 1、偶数なら roll % 2 == 0
    int result = roll % 2;

    // 判定結果を表示
    if (result == userGuess) {
        printf("正解です！\n");
    } else {
        printf("不正解です...\n");
    }
}

//------------------------------------------------------
// 遅延実行関数：DelayReveal()
// 指定時間（ミリ秒）待ってから関数を呼び出す
//------------------------------------------------------
void DelayReveal(void (*fn)(int, int), unsigned int delayMs, int roll, int userGuess)
{
    printf("結果を発表します...\n");
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

    // 3秒待って結果を表示
    DelayReveal(ShowResult, 3000, roll, userGuess);

    return 0;
}
