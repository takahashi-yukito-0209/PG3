#include <Novice.h>

#include <algorithm>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

const char kWindowTitle[] = "Command Pattern Advanced";

// コマンドインターフェース
struct ICommand {
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual ~ICommand() { }
};

// ユニット情報
struct Unit {
    int x;
    int y; 
};

// ユニット移動コマンド
struct MoveUnitCommand : public ICommand {
    std::vector<Unit>* units;
    int index;
    int dx, dy;

    // 移動量
    MoveUnitCommand(std::vector<Unit>* u, int idx, int dx_, int dy_)
        : units(u)
        , index(idx)
        , dx(dx_)
        , dy(dy_)
    {
    }

    // 実行
    void Execute() override
    {
        if (index < 0 || index >= (int)units->size())
            return;
        (*units)[index].x += dx;
        (*units)[index].y += dy;
    }

    // 元に戻す    
    void Undo() override
    {
        if (index < 0 || index >= (int)units->size())
            return;
        (*units)[index].x -= dx;
        (*units)[index].y -= dy;
    }
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    const int tileSize = 32;
    const int columnCount = 1280 / tileSize; // 40
    const int rowCount = 18; // 下部にUI用のスペースを残す

    // 初期ユニット配置
    std::vector<Unit> unitList = { { 10, 6 }, { 15, 8 }, { 36, 5 }, { 30, 14 } };

    
    int selectorGridX = columnCount / 2;
    int selectorGridY = rowCount / 2;
    
    enum Mode { 
        SelectorMode,
        UnitMode 
    } 
    currentMode = SelectorMode;

    bool isUnitGrabbed = false;
    int grabbedUnitIndex = -1;
    int grabbedUnitOriginalGridX = -1;
    int grabbedUnitOriginalGridY = -1;
    int grabbedMoveCount = 0;

    int maxUndoCount = 0;
    std::vector<std::unique_ptr<ICommand>> undoCommandStack;

    // 指定グリッドにユニットがいるか調べる
    auto FindUnitIndexAt = [&](int gridX, int gridY) -> int {
        for (int unitIndex = 0; unitIndex < (int)unitList.size(); ++unitIndex) {
            if (unitList[unitIndex].x == gridX && unitList[unitIndex].y == gridY) {
                return unitIndex;
            }
        }
        return -1;
    };

    // 指定グリッドにユニットを移動できるか（範囲内かつ重なりなし）
    auto CanMoveUnitTo = [&](int gridX, int gridY) -> bool {
        if (gridX < 0 || gridX >= columnCount || gridY < 0 || gridY >= rowCount) {
            return false;
        }
        for (auto& unit : unitList) {
            if (unit.x == gridX && unit.y == gridY) {
                return false;
            }
        }
        return true;
    };

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        // 入力処理 (トリガーで処理する)
        auto KeyDown = [&](int dik) { return preKeys[dik] == 0 && keys[dik] != 0; };

        // Spaceキー: セレクトモード切替、またはユニットモード時は掴む/離す
        if (KeyDown(DIK_SPACE)) {
            if (currentMode == SelectorMode) {
                // ユニットモードに切り替え、直ちに掴めるか試みる
                currentMode = UnitMode;
                int foundUnitIndex = FindUnitIndexAt(selectorGridX, selectorGridY);
                if (foundUnitIndex != -1) {
                    // ユニットを掴む
                    isUnitGrabbed = true;
                    grabbedUnitIndex = foundUnitIndex;
                    grabbedUnitOriginalGridX = unitList[foundUnitIndex].x;
                    grabbedUnitOriginalGridY = unitList[foundUnitIndex].y;
                    // 移動回数カウンタをリセット
                    grabbedMoveCount = 0;
                } else {
                    // 掴んでいない状態にする
                    isUnitGrabbed = false;
                    grabbedUnitIndex = -1;
                }
            } else {
                // ユニットモード: 掴んでいない場合はセレクタ位置のユニットを掴めるか試す
                if (!isUnitGrabbed) {
                    int foundUnitIndex = FindUnitIndexAt(selectorGridX, selectorGridY);
                    if (foundUnitIndex != -1) {
                        // ユニットを掴む
                        isUnitGrabbed = true;
                        grabbedUnitIndex = foundUnitIndex;
                        grabbedUnitOriginalGridX = unitList[foundUnitIndex].x;
                        grabbedUnitOriginalGridY = unitList[foundUnitIndex].y;
                        // 移動回数カウンタをリセット
                        grabbedMoveCount = 0;
                    }
                } else {
                    // 離す（配置）
                    if (grabbedUnitIndex != -1) {
                        int newGridX = selectorGridX;
                        int newGridY = selectorGridY;
                        bool ok = (newGridX == grabbedUnitOriginalGridX && newGridY == grabbedUnitOriginalGridY) || CanMoveUnitTo(newGridX, newGridY);
                        if (ok) {
                            int deltaX = newGridX - grabbedUnitOriginalGridX;
                            int deltaY = newGridY - grabbedUnitOriginalGridY;
                            if (deltaX != 0 || deltaY != 0) {
                                // 実際に移動を実行
                                MoveUnitCommand cmd(&unitList, grabbedUnitIndex, deltaX, deltaY);
                                cmd.Execute();
                                // undoを保存するのは許可されている場合のみ
                                if (maxUndoCount > 0) {
                                    auto up = std::make_unique<MoveUnitCommand>(&unitList, grabbedUnitIndex, deltaX, deltaY);
                                    undoCommandStack.push_back(std::move(up));
                                    if ((int)undoCommandStack.size() > maxUndoCount) {
                                        undoCommandStack.erase(undoCommandStack.begin());
                                    }
                                }
                            }
                            // 成功した配置 -> ユニットは新しい位置にいる
                        } else {
                            // 配置失敗 -> ユニットは元の位置に戻す
                        }
                    }
                    // 配置後はセレクタモードに戻し、掴み状態をリセットする
                    currentMode = SelectorMode;
                    isUnitGrabbed = false;
                    grabbedUnitIndex = -1;
                    grabbedMoveCount = 0; // リセット
                }
            }
        }

        // 移動方向を決定
        int moveDeltaX = 0, moveDeltaY = 0;
        if (KeyDown(DIK_W) || KeyDown(DIK_UP)) {
            moveDeltaY = -1;
        }
        if (KeyDown(DIK_S) || KeyDown(DIK_DOWN)) {
            moveDeltaY = 1;
        }
        if (KeyDown(DIK_A) || KeyDown(DIK_LEFT)) {
            moveDeltaX = -1;
        }
        if (KeyDown(DIK_D) || KeyDown(DIK_RIGHT)) {
            moveDeltaX = 1;
        }

        if (moveDeltaX != 0 || moveDeltaY != 0) {
            // 移動は常にセレクタを動かす（ただし掴んでいるときは重なり禁止ルールを適用）。
            int oldSelectorGridX = selectorGridX;
            int oldSelectorGridY = selectorGridY;

            int intendedGridX = selectorGridX + moveDeltaX;
            int intendedGridY = selectorGridY + moveDeltaY;
            if (intendedGridX < 0) {
                intendedGridX = 0;
            }
            if (intendedGridX >= columnCount) {
                intendedGridX = columnCount - 1;
            }
            if (intendedGridY < 0) {
                intendedGridY = 0;
            }
            if (intendedGridY >= rowCount) {
                intendedGridY = rowCount - 1;
            }

            bool allowSelectorMove = true;
            if (currentMode == UnitMode && isUnitGrabbed && grabbedUnitIndex != -1) {
                // 掴んでいるときは、元の位置へ戻すことは常に許可する。
                if (intendedGridX == grabbedUnitOriginalGridX && intendedGridY == grabbedUnitOriginalGridY) {
                    allowSelectorMove = true;
                } else {
                    // 他のユニットと重なる位置には移動できない
                    allowSelectorMove = CanMoveUnitTo(intendedGridX, intendedGridY);
                }
            }

            if (allowSelectorMove) {
                selectorGridX = intendedGridX;
                selectorGridY = intendedGridY;

                // 掴んでいる間かつセレクタの位置が実際に変化したときにのみ移動回数をカウントする
                if (currentMode == UnitMode && isUnitGrabbed && grabbedUnitIndex != -1) {
                    if (selectorGridX != oldSelectorGridX || selectorGridY != oldSelectorGridY) {
                        grabbedMoveCount += 1;
                    }
                }
            }

            // 掴んだユニットは見た目上セレクタに追従する。ここでunitList配列は変更しない。
            // 実際の移動は離したときにコマンドとして記録する。
        }

        // Undo (Ctrl+Z)
        bool ctrl = keys[DIK_LCONTROL] != 0 || keys[DIK_RCONTROL] != 0;
        if (ctrl && KeyDown(DIK_Z)) {
            // SelectorモードではUndoできない
            if (currentMode == UnitMode && !undoCommandStack.empty()) {
                undoCommandStack.back()->Undo();
                undoCommandStack.pop_back();
            }
        }

        ///
        /// 描画処理
        ///

        // 背景をタイル状に描画
        for (int y = 0; y < rowCount; ++y) {
            for (int x = 0; x < columnCount; ++x) {
                int pixelX = x * tileSize;
                int pixelY = y * tileSize;
                // 塗り
                Novice::DrawBox(pixelX, pixelY, tileSize, tileSize, 0.0f, 0x6A7FAFFF, kFillModeSolid);
                // 枠線
                Novice::DrawBox(pixelX, pixelY, tileSize, tileSize, 0.0f, BLACK, kFillModeWireFrame);
            }
        }

        // ユニットを描画
        for (int unitIndex = 0; unitIndex < (int)unitList.size(); ++unitIndex) {
            // 掴んでいるユニットは描画をスキップして後でセレクタ位置に描画する
            if (isUnitGrabbed && unitIndex == grabbedUnitIndex) {
                continue;
            }
            int unitPixelX = unitList[unitIndex].x * tileSize;
            int unitPixelY = unitList[unitIndex].y * tileSize;
            int unitMargin = 6;
            // ユニットモード: セレクタがこのユニットの上にあり掴んでいない場合はハイライト(ピンク)
            if (currentMode == UnitMode && !isUnitGrabbed && selectorGridX == unitList[unitIndex].x && selectorGridY == unitList[unitIndex].y) {
                Novice::DrawBox(unitPixelX + unitMargin, unitPixelY + unitMargin, tileSize - unitMargin * 2, tileSize - unitMargin * 2,
                    0.0f, 0xFFC0C0FF, kFillModeSolid);
            } else {
                Novice::DrawBox(unitPixelX + unitMargin, unitPixelY + unitMargin, tileSize - unitMargin * 2, tileSize - unitMargin * 2,
                    0.0f, WHITE, kFillModeSolid);
            }
        }

        // 掴んでいるユニットをセレクタ位置に描画（実際の units 配列は変更しない）
        if (isUnitGrabbed && grabbedUnitIndex != -1) {
            int unitPixelX = selectorGridX * tileSize;
            int unitPixelY = selectorGridY * tileSize;
            int unitMargin = 6;
            Novice::DrawBox(unitPixelX + unitMargin, unitPixelY + unitMargin, tileSize - unitMargin * 2, tileSize - unitMargin * 2,
                0.0f, 0xFFC0C0FF, kFillModeSolid);
        }

        // セレクタ描画
        int selPx = selectorGridX * tileSize;
        int selPy = selectorGridY * tileSize;
        // 検出されるユニットがあれば赤枠で強調
        int selUnit = FindUnitIndexAt(selectorGridX, selectorGridY);
        if (currentMode == SelectorMode) {
            // セレクタは淡い枠
            Novice::DrawBox(selPx, selPy, tileSize, tileSize, 0.0f, RED, kFillModeWireFrame);
        } else {
            // ユニットモード: 掴んでいるときは周りの赤枠を表示しない
            if (isUnitGrabbed) {
                // セレクタは淡い枠
            } else {
                // ユニットがある場所を赤枠で囲む
                if (selUnit != -1) {
                    int ux = unitList[selUnit].x * tileSize;
                    int uy = unitList[selUnit].y * tileSize;
                    Novice::DrawBox(ux - 2, uy - 2, tileSize + 4, tileSize + 4, 0.0f, RED,
                        kFillModeWireFrame);
                } else {
                    // ユニットが無ければセレクタ位置に小さな赤枠
                    Novice::DrawBox(selPx, selPy, tileSize, tileSize, 0.0f, RED, kFillModeWireFrame);
                }
            }
        }

        // UI テキスト
        int textY = rowCount * tileSize + 10;
        if (currentMode == SelectorMode) {
            Novice::ScreenPrintf(16, textY, "WASD | arrow keys: move / space key: change selector mode");
            Novice::ScreenPrintf(16, textY + 26, "You are in Selector Mode, you cannot use the 'Undo' action.");
        } else {
            if (isUnitGrabbed) {
                Novice::ScreenPrintf(16, textY, "WASD | arrow keys: move / space key: change selector mode / ctrl+z: undo");
                Novice::ScreenPrintf(16, textY + 26, "You have %d more 'UnDo' actions available.", grabbedMoveCount);
            } else {
                Novice::ScreenPrintf(16, textY, "WASD | arrow keys: move / space key: change selector mode");
                Novice::ScreenPrintf(16, textY + 26, "You are in Selector Mode, you cannot use the 'Undo' action.");
            }
        }

        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}
