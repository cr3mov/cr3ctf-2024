## Steps:

# Goal
Leverage Unsafe DelegateCall to Modify the storage of the proxy contract to become owner of the Proxy.

## StepOne
Change `Str.z` boolean value to true using Attacker's Logic contract

## StepTwo
Update Logic Address Value of the Proxy Contract

## StepThree
Update Owner of the proxy contract

## StepFour 
Modify address from address to bytes32 and send it back to have it truncated

### Solver

See `solver.sh`
