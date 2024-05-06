#include <bits/stdc++.h>

using namespace std;

using i64 = int64_t;
using pi64 = pair<i64, i64>;

int main() {
    i64 n, k;
    cin >> n >> k;

    vector<i64> a(n);
    vector<pi64> b(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i];

        b[i] = {a[i], i};
    }

    b.push_back({0, n});

    sort(b.begin(), b.end(), [](const pi64& x, const pi64& y){
        return forward_as_tuple(-x.first, x.second) < forward_as_tuple(-y.first, y.second);
    });

    i64 bp = 0;
    i64 bp_sum = 0;
    while (bp < n) {
        bp_sum += b[bp].first;

        bp++;

        //cerr << "bp=" << bp << " bp_sum=" << bp_sum << " bp*b[bp].first=" << bp*b[bp].first << endl;

        if (bp_sum - bp*b[bp].first >= k) break;
    }

    const i64& target = b[bp].first;
    const i64 v = (bp_sum - k + bp - 1) / bp;
    i64 ones_to_cut = bp * v - (bp_sum - k);

    for (int i = 0; i < bp; i++) {
        a[ b[i].second ] = b[i].first;
    }

    for (int i = 0; i < n; i++) {
        if (a[i] < v) continue;

        a[i] = v;

        if (ones_to_cut > 0) {
            a[i]--;
            ones_to_cut--;
        }
    }

    for (auto x : a) cout << x << " ";
}
