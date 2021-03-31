#%%
# Caesar cipher

from caesar_cipher import CaesarEncoder
from stream_cipher import readFileBytes


alphabet = [i for i in range(ord('a'), ord('z')+1)] + \
           [i for i in range(ord('A'), ord('Z')+1)] + \
           [i for i in range(ord('0'), ord('9')+1)] + \
           [ord(' '), ord('-'), ord('.'), ord(','), ord(';')]

encoder = CaesarEncoder(7, alphabet)

originalMessage = readFileBytes("original2.txt")
encodedMessage = encoder.encodeBytes(originalMessage)

decoder = encoder.createDecoder()
decodedMessage = decoder.encodeBytes(encodedMessage)

print("=== Original ===")
print(originalMessage.decode("ascii"))
print("\n=== Encoded ===")
print(encodedMessage.decode("ascii"))
print("\n=== Decoded ===")
print(decodedMessage.decode("ascii"))
# %%
