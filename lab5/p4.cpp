#include <bits/stdc++.h>
#include "geometry.hpp"

using namespace std;

typedef list<int> points_list;
typedef points_list::iterator points_iterator;
typedef tuple<points_iterator, points_iterator, points_iterator, double> rij_pair;

struct rij_comp {
    bool operator()(const rij_pair& lhs, const rij_pair& rhs) const {
        return get<3>(lhs) > get<3>(rhs);
    }
};

int main() {
    int n; cin >> n;
    geometry::Polygon2D poly(n);

    for (int i = 0; i < n; i++) {
        cin >> poly[i];
    }

    auto tour = geometry::graham_scan(poly);

    bool used_in_hull[n];
    std::memset(used_in_hull, false, n * sizeof(bool));
    for (auto i: tour) {
        used_in_hull[i] = true;
    }

    // create a list of unused points
    list<int> points_left;
    for (int i = 0; i < n; i++) {
        if (not used_in_hull[i]) points_left.push_back(i);
    }

    while (!points_left.empty()) {
        priority_queue<rij_pair, vector<rij_pair>, rij_comp> pq;

        // two nested fors: N^2
        for (auto r_it = points_left.begin(); r_it != points_left.end(); ++r_it) {
            double min_cost = numeric_limits<double>::max();
            rij_pair best;

            // find two neighbours (i, j) to minimize the cost of (i, r, j)
            for (auto i_it = tour.begin(); i_it != tour.end(); ++i_it) {
                auto j_it = i_it;
                if (++j_it == tour.end()) continue;

                auto c_ir = geometry::euclidean_distance(poly[*i_it], poly[*r_it]);
                auto c_rj = geometry::euclidean_distance(poly[*r_it], poly[*j_it]);
                auto c_ij = geometry::euclidean_distance(poly[*i_it], poly[*j_it]);
                auto cost = c_ir + c_rj - c_ij;

                if (cost < min_cost) {
                    min_cost = cost;
                    best = make_tuple(r_it, i_it, j_it, 0);
                }
            }

            points_iterator i_it = get<1>(best);
            points_iterator j_it = get<2>(best);

            auto c_ir = geometry::euclidean_distance(poly[*i_it], poly[*r_it]);
            auto c_rj = geometry::euclidean_distance(poly[*r_it], poly[*j_it]);
            auto c_ij = geometry::euclidean_distance(poly[*i_it], poly[*j_it]);
            auto final_cost = (c_ir + c_rj) / c_ij;

            rij_pair final = make_tuple(r_it, i_it, j_it, final_cost);
            // minumum in log N
            pq.push(final);
        }

        // choose the best (r, i, j) pair from a PQ
        auto best = pq.top();
        auto r_it = get<0>(best);
        auto j_it = get<2>(best);
        // i (insert r here) j
        tour.insert(j_it, *r_it);
        points_left.erase(r_it);
    }

    // display the last element twice to complete the tour
    cout << poly[tour.back()] << endl;
    for (auto i: tour) {
        cout << poly[i] << endl;
    }

    return 0;
}