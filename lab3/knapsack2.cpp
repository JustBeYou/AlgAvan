#include <bits/stdc++.h>

using namespace std;

int main() {
    freopen("knapsack2.in", "r", stdin);

    int k, x, sum = 0;
    cin >> k;

    while (cin >> x) {
        if (x <= k) sum += x, k -= x;
        else if (sum < x) {
            sum = x;
            break;
        }
    }
    cout << sum << endl;

    return 0;
}
