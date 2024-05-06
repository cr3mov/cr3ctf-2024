// SPDX-License-Identifier: UNLICENSED
pragma solidity ^0.8.20;

import "forge-ctf/CTFDeployer.sol";
import "forge-ctf/CTFChallenge.sol";

import "src/Cr3Proxy.sol";
import "src/Logic.sol";

contract Deploy is CTFDeployer {
    function deploy(address system, address player) internal override returns (CTFChallenge[] memory challenges) {
        vm.startBroadcast(system);

        Logic logic = new Logic();
        Cr3Proxy cr3proxy = new Cr3Proxy(address(logic));

        challenges = new CTFChallenge[](2);
        challenges[0] = CTFChallenge("Logic", address(logic));
        challenges[1] = CTFChallenge("Cr3Proxy", address(cr3proxy));

        vm.stopBroadcast();
    }
}
