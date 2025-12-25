#include <iostream>
#include <memory>

// ===== 基底クラス =====
// 現実世界の「動物」を表すクラス
class Animal {
public:
    // 仮想デストラクタ
    // 派生クラスをポリモーフィズムで扱うために必須
    virtual ~Animal() = default;

    // 鳴き声を表す仮想関数
    // 派生クラスごとに振る舞いが変わる
    virtual void Speak() const
    {
        std::cout << "動物が鳴く\n";
    }
};

// ===== 派生クラス =====
// 犬クラス
class Dog : public Animal {
public:
    // Animal::Speak をオーバーライド
    void Speak() const override
    {
        std::cout << "犬：ワンワン\n";
    }
};

// 猫クラス
class Cat : public Animal {
public:
    // Animal::Speak をオーバーライド
    void Speak() const override
    {
        std::cout << "猫：ニャーニャー\n";
    }
};

int main()
{
    // 基底クラス型のポインタで派生クラスのインスタンスを扱う
    std::unique_ptr<Animal> animal1 = std::make_unique<Dog>();
    std::unique_ptr<Animal> animal2 = std::make_unique<Cat>();

    // 同じ Animal 型だが、呼ばれる関数は実体の型によって変わる
    animal1->Speak(); // Dog の Speak が呼ばれる
    animal2->Speak(); // Cat の Speak が呼ばれる

    return 0;
}
