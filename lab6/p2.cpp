#include <bits/stdc++.h>
#include "geometry.hpp"

using namespace std;

bool x_monotone(const geometry::Polygon2D& poly) {
    bool ascending = poly[0].x < poly[1].x;
    int dir_changes = 0;
    size_t n = poly.size();
    for (size_t i = 1; i != 0; i = (i+1) % n) {
        if (ascending and poly[i].x > poly[(i+1)%n].x) {
            ascending = false;
            dir_changes++;
        } else if (not ascending and poly[i].x < poly[(i+1)%n].x) {
            ascending = true;
            dir_changes++;
        }
    }

    return dir_changes == 2;
}

bool y_monotone(const geometry::Polygon2D& poly) {
    bool ascending = poly[0].y < poly[1].y;
    int dir_changes = 0;
    size_t n = poly.size();
    for (size_t i = 1; i != 0; i = (i+1) % n) {
        if (ascending and poly[i].y > poly[(i+1)%n].y) {
            ascending = false;
            dir_changes++;
        } else if (not ascending and poly[i].y < poly[(i+1)%n].y) {
            ascending = true;
            dir_changes++;
        }
    }

    return dir_changes == 2;;
}

int main() {
    int t; cin >> t;
    int j = 1;
    while (t--) {
        int n; cin >> n;
        geometry::Polygon2D poly(n);

        for (int i = 0; i < n; i++) cin >> poly[i];

        printf("Poligonul %d:\n", j++);
        printf("%s x-monoton\n", x_monotone(poly) ? "este" : "nu este");
        printf("%s y-monoton\n", y_monotone(poly) ? "este" : "nu este");
        printf("\n");       
    }

    return 0;
}