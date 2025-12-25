#include <iostream>
#include <memory>

// 抽象クラス（インターフェース）
class IShape {
public:
    // 抽象クラスとして扱うため、仮想デストラクタを定義
    virtual ~IShape() = default;

    // 面積を計算する純粋仮想関数
    virtual void Size() const = 0;

    // 面積の値を表示する純粋仮想関数
    virtual void Draw() const = 0;
};

// Circle クラス（円）
class Circle : public IShape {
public:
    // 半径を受け取るコンストラクタ
    Circle(double radius)
        : radius_(radius)
        , area_(0.0)
    {
    }

    // 円の面積を計算する
    void Size() const override
    {
        area_ = 3.1415926535 * radius_ * radius_;
    }

    // 計算された面積をコンソールに表示する
    void Draw() const override
    {
        std::cout << "円の面積: " << area_ << std::endl;
    }

private:
    double radius_; // 半径
    mutable double area_; // 面積
};

// Rectangle クラス（長方形）
class Rectangle : public IShape {
public:
    // 幅と高さを受け取るコンストラクタ
    Rectangle(double width, double height)
        : width_(width)
        , height_(height)
        , area_(0.0)
    {
    }

    // 長方形の面積を計算する
    void Size() const override
    {
        area_ = width_ * height_;
    }

    // 計算された面積をコンソールに表示する
    void Draw() const override
    {
        std::cout << "長方形の面積: " << area_ << std::endl;
    }

private:
    double width_; // 幅
    double height_; // 高さ
    mutable double area_; // 面積
};

int main()
{

    // 抽象クラス IShape 型でCircle と Rectangle を扱う
    std::unique_ptr<IShape> shape1 = std::make_unique<Circle>(5.0);
    std::unique_ptr<IShape> shape2 = std::make_unique<Rectangle>(4.0, 6.0);

    // 円
    shape1->Size();
    shape1->Draw();

    // 長方形
    shape2->Size();
    shape2->Draw();

    return 0;
}
