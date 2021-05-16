#include <bits/stdc++.h>
#include "geometry.hpp"

using namespace std;

int main() {
    geometry::Point2D A, B, C, D;
    cin >> A >> B >> C >> D;

    // AC ilegala <=> D in c. circumscris ACB
    {
        geometry::Segment2D a(C, B), b(A, C), c(B, A);
        auto R = geometry::compute_circumcenter_radius(a, c, b);
        auto U = geometry::compute_circumcenter(a, c, b);  

        if (geometry::Segment2D(U, D).length() < R) {
            cout << A << " - " << C << " este ilegala\n";
        }
    }

    // BD ilegala <=> C in c. circumscris BDA
    {
        geometry::Segment2D b(D, A), d(A, B), a(B, D);
        auto R = geometry::compute_circumcenter_radius(b, d, a);
        auto U = geometry::compute_circumcenter(b, d, a);  

        if (geometry::Segment2D(U, D).length() < R) {
            cout << B << " - " << D << " este ilegala\n";
        }
    }

    return 0;
}