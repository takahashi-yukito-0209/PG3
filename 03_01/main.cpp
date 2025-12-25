#include <iostream>

// Enemyクラス
class Enemy {
public:
    // 状態を表すメンバ関数ポインタ型
    using StateFunc = void (Enemy::*)();

    Enemy()
        : state_(&Enemy::Approach) // 初期状態は「接近」
    {
    }

    // 毎フレーム呼ばれる想定の更新関数
    void Update()
    {
        // 現在の状態関数を実行
        (this->*state_)();
    }

private:
    StateFunc state_; // 現在の状態

    // 接近状態
    void Approach()
    {
        std::cout << "敵は接近している\n";

        // 次の状態を「攻撃」に変更
        state_ = &Enemy::Attack;
    }

    // 攻撃状態
    void Attack()
    {
        std::cout << "敵は攻撃している\n";

        // 次の状態を「離脱」に変更
        state_ = &Enemy::Escape;
    }

    // 離脱状態
    void Escape()
    {
        std::cout << "敵は離脱している\n";

        // 再び接近に戻す（ループさせる）
        state_ = &Enemy::Approach;
    }
};

int main()
{
    Enemy enemy;

    // 状態遷移の確認用に複数回更新
    for (int i = 0; i < 6; ++i) {
        enemy.Update();
    }

    return 0;
}
