#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <limits>

namespace geometry {
    // real value such that |x - y| < precision => x = y
    const double precision = 10e-5;
    bool is_equal(const double x, const double y) {
        return std::abs(x - y) < precision;
    }

    // 2-dimensional point with arbitrary origin
    struct Point2D {
        double x = 0, y = 0;
        double r = 0, theta = 0;
        double x_base = 0, y_base = 0;

        Point2D() {}

        Point2D(const double x, const double y, const double x_base = 0, const double y_base = 0) {
            this->x = x;
            this->y = y;
            this->x_base = x_base;
            this->y_base = y_base;
            __compute();
        }

        void adjust_base(const double x_base, const double y_base) {
            this->x_base = x_base;
            this->y_base = y_base;
            __compute();
        }

        // calculate polar coordinates
        void __compute() {
            this->r = std::sqrt(std::pow(x - x_base, 2) + std::pow(y - y_base, 2));
            this->theta = std::atan2(y - y_base, x - x_base);
        }

        // order by polar angle (used for graham scan)
        bool operator<(const Point2D& other) const {
            if (is_equal(this->theta, other.theta)) return this->r < other.r;
            return this->theta < other.theta;
        }
    };

    std::istream& operator>>(std::istream& in, Point2D& point) {
        in >> point.x >> point.y;
        point.__compute();
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const Point2D& point) {
        out << point.x << " " << point.y;
        return out;
    }

    enum Position {
        // poisition of 3 points
        COUNTERCLOCKWISE,
        CLOCKWISE,
        COLINEAR,

        // position of point vs segment
        ON_SEGMENT,
        COLINEAR_OUTSIDE_SEGMENT,
    };

    double euclidean_distance(const Point2D& p1, const Point2D& p2) {
        return std::sqrt(
            std::pow(p2.x - p1.x, 2) +
            std::pow(p2.y - p1.y, 2)
        );
    }

    Position compute_orientation(const Point2D& p, const Point2D& q, const Point2D& r) {
        double result = (q.y - p.y) * (r.x - q.x) -
                        (q.x - p.x) * (r.y - q.y);
        if (std::abs(result) < precision) return COLINEAR;
        return result > 0 ? CLOCKWISE : COUNTERCLOCKWISE;
    }

    // Distance between two elements of a circular list (from left to right)
    int modulo_distance(int a, int b, int n) {
        if (a < b) return b - a;
        return (n - a) + b;
    }

    struct Segment2D {
        Point2D a = Point2D(0, 0), b = Point2D(0, 0);
    };

    typedef std::vector<Point2D> Polygon2D;

    // get the n-th segment of a polygon
    Segment2D nth_segment(const Polygon2D& poly, int n, int start = 0) {
        return Segment2D {
            .a = poly[(start + n) % poly.size()],
            .b = poly[(start + n + 1) % poly.size()]       
        };
    }

    bool on_segment_x(const Point2D& p, const Segment2D& s) {
        return (s.a.x - precision <= p.x) and (p.x <= s.b.x + precision);
    }

    bool on_segment_y(const Point2D& p, const Segment2D& s) {
        return (s.a.y - precision <= p.y) and (p.y <= s.b.y + precision);
    }

    bool is_vertical(const Segment2D& seg) {
        return seg.a.x == seg.b.x;
    }

    Position point_position(const Point2D& p, const Segment2D& seg) {
        auto orientation = compute_orientation(seg.a, p, seg.b);
        
        // it is important if the segment is vertical or not
        if (orientation == COLINEAR and is_vertical(seg) and on_segment_y(p, seg)) return ON_SEGMENT;
        else if (orientation == COLINEAR and not is_vertical(seg) and on_segment_x(p, seg)) return ON_SEGMENT;
        else if (orientation == COLINEAR) return COLINEAR_OUTSIDE_SEGMENT;
        return orientation;
    }

    enum SearchArea {
        BOTTOM,
        TOP,
    };

    // project top or bottom edges of a polygon on Ox then
    // search on which projection would sit the point
    int search_x_projection(Point2D p, SearchArea area, Polygon2D& poly, int leftmost, int rightmost) {
        if (area == TOP) std::swap(leftmost, rightmost);
        int n = poly.size();
        int segments = modulo_distance(leftmost, rightmost, n);

        int i = 0, j = segments - 1; 
        while (i <= j) {
            int mid = (i + j) / 2;
            auto s = nth_segment(poly, mid, leftmost); 
            if (area == TOP) std::swap(s.a, s.b); 

            if (on_segment_x(p, s)) {
                // if the point is exactly on the margin, return the
                // neighbouring segment if it is vertical
                if (mid > 0 and is_equal(p.x, s.a.x)) {
                    auto before = nth_segment(poly, mid - 1, leftmost);
                    if (is_vertical(before)) return mid - 1;
                }
                
                if (mid < segments - 1 and is_equal(p.x, s.b.x)) {
                    auto after = nth_segment(poly, mid + 1, leftmost);
                    if (is_vertical(after)) return mid + 1;
                }

                return mid;
            } else if (p.x < s.a.x) {
                if (area == TOP) i = mid + 1;
                else             j = mid - 1;
            } else if (s.b.x < p.x) {
                if (area == TOP) j = mid - 1;
                else             i = mid + 1;
            }
        }

        return -1;
    }

    void inplace_trigonometric_sort(Polygon2D& poly, const Point2D base) {
        for (unsigned int i = 0; i < poly.size(); i++) poly[i].adjust_base(base.x, base.y);
        sort(poly.begin(), poly.end());
    }

    std::list<int> graham_scan(Polygon2D& poly, bool is_sorted = false) {
        int n = poly.size(); 
        double leftmost_x = std::numeric_limits<double>::max();
        double leftmost_y = std::numeric_limits<double>::max();
        int leftmost_i = 0;

        // if the polygon is already sorted in trigonometric order
        // this will run in linear time
        if (is_sorted) {
            for (int i = 0; i < n; i++) {
                if (poly[i].x < leftmost_x) {
                    leftmost_x = poly[i].x;
                    leftmost_i = i;
                }
            }
        } else {
            for (int i = 0; i < n; i++ ) {
                if (poly[i].y < leftmost_y or (is_equal(poly[i].y, leftmost_y) and poly[i].x < leftmost_x)) {
                    leftmost_x = poly[i].x;
                    leftmost_y = poly[i].y;
                    leftmost_i = i;
                }
            }

            inplace_trigonometric_sort(poly, poly[leftmost_i]);
            leftmost_i = 0;
        }

        std::stack<int> st;
        st.push(leftmost_i);

        for (int i = (leftmost_i + 1) % n; i != leftmost_i; i = (i + 1) % n) {
            st.push(i);
            if (st.size() < 3) continue;

            while (!st.empty() && st.size() >= 3) { 
                int p_i, q_i, r_i;
                r_i = st.top(); st.pop();
                q_i = st.top(); st.pop();
                p_i = st.top(); st.pop();

                const auto& p = poly[p_i], q = poly[q_i], r = poly[r_i];
                auto orientation = compute_orientation(p, q, r);
            
                st.push(p_i);
                if (orientation == COUNTERCLOCKWISE) st.push(q_i);
                st.push(r_i);
                if (orientation == COUNTERCLOCKWISE) break;
            }
        }

        std::list<int> convex_hull;
        while (!st.empty()) {
            convex_hull.push_front(st.top());
            st.pop();
        }

        return convex_hull;
    }
};