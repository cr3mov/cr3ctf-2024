#include <bits/stdc++.h>

using namespace std;

const int S = 1024;
const int B = 4;
const int E = S-4;

int a[S][S];

int qim[S][S];
int qip[S][S];
int qjm[S][S];
int qjp[S][S];

int main(int argc, char *argv[]) {
    int n, m;
    cin >> n >> m;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++) {
            cin >> a[i+9][j+9];
        }
    }

    for (int i = B; i <= E; i++)
    for (int j = B; j <= E; j++) {
        qim[i][j] = max(a[i][j], a[i][j-1] + a[i][j+1] + qim[i-1][j]);
        qjm[i][j] = max(a[i][j], a[i-1][j] + a[i+1][j] + qjm[i][j-1]);
    }

    for (int i = E; i >= B; i--)
    for (int j = E; j >= B; j--) {
        qip[i][j] = max(a[i][j], a[i][j-1] + a[i][j+1] + qip[i+1][j]);
        qjp[i][j] = max(a[i][j], a[i-1][j] + a[i+1][j] + qjp[i][j+1]);
    }

    int answer = 0;

    for (int i = B; i <= E; i++)
    for (int j = B; j <= E; j++)
    for (int use_ip : {true, false})
    for (int use_im : {true, false})
    for (int use_jp : {true, false})
    for (int use_jm : {true, false})
    {
        int cur_ans =
            + a[i-1][j-1] * int(use_im || use_jm)
            + a[i+1][j-1] * int(use_ip || use_jm)
            + a[i-1][j+1] * int(use_im || use_jp)
            + a[i+1][j+1] * int(use_ip || use_jp)
            + (use_im ? qim[i-2][j] : a[i-1][j])
            + (use_ip ? qip[i+2][j] : a[i+1][j])
            + (use_jm ? qjm[i][j-2] : a[i][j-1])
            + (use_jp ? qjp[i][j+2] : a[i][j+1]);

        answer = max(cur_ans, answer);
    }

    cout << answer << endl;
}
