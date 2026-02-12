#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iomanip>

// マップデータを保持する型
using Map = std::vector<std::vector<int>>;

std::mutex g_mutex;
std::condition_variable g_cv;
bool g_loaded = false;
Map g_map;

// CSVを読み込む関数（バックグラウンドスレッドで実行）
void loadCsv(const std::string& path)
{
    Map local;

    std::ifstream ifs(path);
    if (!ifs) {
        // ファイルが無ければサンプルを作成して読み直す
        std::ofstream ofs(path);
        ofs << "0,0,1,1,1,0,0\n";
        ofs << "0,1,1,2,1,1,0\n";
        ofs << "1,1,2,2,2,1,1\n";
        ofs << "1,2,2,2,2,2,1\n";
        ofs << "0,1,1,2,1,1,0\n";
        ofs.close();
        ifs.open(path);
    }

    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string token;
        std::vector<int> row;
        while (std::getline(ss, token, ',')) {
            try {
                int val = std::stoi(token);
                row.push_back(val);
            } catch (...) {
                // 非数値は0とみなす
                row.push_back(0);
            }
        }
        if (!row.empty()) local.push_back(std::move(row));
        // 模擬的に読み込みに時間をかける（ロード進行の可視化やスレッド感の演出）
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // 読み込み完了：共有データに移し替えて通知
    {
        std::lock_guard<std::mutex> lk(g_mutex);
        g_map = std::move(local);
        g_loaded = true;
    }
    g_cv.notify_one();
}

// マップチップ表示（CSVの数字と同じ値を表示する）
void renderMap(const Map& map)
{
    for (const auto& row : map) {
        for (size_t i = 0; i < row.size(); ++i) {
            // 数字をそのまま表示し、見やすいように幅を揃える
            std::cout << std::setw(2) << row[i];
            if (i + 1 < row.size()) std::cout << ' ';
        }
        std::cout << '\n';
    }
}

int main()
{
    const std::string path = "map.csv";

    // バックグラウンドでCSVを読み込むスレッドを起動
    std::thread loader(loadCsv, path);

    // メインスレッドは読み込み完了を待ち、必要に応じて他の処理を行える
    std::cout << "マップをバックグラウンドで読み込み中..." << std::endl;

    // 読み込み完了まで待機（条件変数で効率的に待つ）
    {
        std::unique_lock<std::mutex> lk(g_mutex);
        g_cv.wait(lk, [] { return g_loaded; });
    }

    std::cout << "読み込み完了。マップチップを表示します:\n" << std::endl;

    // 読み込んだマップを表示
    renderMap(g_map);

    // スレッドを結合して終了
    loader.join();
    return 0;
}
