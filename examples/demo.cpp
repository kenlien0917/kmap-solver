#define main coursework_demo_main
#include "../src/Kmap.cpp"
#undef main

// Added portfolio example. The submitted solver is included without edits.
int main() {
    int n = 4;
    vector<int> on{0, 4, 5, 10, 11};
    vector<int> dc{1, 13, 14, 15};
    auto* sol = solveKmap(n, on, dc);
    vector<vector<string>> forms{sol->minimumSop};
    for (const auto& sop : forms) {
        cout << "SOP: ";
        if (sop.empty()) cout << "0";
        for (size_t i=0; i<sop.size(); ++i) {
            if (i) cout << " + ";
            cout << (sop[i].empty() ? "1" : sop[i]);
        }
        cout << '\n';
    }
    for (int r=0; r<(n==4?4:2); ++r) delete[] sol->kmap[r];
    delete[] sol->kmap;
    delete sol;
}
