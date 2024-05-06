// SPDX-License-Identifier: MIT
pragma solidity 0.8.10;

import "./IERC20.sol";

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

    bytes32 private crc;

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


        crc = keccak256(abi.encodePacked(address(this)));

        bytes memory extracted = fs[:4];

        bytes memory toComp = "0x8a8e21bf";
        bytes memory toCompII = "0x23b872dd";


        for (uint i = 0; i < extracted.length; i++) {
            if (extracted[i] == toComp[i]) {
                revert("No rush my friend, No rush : |");
            }
        }
        for (uint i = 0; i < extracted.length; i++) {
            if (extracted[i] == toCompII[i]) {
                revert("No rush my friend, No rush : |");
            }
        }

        (bool success, bytes memory returnData) = address(Cr3Token).call(fs);

        require(success, "Sed :| ");

        uint256 returnedBal;
        bytes32 returnedCrc;

        assembly {
            returnedBal := mload(add(returnData, 0x20))
            returnedCrc := mload(add(returnData, 0x40))
        }

        require(returnedCrc == crc, "Try Harder");

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
