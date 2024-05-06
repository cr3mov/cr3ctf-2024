package encryption

import (
	"crypto/rand"
	"math/big"
)

type PrimeKey int
type PrimeKeyUnit big.Int
const PrimeKeySamples = 8

func (k PrimeKey) SampleCount() int {
	return PrimeKeySamples
}

func (k PrimeKey) Generate(enc *Encryption, keyLength int) []byte {

	res := make([]*PrimeKeyUnit, keyLength)
	i := 0
	for bi := range enc.RandomBigInts {

		// Avoid copying to speed this up, cuz algorithm takes a lot of time as security pay off 
		res[i] = &bi
		i++

		if i == keyLength {
			break
		}
	}

	chosenInt, _ := rand.Int(rand.Reader, big.NewInt(int64(keyLength)))
	ret := big.Int(*res[chosenInt.Int64()])
	return ret.Bytes()
}