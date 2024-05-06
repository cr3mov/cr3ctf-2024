#include <bits/stdc++.h>

using namespace std;

using i64 = int64_t;

i64 x, y, w, p, b, q;

struct sinfo {
    int wr;
    int br;
    i64 v;
    i64 a;
    i64 a_w;
    i64 a_b;

    void inspect() {
        cerr << "STATE "
            << " wr=" << wr << " br=" << br
            << " v=" << v //<< " x+v=" << x+v
            << " a=" << a << " a_w=" << a_w << " a_b=" << a_b << endl;
    }
};

const i64 ZB = (1 << 20);
const i64 ZW = 0;
const i64 Z = 0;
const i64 S = (1 << 22);

sinfo s[S];
bool used[S];

inline void split_s(int to_s, int& wr, int& br) {
    if (to_s == Z) {
        wr = 0;
        br = 0;
        return;
    }

    if (ZW <= to_s && to_s < ZW + w) {
        wr = to_s - ZW;
        br = 0;
        return;
    }

    if (ZB <= to_s && to_s < ZB + b) {
        wr = 0;
        br = to_s - ZB;
        return;
    }

    assert(false);
}

inline int pack_s(int wr, int br) {
    wr %= w;
    br %= b;

    //cerr << " wr=" << wr << " br=" << br << endl;

    if (wr == 0 && br == 0) return Z;
    if (br == 0) return ZW + wr;
    if (wr == 0) return ZB + br;

    assert(false);
    return 0;
}

inline int pack_s(const sinfo& si) {
    return pack_s(si.wr, si.br);
}

sinfo calc_transition(int cur_s) {
    sinfo si;

    split_s(cur_s, si.wr, si.br);

    int& wr = si.wr;
    int& br = si.br;

    // answers
    si.a = 1;
    si.v = 1 + (wr == 0 ? p : 0) + (br == 0 ? q : 0);
    si.a_w = (wr == 0 ? 1 : 0);
    si.a_b = (br == 0 ? 1 : 0);

    // next state
    wr = (wr + si.v) % w;
    br = (br + si.v) % b;

    if (wr != 0 && br != 0) {
        int d = min(w-wr, b-br);

        si.a += d;
        si.v += d;
        wr = (wr + d) % w;
        br = (br + d) % b;

        assert(wr == 0 || br == 0);
    }

    return si;
}

sinfo starting_state(i64 x) {
    sinfo si;

    si.wr = x % w;
    si.br = x % b;

    si.v = x;
    si.a = 0;
    si.a_w = 0;
    si.a_b = 0;

    return si;
}

void push(sinfo& si, const sinfo& sio, i64 coef=1) {
    si.v += sio.v * coef;
    si.a += sio.a * coef;
    si.a_w += sio.a_w * coef;
    si.a_b += sio.a_b * coef;

    si.wr = sio.wr;
    si.br = sio.br;
}

void answer_naive_from(sinfo si) {
    int& wr = si.wr;
    int& br = si.br;

    while (si.v < y) {
        //si.inspect();

        i64 vd = 1 + (wr == 0 ? p : 0) + (br == 0 ? q : 0);

        si.v += vd;
        si.a += 1;
        si.a_w += (wr == 0 ? 1 : 0);
        si.a_b += (br == 0 ? 1 : 0);

        si.wr = (si.wr + vd) % w;
        si.br = (si.br + vd) % b;
    }

    cout << si.a << " " << si.a_w << " " << si.a_b << endl;
}

int main() {
    cin >> x >> y >> w >> p >> b >> q;


    s[Z] = calc_transition(Z);
    for (int wr = 0; wr < w; wr++) s[ZW + wr] = calc_transition(ZW + wr);
    for (int br = 0; br < b; br++) s[ZB + br] = calc_transition(ZB + br);

    sinfo sx = starting_state(x);

    //cout << "NAIVE" << endl; answer_naive_from(sx);

    /// push to valid state
    while (sx.br != 0 && sx.wr != 0) {
        sx.a += 1;
        sx.v += 1;

        sx.wr = (sx.wr + 1) % w;
        sx.br = (sx.br + 1) % b;

        if (sx.v >= y) {
            answer_naive_from(sx);
            return 0;
        }
    }

    /// until loop
    sinfo snext;

    while (!used[pack_s(sx)]) {
        used[pack_s(sx)] = true;

        snext = sx;

        push(snext, s[pack_s(sx)]);

        if (snext.v >= y) {
            answer_naive_from(sx);
            return 0;
        }

        sx = snext;
    }

    /// loop
    const int loop_wr = sx.wr;
    const int loop_br = sx.br;

    sinfo lps = starting_state(0);
    lps.br = loop_br;
    lps.wr = loop_wr;

    for (int i = 0; (loop_br != lps.br || loop_wr != lps.wr || i == 0) && i < S; i++) {
        push(lps, s[pack_s(lps)]);
    }

    assert(loop_br == lps.br && loop_wr == lps.wr);

    i64 loop_coef = (y-sx.v) / lps.v;

    push(sx, lps, loop_coef);

    assert(sx.v <= y);

    /// loop part
    while (true) {
        snext = sx;

        push(snext, s[pack_s(snext)]);

        if (snext.v >= y) {
            answer_naive_from(sx);
            return 0;
        }

        sx = snext;
    }

    assert(false);
}
