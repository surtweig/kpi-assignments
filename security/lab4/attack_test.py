#%%
# attack_test
from stream_cipher import KeystreamGenerator, StreamEncoder
from attack import BitFlippingAttack

def readFileBytes(fileName):
    f = open(fileName, "rb")
    buffer = []
    c = f.read(1)
    while c:
        buffer.append(int(c[0]))
        c = f.read(1)
    return bytes(buffer)

originalMessage = readFileBytes("original.txt")
adversaryMessage = readFileBytes("original2.txt")

print("=== Original ===")
print(originalMessage.decode("ascii"))
print("\n=== Adversary ===")
print(adversaryMessage.decode("ascii"))

generator = KeystreamGenerator(12345)
encoder = StreamEncoder(generator)

encodedMessage = encoder.encodeBytes(originalMessage)

encodedAdversary = BitFlippingAttack(encodedMessage, originalMessage, adversaryMessage)

generator.reset()
decodedAdversary = encoder.encodeBytes(encodedAdversary)

print("\n=== Decoded adversary ===")
print(decodedAdversary.decode("ascii"))

# %%
