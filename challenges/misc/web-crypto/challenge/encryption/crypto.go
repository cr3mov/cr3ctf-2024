package encryption

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"math/big"

	"github.com/google/uuid"
	"github.com/zenazn/pkcs7pad"
)

type Encryption struct {
	generatorRoutine bool
	RandomBigInts chan PrimeKeyUnit
	id uuid.UUID
}

type KeyGenerator interface {
	PrimeKey | ByteKey

	Generate(enc *Encryption, keyLength int) []byte
	SampleCount() int
}


func NewEncryption() *Encryption {
	enc := &Encryption{
		generatorRoutine: true,
		id: uuid.New(),
	}
	enc.init()	

	return enc
}

func (e *Encryption) Stop() {
	e.generatorRoutine = false
	<- e.RandomBigInts
	close(e.RandomBigInts)
}

func (e *Encryption) init() {
	e.RandomBigInts = make(chan PrimeKeyUnit)

	go func() {
		for e.generatorRoutine {
			bi, _ := rand.Prime(rand.Reader, 256)
			e.RandomBigInts <- PrimeKeyUnit(*bi)
		}
	}()
}

func generateKeys[V KeyGenerator](e *Encryption, keyLength int, gen V) [][]byte {
	res := make([][]byte, keyLength)
	for i := range res {
		res[i] = gen.Generate(e, keyLength)
	}
	
	return res
}

func encrypt[V KeyGenerator](e *Encryption, gen V, data []byte) []byte {
	keyLength := gen.SampleCount()

	chosenKey, _ := rand.Int(rand.Reader, big.NewInt(int64(keyLength)))
	chosenIv, _ := rand.Int(rand.Reader, big.NewInt(int64(keyLength)))

	possibleKeys := generateKeys(e, keyLength, gen)

	aes, _ := aes.NewCipher(possibleKeys[chosenKey.Int64()])
	cbc := cipher.NewCBCEncrypter(aes, possibleKeys[chosenIv.Int64()][:16])
	
	enc := pkcs7pad.Pad(data, aes.BlockSize())
	cbc.CryptBlocks(enc, enc)
	return enc
}

func (e *Encryption) EncryptByteKey(data []byte) []byte {

	var key ByteKey
	return encrypt(e, key, data)
}

func (e *Encryption) EncryptPrimeKey(data []byte) []byte {

	var key PrimeKey
	return encrypt(e, key, data)
}