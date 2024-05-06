#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from typing import List

from web3 import Web3

from ctf_launchers.pwn_launcher import PwnChallengeLauncher
from ctf_launchers.types import ChallengeContract


class Launcher(PwnChallengeLauncher):
    def is_solved(self, web3: Web3, contracts: List[ChallengeContract]) -> bool:
        contract = next(x for x in contracts if x['name'] == 'Cr3Token')
        return self.is_contract_solved(web3, contract)


Launcher(project_location='/challenge/project').run()
