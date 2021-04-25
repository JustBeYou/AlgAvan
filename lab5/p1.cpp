#include <bits/stdc++.h>
#include "geometry.hpp"

using namespace std;

int main() {
    int t; cin >> t;
    while (t--) {
        geometry::Point2D p, q, r;
        cin >> p >> q >> r;

        auto result = geometry::compute_orientation(p, q, r);
        switch (result) {
            case geometry::COUNTERCLOCKWISE:
                cout << "stanga\n";
                break;
            case geometry::CLOCKWISE:
                cout << "dreapta\n";
                break;
            default:
                cout << "coliniare\n";
                break;
        }
    }

    return 0;
}
