## Steps:

# Goal
Cr3Token.balanceOf(owner) == 0 && Cr3Token.balanceOf(Cr3Dao) == 0

# Player
## Call SetBalanceoF() function to reallocate the index and values that were assigned and deleted during deployment

https://docs.soliditylang.org/en/latest/security-considerations.html#clearing-mappings

This is important because during deployment we allocate a mapping, and delete the mapping. 

But deleting it does not actually delete the mapping.
Since the mapping is still there, we can access it again. 

If we look closely, the function below allocates the mapping again.

`function setBalanceoF(uint256 i, address user, uint256 amount)` 

We can call the function below with following arguments.
We are using index upto 4 because it will be checked later on this function `function getBalanceoF(uint id, address add) public view returns (uint)`

Also the balance of the admin needs to be `1337 * 10 ** 18`



```
setBalanceoF(0, addressOfAdmin, 1337000000000000000000) 
setBalanceoF(1, addressOfAdmin, 1337000000000000000000)
setBalanceoF(2, addressOfAdmin, 1337000000000000000000) 
setBalanceoF(3, addressOfAdmin, 1337000000000000000000) 
setBalanceoF(4, addressOfAdmin, 1337000000000000000000)
```

## Create a  proposal in `Cr3Dao` contract to vote on later

One can create any proposal in here

```
createProposal("Cr3")
createProposal("Cr3")
createProposal("Cr3")
createProposal("Cr3")
```

## Vote on the created proposal
This function allow us to vote on our created proposal and calls `Cr3Token` contract to get highest number of votes.

`function vote(uint256 _proposalId, bytes calldata fs)` 

When calling this function, it expects `proposalId` which was just created. Let's use `4` and 
 also expects `bytes calldata fs` which will include the full calldata to call the expected function from `Cr3Token` contract.

 Since we want to have highest number of votes, we can call `getBalanceoF()` function of `Cr3Token` contract with the address of admin.

 It will return us `1337 * 10 ** 18` value because we did set that up earlier in  the first step.

 We can construct calldata using foundry. For example:

 ```
 cast calldata "getBalanceoF(uint, address)" 4 0x9Ab04Deaf4ff03BecBDE707cd026147a9497C103
 0x3187ac2300000000000000000000000000000000000000000000000000000000000000040000000000000000000000009ab04deaf4ff03becbde707cd026147a9497c103
 ```
This will make the player the highestVoter with highest number of votes and can execute the proposal by themselves.


## Execute The Proposal

Executing the vote transfers `1337*10**18 Cr3` tokens from admin to Cr3Dao contract since previously during deployment admin allowed Cr3Dao contract to spend `1337*10**18` tokens on their behalf.

Since this function can only be called by the player with highest number of votes, it should be no problem to call this function as we made sure we have highest number of votes in the previous step

```
 function executeProposal(
        uint256 _proposalId,
        address target,
        uint256 amount
    )
```

Successful execution of this function will transfer all the `Cr3Tokens` from admin to this `Cr3Dao` contract.

But the final goal is to steal all the tokens from `Cr3Dao` contract to the players account.


## Dirty Byte Vulnerability
https://soliditylang.org/blog/2022/06/15/dirty-bytes-array-to-storage-bug/
https://blog.oxor.io/exploring-the-bugs-and-features-of-solidity-compiler-versions-a-guide-for-smart-contract-fe04e852ea64

Since the compiler version is `0.8.10`, the dirty byte vulnerability can be used to pass the check in the function which will transfer all the tokens from `Cr3Dao` contract to `player` 

Calling the function `coolCheck(uint8 _oneFa, uint _twoFa, address player)` has dirty byte vulnerability which copies one extra byte. Since the data is copied in chunks of 32 byte blocks even if the length is not multiple of 32, that one extra byte is visible after `push()` operation within the function.
So valid argument values to bypass the check will be `coolCheck(2, 63, attackersAddress)` or `(3, 63, attackerAddress)` or `(4, 63, attackersAddress)` and so on ......

## Doing all of the given steps should satisfy the check condition below and give us the flag

`Cr3Token.balanceOf(owner) == 0 && Cr3Token.balanceOf(Cr3Dao) == 0`

### Solver

see `solve.py` and `solver.sh`

### Unintended solution

you could've burned all the tokens instead of solving it in the intended way
