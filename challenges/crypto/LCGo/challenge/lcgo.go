package main

import (
    "crypto/rand"
    "math/big"
    "os"
)

var flag = "cr3{test_flag}"

type LCGO struct {
    n    *big.Int
    a    *big.Int
    seed *big.Int
}

func check(err error) {
    if err != nil {
        panic(err)
    }
}

func Init() *LCGO {
    n := nextPrime(new(big.Int).Exp(big.NewInt(2), big.NewInt(1024), nil))
    a := new(big.Int).SetBytes([]byte(flag))
    seed, err := rand.Int(rand.Reader, n)
    check(err)

    return &LCGO{n, a, seed}
}

func (lcg *LCGO) next() *big.Int {
    max := new(big.Int).Exp(big.NewInt(2), big.NewInt(768), nil)
    rd, err := rand.Int(rand.Reader, new(big.Int).Mul(big.NewInt(2), max))
    rd.Sub(rd, max)
    check(err)

    lcg.seed.Mul(lcg.seed, lcg.a)
    lcg.seed.Add(lcg.seed, rd)
    lcg.seed.Mod(lcg.seed, lcg.n)

    return new(big.Int).Set(lcg.seed)
}

func nextPrime(n *big.Int) *big.Int {
    for {
        if n.ProbablyPrime(20) {
            return n
        }
        n.Add(n, big.NewInt(1))
    }
}

func main() {
    lcg := Init()

    res := make([]*big.Int, 16)
    for i := 0; i < 16; i++ {
        res[i] = lcg.next()
    }

    file, err := os.Create("out.txt")
    check(err)
    defer file.Close()

    for _, num := range res {
        _, err := file.WriteString(num.String() + "\n")
        check(err)
    }
}