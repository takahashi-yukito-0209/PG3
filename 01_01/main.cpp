#include <iostream>
#include <list>
#include <stdio.h>
using namespace std;

int main()
{

    // 駅一覧をまとめるリスト
    list<const char*> lst {
        "Tokyo", "Kanda", "Akihabara", "Okachimachi", "Ueno",
        "Uguisudani", "Nippori", "Tabata", "Komagome", "Sugamo",
        "Otsuka", "Ikebukuro", "Mejiro", "Takadanobaba", "Shin-Okubo",
        "Shinjuku", "Yoyogi", "Harajuku", "Shibuya", "Ebisu",
        "Meguro", "Gotanda", "Osaki", "Shinagawa", "Tamachi",
        "Hamamatsucho", "Shimbashi", "Yurakucho"
    };

    //  1970年の駅一覧
    cout << "Yamanote Line (1970):\n";
    for (list<const char*>::iterator it = lst.begin(); it != lst.end(); ++it) {
        cout << *it;
        list<const char*>::iterator nextIt = it;
        ++nextIt;
        if (nextIt != lst.end()) {
            cout << " -> ";
        }
    }
    cout << "\n\n";

    // 西日暮里を挿入
    for (list<const char*>::iterator it = lst.begin(); it != lst.end(); ++it) {
        if (strcmp(*it, "Tabata") == 0) {
            lst.insert(it, "Nishi-Nippori");
            break;
        }
    }

    // 2019年の駅一覧
    cout << "Yamanote Line (2019):\n";
    for (list<const char*>::iterator it = lst.begin(); it != lst.end(); ++it) {
        cout << *it;
        list<const char*>::iterator nextIt = it;
        ++nextIt;
        if (nextIt != lst.end()) {
            cout << " -> ";
        }
    }
    cout << "\n\n";

    //  高輪ゲートウェイを挿入
    for (list<const char*>::iterator it = lst.begin(); it != lst.end(); ++it) {
        if (strcmp(*it, "Shinagawa") == 0) {
            lst.insert(it, "Takanawa Gateway");
            break;
        }
    }

    // 2022年の駅一覧
    cout << "Yamanote Line (2022):\n";
    for (list<const char*>::iterator it = lst.begin(); it != lst.end(); ++it) {
        cout << *it;
        list<const char*>::iterator nextIt = it;
        ++nextIt;
        if (nextIt != lst.end()) {
            cout << " -> ";
        }
    }
    cout << "\n\n";

    return 0;
}
