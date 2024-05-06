// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

contract Cr3Proxy {
  struct Structure {
    uint8 x;
    uint16 y;
    bool z;
    uint24 a;
    bytes8 bt;
    uint32 b;
  }

  address public owner;
  Structure private str = Structure(1, 13, false, 37, "cafe", 10);
  address public logic;
  bool public cr3 = false;

  constructor(address _log) {
    owner = msg.sender;
    logic = _log;
  }

  modifier onlyOwner() {
    require(msg.sender == owner, "Not owner");
    _;
  }

  modifier onlyReal() {
    require(str.z == true, "Na, Try Harder");
    _;
  }

  function upgrade(address _newLogic) onlyReal public {
    require(_newLogic != address(this), "Nope");
    require(_newLogic != address(0), "Nope");
    require(str.bt == "ffff", "Final Check");
    logic = _newLogic;
  }

  function update(bytes calldata fs, address add) external {
    address mainOwner = owner;
    address mainLogic = logic;
    bool oldCr3 = cr3;

    if(str.z) {
      (bool suc, ) = address(logic).delegatecall(fs);
      require(suc, "Failed");
      require(cr3 == oldCr3, "No NOOOOOOOO");
    } else {
      (bool success, bytes memory returnData) = address(add).delegatecall(fs);
      require(success, "ERROR, Delegatecall Failed");
      require(mainOwner == owner, "Soon");
      require(mainLogic == logic, "Later");
      require(cr3 == oldCr3, "Nott Yettt");

      (, , bytes8 sig) = abi.decode(returnData, (bool, address, bytes8));
      str.bt = sig;
    }
  }

  function checkValid(bytes calldata flFunc) onlyOwner external {
    address currentOwner = owner;

    (bool success, bytes memory ret) = address(logic).call(flFunc);
    require(success, "Call To The Logic Contract Failed");

    (bytes6 name, bytes32 pass) = abi.decode(ret, (bytes6, bytes32));
    require(address(uint160(uint256(pass))) == currentOwner, "Read More");
    require(name == "cr3", "It's Cr3 CTF");

    cr3 = true;
  }

  function isSolved() public view returns (bool) {
    if(cr3 == true) {
      return true;
    } else {
      return false;
    }
  }
}
