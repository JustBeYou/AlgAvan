#include <bits/stdc++.h>
#include "geometry.hpp"

using namespace std;

int main() {
    int n; cin >> n;
    geometry::Polygon2D poly(n);

    double leftmost_x = std::numeric_limits<double>::max();
    int leftmost_i = 0;

    double rightmost_x = std::numeric_limits<double>::min();
    int rightmost_i = 0;

    for (int i = 0; i < n; i++) {
        cin >> poly[i];

        if (poly[i].x < leftmost_x) {
            leftmost_x = poly[i].x;
            leftmost_i = i;
        } else if (poly[i].x > rightmost_x) {
            rightmost_x = poly[i].x;
            rightmost_i = i;
        }
    }

    int q; cin >> q; 
    for (int i = 0; i < q; i++) {
        double x, y;
        cin >> x >> y;
        geometry::Point2D p({x, y});

        // find wich projection would contain the point
        int seg_bottom_i = geometry::search_x_projection({x, y}, geometry::BOTTOM, poly, leftmost_i, rightmost_i);
        int seg_top_i = geometry::search_x_projection({x, y}, geometry::TOP, poly, leftmost_i, rightmost_i);

        if (seg_bottom_i == -1 or seg_top_i == -1) {
            cout << "outside\n";
            continue;
        }

        // get the associated segment with the projection
        auto seg_bottom = geometry::nth_segment(poly, seg_bottom_i, leftmost_i);
        auto seg_top = geometry::nth_segment(poly, seg_top_i, rightmost_i);

        // check if the point is inside the polygon or not
        auto bottom_position = geometry::point_position(p, seg_bottom);
        auto top_position = geometry::point_position(p, seg_top);

        if (bottom_position == geometry::ON_SEGMENT or top_position == geometry::ON_SEGMENT) {
            cout << "on edge\n";
        } else if (bottom_position == geometry::CLOCKWISE and top_position == geometry::CLOCKWISE) {
            cout << "inside\n";
        } else {
            cout << "outside\n";
        }
    }

    return 0;
}