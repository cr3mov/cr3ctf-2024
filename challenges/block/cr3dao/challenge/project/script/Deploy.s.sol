// SPDX-License-Identifier: UNLICENSED
pragma solidity ^0.8.0;

import "forge-ctf/CTFDeployer.sol";
import "forge-ctf/CTFChallenge.sol";

import "src/Cr3DAO.sol";
import "src/Cr3Token.sol";

contract Deploy is CTFDeployer {
    function deploy(address system, address player) internal override returns (CTFChallenge[] memory challenges) {
        vm.startBroadcast(system);

        Cr3Token token = new Cr3Token("Cr3", "Cr3", 18);
        Cr3DAO dao = new Cr3DAO(address(token));

        token.approve(address(dao), 1337);
        token.setDaoAddress(address(dao));

        challenges = new CTFChallenge[](2);
        challenges[0] = CTFChallenge("Cr3Token", address(token));
        challenges[1] = CTFChallenge("Cr3DAO", address(dao));

        vm.stopBroadcast();
    }
}