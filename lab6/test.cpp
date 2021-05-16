#include <bits/stdc++.h>
#include "geometry.hpp"

using namespace std;


int main() {
    cout << (geometry::segments_position(
        geometry::Segment2D({0, 0}, {3, 3}),
        geometry::Segment2D({1, 1}, {2, 2})
    ) == geometry::INTERSECTING) << endl;

    return 0;
}