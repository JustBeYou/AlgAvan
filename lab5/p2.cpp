#include <bits/stdc++.h>
#include "geometry.hpp"

using namespace std;

int main() {
    int n; cin >> n;
    geometry::Polygon2D poly(n);

    for (int i = 0; i < n; i++) {
        cin >> poly[i];
    }

    // graham scan with points already sorted
    // linear time
    auto convex_hull = geometry::graham_scan(poly, true);

    for (auto p_i: convex_hull) {
        cout << poly[p_i] << "\n";
    }

    return 0;
}
