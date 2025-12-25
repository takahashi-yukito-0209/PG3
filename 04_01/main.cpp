#include <Novice.h>
#include <cstring>
#include <memory>

const char kWindowTitle[] = "学籍番号";

//==================================================
// 前方宣言
//==================================================
class SceneManager;
class StageScene;
class ClearScene;

//==================================================
// シーン基底クラス（ステート）
//==================================================
class Scene {
public:
    virtual ~Scene() = default;

    // シーン開始時に一度だけ呼ばれる
    virtual void Enter(SceneManager& manager) = 0;

    // 毎フレーム呼ばれる更新処理
    virtual void Update(SceneManager& manager, const char* keys, const char* preKeys) = 0;

    // 描画処理
    virtual void Draw() = 0;
};

//==================================================
// シーン管理クラス（コンテキスト）
//==================================================
class SceneManager {
public:
    // シーン切り替え
    void ChangeScene(std::unique_ptr<Scene> scene)
    {
        scene_ = std::move(scene);
        scene_->Enter(*this);
    }

    // 更新
    void Update(const char* keys, const char* preKeys)
    {
        if (scene_) {
            scene_->Update(*this, keys, preKeys);
        }
    }

    // 描画
    void Draw()
    {
        if (scene_) {
            scene_->Draw();
        }
    }

private:
    std::unique_ptr<Scene> scene_;
};

//==================================================
// クリアシーン
//==================================================
class ClearScene : public Scene {
public:
    void Enter(SceneManager&) override
    {
    }

    void Update(SceneManager&, const char*, const char*) override
    {
        // 何もしない
    }

    void Draw() override
    {
        Novice::ScreenPrintf(500, 320, "CLEAR SCENE");
    }
};

//==================================================
// ステージシーン（敵を倒す）
//==================================================
class StageScene : public Scene {
public:
    StageScene()
        : enemyCount_(3)
    {
    }

    void Enter(SceneManager&) override
    {
        // 敵数を初期化
        enemyCount_ = 3;
    }

    void Update(SceneManager& manager, const char* keys, const char* preKeys) override
    {
        // SPACEキーで敵を1体倒す
        if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
            enemyCount_--;
        }

        // 敵を全て倒したらクリアへ
        if (enemyCount_ <= 0) {
            manager.ChangeScene(std::unique_ptr<Scene>(new ClearScene()));
        }
    }

    void Draw() override
    {
        Novice::ScreenPrintf(480, 300, "STAGE SCENE");

        char text[64];
        sprintf_s(text, "ENEMY COUNT : %d", enemyCount_);
        Novice::ScreenPrintf(460, 340, "%s", text);

        Novice::ScreenPrintf(380, 380, "PRESS SPACE TO DEFEAT ENEMY");
    }

private:
    int enemyCount_; // 残り敵数
};

//==================================================
// タイトルシーン
//==================================================
class TitleScene : public Scene {
public:
    void Enter(SceneManager&) override
    {
        // 特に初期化はしない
    }

    void Update(SceneManager& manager, const char* keys, const char* preKeys) override
    {
        // SPACEキーでステージへ
        if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
            manager.ChangeScene(std::unique_ptr<Scene>(new StageScene()));
        }
    }

    void Draw() override
    {
        Novice::ScreenPrintf(500, 300, "TITLE SCENE");
        Novice::ScreenPrintf(420, 340, "PRESS SPACE KEY");
    }
};

//==================================================
// WinMain
//==================================================
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{

    // ライブラリ初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // シーン管理クラス
    SceneManager sceneManager;

    // 初期シーンはタイトル
    sceneManager.ChangeScene(std::unique_ptr<Scene>(new TitleScene()));

    // メインループ
    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();

        // 入力更新
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        //=====================
        // 更新処理
        //=====================
        sceneManager.Update(keys, preKeys);

        //=====================
        // 描画処理
        //=====================
        sceneManager.Draw();

        Novice::EndFrame();

        // ESCで終了
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // 終了処理
    Novice::Finalize();
    return 0;
}
