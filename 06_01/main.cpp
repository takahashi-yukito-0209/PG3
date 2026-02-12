#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

int main()
{
    // ミューテックスと条件変数を用意する
    std::mutex m;
    std::condition_variable cv;
    // stage はどのスレッドが実行するかを示す状態変数
    // 1 -> thread1, 2 -> thread2, 3 -> thread3
    int stage = 1;

    // スレッド1: stage が 1 になったら実行する
    std::thread t1([&]{
        std::unique_lock<std::mutex> lk(m);
        // 条件を満たすまで待機する（スプリアスウェイクアップ対策あり）
        cv.wait(lk, [&]{ return stage == 1; });
        std::cout << "thread 1" << std::endl;
        // 次のスレッドに進める
        stage = 2;
        lk.unlock();
        // 待機中のスレッドに通知する
        cv.notify_all();
    });

    // スレッド2: stage が 2 になったら実行する
    std::thread t2([&]{
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&]{ return stage == 2; });
        std::cout << "thread 2" << std::endl;
        stage = 3;
        lk.unlock();
        cv.notify_all();
    });

    // スレッド3: stage が 3 になったら実行する
    std::thread t3([&]{
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&]{ return stage == 3; });
        std::cout << "thread 3" << std::endl;
        // 完了
    });

    // 全てのスレッドの終了を待つ
    t1.join();
    t2.join();
    t3.join();

    return 0;
}
