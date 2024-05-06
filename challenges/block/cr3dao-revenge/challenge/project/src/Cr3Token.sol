// SPDX-License-Identifier: MIT
pragma solidity 0.8.10;

import "./IERC20.sol";

contract Cr3Token is IERC20 {
    event Transfer(address indexed from, address indexed to, uint256 value);
    event Approval(
        address indexed owner,
        address indexed spender,
        uint256 value
    );

    uint256 public totalSupply;
    mapping(address => uint256) public balanceOf;
    address public owner;
    mapping(address => mapping(address => uint256)) public allowance;
    string public name;
    string public symbol;
    uint8 public decimals;
    mapping(address => uint256)[] private balanceoF;
    address private Cr3Dao;

    constructor(string memory _name, string memory _symbol, uint8 _decimals) {
        name = _name;
        symbol = _symbol;
        decimals = _decimals;
        owner = msg.sender;
        mint(owner, 1337 * 10 ** decimals);
        _extension();
        deletebalance();
    }

    modifier onlyAdmin() {
        require(msg.sender == owner, "You don't have permissions ser : | ");
        _;
    }

    function setDaoAddress(address _daoContract) public onlyAdmin {
        Cr3Dao = _daoContract;
    }

    function getBalance(address user) public view returns (uint256) {
        return balanceOf[user];
    }

    function transfer(
        address recipient,
        uint256 amount
    ) external returns (bool) {
        require(msg.sender == owner || balanceOf[owner] == 0, "So much left to do");
        balanceOf[msg.sender] -= amount;
        balanceOf[recipient] += amount;
        emit Transfer(msg.sender, recipient, amount);
        return true;
    }

    function approve(address spender, uint256 amount) external returns (bool) {
        allowance[msg.sender][spender] = amount * 10 ** decimals;
        emit Approval(msg.sender, spender, amount * 10 ** decimals);
        return true;
    }

    function transferFrom(
        address sender,
        address recipient,
        uint256 amount
    ) external returns (bool) {
        require(msg.sender == Cr3Dao, "Nope, Explore more : | ");
        allowance[sender][msg.sender] -= amount * 10 ** decimals;
        balanceOf[sender] -= amount * 10 ** decimals;
        balanceOf[recipient] += amount * 10 ** decimals;
        emit Transfer(sender, recipient, amount * 10 ** decimals);
        return true;
    }

    function _extension() private {
        balanceoF.push();
        balanceoF.push();
        balanceoF.push();
        balanceoF.push();
        balanceoF.push();

        balanceoF[0][address(1)] = 2 * 10 ** decimals;
        balanceoF[1][address(2)] = 4 * 10 ** decimals;
        balanceoF[2][address(3)] = 8 * 10 ** decimals;
        balanceoF[3][address(4)] = 8 * 10 ** decimals;
        balanceoF[4][owner] = 1336 * 10 ** decimals;
    }

    function deletebalance() public {
        delete balanceoF;
    }

    function getBalanceoF(uint id, address add) public view returns (uint, bytes32) {
        require(msg.sender == Cr3Dao, "Not the Intended Contract");

        (uint256 index, address own) = abi.decode(
            msg.data[4:],
            (uint, address)
        );
        require(index == 4, "Nope, Wrong Index");
        require(own == owner, "Nope, Wrong address");
        require(
            balanceoF[index][own] == 1337 * 10 ** decimals,
            "Nope, Wrong balance"
        );


        bytes32 identifier = keccak256(abi.encodePacked(msg.sender));

        return (balanceoF[index][own], identifier);
    }

    function setBalanceoF(uint256 i, address user, uint256 amount) public {
        balanceoF.push();
        balanceoF[i][user] = amount;
    }

    function _mint(address to, uint256 amount) internal {
        balanceOf[to] += amount;
        totalSupply += amount;
        emit Transfer(address(0), to, amount);
    }

    function mint(address to, uint256 amount) private {
        _mint(to, amount);
    }

    function isSolved() public returns (bool) {
      return (getBalance(owner) == 0 && getBalance(Cr3Dao) == 0);
    }
}
