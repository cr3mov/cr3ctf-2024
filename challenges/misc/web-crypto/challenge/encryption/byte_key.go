package encryption

import (
	"crypto/rand"
	"math/big"
)

type ByteKey int
type ByteKeyUnit byte
// It works faster than prime key, so I can use more samples
const ByteKeySamples = 256

func (k ByteKey) SampleCount() int {
	return ByteKeySamples
}

func (k ByteKey) Generate(enc *Encryption, keyLength int) []byte {

	res := make([]*byte, keyLength*256)
	ba := make([]byte, keyLength*256)
	rand.Read(ba)
	i := 0
	for _, bi := range ba {

		// Avoid copying to speed this up, cuz algorithm takes a lot of time as security pay off 
		res[i] = &bi
		i++

		if i == keyLength*256{
			break
		}
	}

	ret := make([]byte, 32)
	for i := range ret {
		chosenInt, _ := rand.Int(rand.Reader, big.NewInt(int64(keyLength*256)))
		ret[i] = byte(*res[chosenInt.Int64()])
	}

	return ret
}