import os
from Crypto.Util.number import bytes_to_long
from aes import *

try:
    flag = open('/flag').read().strip()
except:  # noqa
    flag = 'A' * 15


def menu():
    print("==== MENU ====")
    print("1. Encrypt your plaintext")
    print("2. Get encrypted flag")
    print("3. Quit")
    return input("> ")


def main():
    print("Welcome to my new AES!!")
    key = os.urandom(16)
    cipher = AES(bytes_to_long(key))

    for _ in range(10):
        choice = menu()
        if choice == '1':
            try:
                plaintext = bytes.fromhex(input("Enter your plaintext(hex): "))
                matrix = bytes.fromhex(input("Enter your matrix(hex): "))
                assert len(matrix) == 16
                print(cipher.encrypt_message(plaintext, matrix))
            except Exception as e:
                print("Ooops!", e)
        elif choice == '2':
            try:
                matrix = bytes.fromhex(input("Enter your matrix(hex): "))
                assert len(matrix) == 16
                print(cipher.encrypt_message(flag.encode(), matrix))
            except Exception as e:
                print("Ooops!", e)
        elif choice == '3':
            break
        else:
            print("Bruh!")
    print("Goodbye!")


if __name__ == '__main__':
    main()
