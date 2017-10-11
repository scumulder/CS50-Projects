#chr(97) returns the string 'a'
#ord('a') returns the integer 97

import cs50
import sys

if len(sys.argv) != 2:
   print("Usage: python caesar.py k")
   exit(1)

k = int(sys.argv[1])

while k>26:
   k = k -26

plaintext = input("plaintext: ")
print("ciphertext: ", end="")

for char in plaintext:
   if char.isalpha():
       if char.islower():
           ciphertext = ord(char) + k
           if ciphertext > 122:
               ciphertext = ciphertext - 26
           print(chr(ciphertext), end="")
       elif char.isupper():
           ciphertext = ord(char) + k
           if ciphertext > 90:
              ciphertext = ciphertext - 26
           print(chr(ciphertext), end="")
   else:
       print(char, end="")
print()
