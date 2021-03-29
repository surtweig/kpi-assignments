#%%
# stream_cipher_test
from stream_cipher import KeystreamGenerator, StreamEncoder

generator = KeystreamGenerator(12345)
encoder = StreamEncoder(generator)

encoder.encodeFile("original.txt", "encoded.txt")
generator.reset()
encoder.encodeFile("encoded.txt", "decoded.txt")


size = 1000
import random
msg = bytes([random.randint(0, 255) for i in range(size)])

generator.reset()
msgenc = encoder.encodeBytes(msg)

generator.reset()
msgdec = encoder.encodeBytes(msgenc)

check = True
for i in range(len(msg)):
    if msg[i] != msgdec[i]:
        check = False
        break

if check:
    print("Test passed")
else:
    print("Test failed")

# %%
