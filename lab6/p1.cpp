#include <bits/stdc++.h>
#include "geometry.hpp"

using namespace std;

geometry::Point2D random_point_outside(double max_coord, const geometry::Polygon2D& poly) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distr(max_coord, max_coord*2);

    geometry::Point2D M;
    while (true) {
        M = geometry::Point2D(distr(gen), distr(gen));

        bool not_colinear = true;
        for (size_t i = 0; i < poly.size() - 1; i++) {
            not_colinear = not_colinear and (geometry::compute_orientation(
                M,
                geometry::nth_segment(poly, i)
            ) != geometry::COLINEAR);
        }

        if (not_colinear) break;
    }
    return M;
}

int main() {
    int n; cin >> n;
    geometry::Polygon2D poly(n);

    double max_coord = numeric_limits<double>::min();

    for (int i = 0; i < n; i++) {
        cin >> poly[i];

        max_coord = max(max_coord, poly[i].x);
        max_coord = max(max_coord, poly[i].y);
    }

    auto M = random_point_outside(max_coord, poly);

    int t; cin >> t;
    while (t--) {
        geometry::Point2D q; cin >> q;

        int intersect = 0;
        bool colinear_but_outside = false;
        bool on_segment = false;

        auto S1 = geometry::Segment2D(M, q);
        for (int i = 0; i < n - 1; i++) {
            auto S2 = geometry::nth_segment(poly, i);
            auto orientation = geometry::point_position(q, S2);

            on_segment = orientation == geometry::ON_SEGMENT;
            colinear_but_outside = orientation == geometry::COLINEAR_OUTSIDE_SEGMENT;
            if (on_segment or colinear_but_outside) break;

            if (geometry::segments_position(
                S1, S2
            ) == geometry::INTERSECTING) intersect++;
        }

        cout << "(" << q.x << ", " << q.y << ") - ";
        if (on_segment) {
            cout << "pe una din laturi\n";
        } else if (colinear_but_outside) {
            cout << "exterior\n";
        } else if (intersect % 2 == 1) {
            cout << "interior\n";
        } else {
            cout << "exterior\n";
        }
    }

    return 0;
}