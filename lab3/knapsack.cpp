#include <bits/stdc++.h>

using namespace std;

int main() {
    freopen("knapsack.in", "r", stdin);

    int n, K;
    cin >> n >> K;

    vector<int> S(n), W(n);
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        W[i] = S[i] = x;
    }

    vector<int> dp(K+1, 0);
    vector<int> prev(K+1, -1);
    for (int i = 0; i < n; i++) {
        for (int w = K; w >= 0 and w - W[i] >= 0; w--) {
            if (dp[w - W[i]] + S[i] > dp[w]) {
                prev[w] = i;
            }
            dp[w] = max(dp[w], dp[w - W[i]] + S[i]);
        }
    }

    cout << dp[K] << "\n";

    while (prev[K] != -1) {
        cout << prev[K] << " ";
        K -= W[prev[K]];
    }
    cout << "\n";

    return 0;
}
