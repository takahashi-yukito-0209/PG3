#include <iostream>
#include <list>
#include <stdio.h>
using namespace std;

int main()
{

    // ‰wˆê——‚ð‚Ü‚Æ‚ß‚éƒŠƒXƒg
    list<const char*> lst {
        "Tokyo", "Kanda", "Akihabara", "Okachimachi", "Ueno",
        "Uguisudani", "Nippori", "Tabata", "Komagome", "Sugamo",
        "Otsuka", "Ikebukuro", "Mejiro", "Takadanobaba", "Shin-Okubo",
        "Shinjuku", "Yoyogi", "Harajuku", "Shibuya", "Ebisu",
        "Meguro", "Gotanda", "Osaki", "Shinagawa", "Tamachi",
        "Hamamatsucho", "Shimbashi", "Yurakucho"
    };

    //  1970”N‚Ì‰wˆê——
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

    // ¼“ú•é—¢‚ð‘}“ü
    for (list<const char*>::iterator it = lst.begin(); it != lst.end(); ++it) {
        if (strcmp(*it, "Tabata") == 0) {
            lst.insert(it, "Nishi-Nippori");
            break;
        }
    }

    // 2019”N‚Ì‰wˆê——
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

    //  ‚—ÖƒQ[ƒgƒEƒFƒC‚ð‘}“ü
    for (list<const char*>::iterator it = lst.begin(); it != lst.end(); ++it) {
        if (strcmp(*it, "Shinagawa") == 0) {
            lst.insert(it, "Takanawa Gateway");
            break;
        }
    }

    // 2022”N‚Ì‰wˆê——
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
