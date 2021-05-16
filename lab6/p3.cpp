#include <bits/stdc++.h>
#include "geometry.hpp"

using namespace std;

int main() {
    geometry::Point2D A, B, C;
    cin >> A >> B >> C;
    geometry::Segment2D a(B, C), b(A, C), c(A, B);

    auto R = geometry::compute_circumcenter_radius(a, b, c);
    auto U = geometry::compute_circumcenter(a, b, c); 

    int n; cin >> n;
    while (n--) {
        geometry::Point2D Q; cin >> Q;
        geometry::Segment2D q(Q, U);
        auto l = q.length();

        cout << "(" << Q.x << ", " << Q.y << ") - ";
        if (geometry::is_equal(l, R)) {
            cout << "pe cerc\n";
        } else if (l > R) {
            cout << "in exterior\n";
        } else {
            cout << "in interior\n";
        }
    }

    return 0;
}