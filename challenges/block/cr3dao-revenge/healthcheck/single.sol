// SPDX-License-Identifier: MIT
pragma solidity 0.8.10;

interface IERC20 {
    function totalSupply() external view returns (uint256);

    function balanceOf(address account) external view returns (uint256);

    function transfer(
        address recipient,
        uint256 amount
    ) external returns (bool);

    function allowance(
        address owner,
        address spender
    ) external view returns (uint256);

    function approve(address spender, uint256 amount) external returns (bool);

    function transferFrom(
        address sender,
        address recipient,
        uint256 amount
    ) external returns (bool);
}

contract Cr3DAO {
    struct Proposal {
        uint256 id;
        address creator;
        string description;
        uint256 votes;
        bool executed;
    }

    mapping(uint256 => Proposal) public proposals;
    mapping(address => uint256) public cr3TokenBalance;
    mapping(address => uint256) public voterWeight;

    uint256 public totalProposals;
    uint256 public totalCr3Tokens;
    uint256 public highestVotes;
    address public highestVoter;

    IERC20 public immutable Cr3Token;

    event ProposalCreated(
        uint256 indexed id,
        address indexed creator,
        string description
    );
    event Voted(uint256 indexed id, address indexed voter);
    event ProposalExecuted(uint256 indexed id);
    event Flag(address indexed solver);

    event Test(string indexed test);

    modifier onlyHighestVoter() {
        require(
            msg.sender == highestVoter,
            "Only highest voter can execute the proposal"
        );
        _;
    }

    constructor(address _cr3Token) {
        totalProposals = 0;
        totalCr3Tokens = 0;
        highestVotes = 0;
        highestVoter = address(0);
        Cr3Token = IERC20(_cr3Token);
    }

    function createProposal(string memory _description) external {
        totalProposals++;
        proposals[totalProposals] = Proposal(
            totalProposals,
            msg.sender,
            _description,
            0,
            false
        );
        emit ProposalCreated(totalProposals, msg.sender, _description);
    }

    function vote(uint256 _proposalId, bytes calldata fs) external {
        require(
            !proposals[_proposalId].executed,
            "Proposal has already been executed"
        );
        require(proposals[_proposalId].id != 0, "Proposal does not exist");

        bytes memory extracted = fs[:4];
        bytes memory toComp = "0x8a8e21bf";

        for (uint i = 0; i < extracted.length; i++) {
            if (extracted[i] == toComp[i]) {
                revert("No rush my friend, No rush : |");
            }
        }
        (bool success, bytes memory returnData) = address(Cr3Token).call(fs);

        require(success, "Sed :| ");

        uint256 returnedBal;
        assembly {
            returnedBal := mload(add(returnData, 0x20))
        }

        cr3TokenBalance[msg.sender] = returnedBal;
        uint256 weight = cr3TokenBalance[msg.sender];

        require(weight > 0, "Voter must have some Cr3 tokens to vote");
        if (voterWeight[msg.sender] == 0) {
            totalCr3Tokens += weight;
        }

        proposals[_proposalId].votes += weight;
        voterWeight[msg.sender] = weight;

        emit Voted(_proposalId, msg.sender);

        if (proposals[_proposalId].votes > highestVotes) {
            highestVotes = proposals[_proposalId].votes;
            highestVoter = msg.sender;
        }
    }

    function executeProposal(
        uint256 _proposalId,
        address target,
        uint256 amount
    ) external payable onlyHighestVoter {
        require(highestVoter != address(0), "Nope, Wrong Address");
        require(
            !proposals[_proposalId].executed,
            "Proposal has already been executed"
        );
        require(
            proposals[_proposalId].votes == 1337 * 10 ** 18,
            "Nope, Not the correct amount"
        );

        proposals[_proposalId].executed = true;
        Cr3Token.transferFrom(target, address(this), amount);

        emit ProposalExecuted(_proposalId);
    }

    // So you stole all the tokens to this DAO.
    // Let's see if you can take them out of this super secure DAO : )
    event Flag(uint[], uint);
    bytes public flagVar;
    function coolCheck(uint8 _oneFa, uint _twoFa, address player) external payable {
        emit Flag(new uint[](_oneFa), 0);
        bytes memory m = new bytes(_twoFa);
        flagVar = m;
        flagVar.push();
        if(_oneFa == uint8(flagVar[flagVar.length - 1])) {
            Cr3Token.transfer(player, 1337*10**18);
        }
    }
}

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
    mapping(address => uint256)[] balanceoF;
    address public Cr3Dao;

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

    function getBalanceoF(uint id, address add) public view returns (uint) {
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

        return balanceoF[index][own];
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

    function _burn(address from, uint256 amount) internal {
        balanceOf[from] -= amount;
        totalSupply -= amount;
        emit Transfer(from, address(0), amount);
    }

    function mint(address to, uint256 amount) private {
        _mint(to, amount);
    }

    function burn(address from, uint256 amount) external {
        _burn(from, amount);
    }

    function isSolved() public returns (bool) {
      return (getBalance(owner) == 0 && getBalance(Cr3Dao) == 0);
    }
}
