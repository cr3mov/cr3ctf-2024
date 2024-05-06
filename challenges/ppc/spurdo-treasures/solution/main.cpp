#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ll L,R;
    cin >> L >> R;
    ll y = sqrt(2 * L + 1);
    while(y * y < 2 * L  + 1) {
        y += 1;
    }
    if(y * y > 2 * R - 1) {
        cout << -1 << ' ' << -1 << endl;
        return 0;
    }
    ll a = y * y / 2;
    ll b = y * y - a;
    if(a == b) {
        a -= 1;
        b += 1;
    }
    cout << a << ' ' << b << endl;   
}