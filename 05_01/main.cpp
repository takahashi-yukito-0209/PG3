#include <Novice.h>
#include <cstring> 
#include <memory>


const char kWindowTitle[] = "学籍番号";

// Player クラス
class Player {
public:
    // コンストラクタ
    Player(int x, int y)
        : x_(x)
        , y_(y)
        , halfWidth_(32)
        , halfHeight_(32)
    {
    }

    // 移動処理
    void Move(int dx)
    {
        // x座標を更新
        x_ += dx;

        // 画面左端からはみ出さないように制限
        if (x_ - halfWidth_ < 0) {
            x_ = halfWidth_;
        }

        // 画面右端からはみ出さないように制限
        if (x_ + halfWidth_ > 1280) {
            x_ = 1280 - halfWidth_;
        }
    }

    // 描画処理
    void Draw() const
    {
        // 矩形としてプレイヤーを描画
        Novice::DrawBox(
            x_ - halfWidth_, // 左上X
            y_ - halfHeight_, // 左上Y
            halfWidth_ * 2, // 幅
            halfHeight_ * 2, // 高さ
            0.0f, // 回転角
            WHITE, // 色
            kFillModeSolid // 塗りつぶし
        );
    }

private:
    int x_; // 中心X座標
    int y_; // 中心Y座標
    int halfWidth_; // 半分の幅
    int halfHeight_; // 半分の高さ
};


// Command 抽象クラス
class Command {
public:
    virtual ~Command() = default;

    // 実行処理
    virtual void Execute() = 0;
};

// 左移動コマンド
class MoveLeftCommand : public Command {
public:
    // receiver : 操作対象（Player）
    // speed    : 移動速度
    MoveLeftCommand(Player* receiver, int speed)
        : receiver_(receiver)
        , speed_(speed)
    {
    }

    // 左方向へ移動を実行
    void Execute() override
    {
        receiver_->Move(-speed_);
    }

private:
    Player* receiver_; // 操作対象の Player
    int speed_; // 移動量
};

// 右移動コマンド
class MoveRightCommand : public Command {
public:
    // receiver : 操作対象（Player）
    // speed    : 移動速度
    MoveRightCommand(Player* receiver, int speed)
        : receiver_(receiver)
        , speed_(speed)
    {
    }

    // 右方向へ移動を実行
    void Execute() override
    {
        receiver_->Move(speed_);
    }

private:
    Player* receiver_; // 操作対象の Player
    int speed_; // 移動量
};

// ==================================================
// Windows アプリケーションのエントリーポイント
// ==================================================
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{

    // Novice ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // 現在のキー入力状態
    char keys[256] = { 0 };
    // 1フレーム前のキー入力状態
    char preKeys[256] = { 0 };

    // Playerを生成
    Player player(640, 360);

    // コマンドオブジェクトを生成
    // 入力処理とは独立したオブジェクトとして存在する
    MoveLeftCommand leftCmd(&player, 5);
    MoveRightCommand rightCmd(&player, 5);

    // メインループ
    while (Novice::ProcessMessage() == 0) {

        // フレーム開始
        Novice::BeginFrame();

        // 前フレームのキー状態を保存
        memcpy(preKeys, keys, 256);

        // 現在のキー状態を取得
        Novice::GetHitKeyStateAll(keys);

        // ====================
        // 更新処理
        // ====================

        // Aキーが押されている間、左移動コマンドを実行
        if (keys[DIK_A] != 0) {
            leftCmd.Execute();
        }

        // Dキーが押されている間、右移動コマンドを実行
        if (keys[DIK_D] != 0) {
            rightCmd.Execute();
        }

        // ====================
        // 描画処理
        // ====================

        // プレイヤーを描画
        player.Draw();

        // フレーム終了
        Novice::EndFrame();

        // ESCキーが押されたら終了
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // Novice ライブラリの終了処理
    Novice::Finalize();
    return 0;
}
