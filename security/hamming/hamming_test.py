#%%
# Hamming test

from hamming import hammingEncode, hammingCheck, hammingDecode
from random import randint

def intToBoolList(x, n):
    bl = [0]*n
    for i in range(n):
        bl[i] = (x >> i) & 1
    return bl

def boolListToInt(bl):
    d = 1
    x = 0
    for i in range(len(bl)):
        if bl[i]:
            x += d
        d <<= 1
    return x


def hammingTest(x):
    #x = 0b1100101
    xlist = intToBoolList(x, 7)
    print("Payload {0} == {1}:".format(x, xlist))

    hcode = hammingEncode(11, xlist)
    print("Encoded {0}, checksum = {1}".format(hcode, hammingCheck(hcode)))

    errorBit = randint(0, len(hcode)-1)
    hcode[errorBit] = 1-hcode[errorBit]
    print("Error bit = {0}, check = {1}, hcode = {2}".format(errorBit, hammingCheck(hcode), hcode))

    ylist = hammingDecode(hcode)
    y = boolListToInt(ylist)
    print("Decoded {0} == {1}".format(ylist, y))

    if x == y:
        print("Test passed\n")
        return 1
    else:
        print("Test failed\n")
        return 0

#hammingTest(0b1100101)
hammingTest(1)

n = 100
c = 0
for i in range(n):
    c += hammingTest(randint(0, 0b1111111))

print("Passed {0}/{1}".format(c, n))


# %%
