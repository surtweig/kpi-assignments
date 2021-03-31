#%%
# RSA

class PrimeGenerator:
    def __init__(self, start = 2):
        self.reset(start)

    def reset(self, start = 2):
        self.i = start
        self.sieve = {}
    
    def next(self):    
        while True:
            if self.i not in self.sieve:
                self.sieve[self.i * self.i] = [self.i]
                self.i += 1
                return self.i-1
            else:
                for j in self.sieve[self.i]:
                    self.sieve.setdefault(self.i + j, []).append(j)
                del self.sieve[self.i]
                self.i += 1


            

class RSAEncoder:

    def __init__(self):
        self.keyD = None
        self.keyE = None
        self.blockSize = 0
        

    def rsaGenerateKeypair(self, seed = None):
        from random import Random

        rand = None
        if seed is not None:
            rand = Random()
        else:
            rand = Random(seed)

        pPrimeIndex = rand.randint(2, 20)
        qPrimeIndex = rand.randint(2, 20)

        p = 0
        q = 0
        primeGen = PrimeGenerator()

        for i in range(max(pPrimeIndex, qPrimeIndex)+1):
            prime = primeGen.next()
            if i == pPrimeIndex:
                p = prime
            if i == qPrimeIndex:
                q = prime


        n = p*q
        m = (p-1)*(q-1)

        print("p = {0}, q = {1} m = {2}".format(p, q, m))

        from math import gcd

        coprimeStart = rand.randint(1, m-1)
        d = 0
        e = 0
        for i in range(1, m):
            d = (coprimeStart + i) % m
            if gcd(d, m) == 1:
                break

        for i in range(1, m):
            if (i*d) % m == 1:
                e = i
                break

        print("d = {0}, e = {1}".format(d, e))

        self.keyD = d
        self.keyE = e
        self.n = n
        from math import log2, ceil, log
        self.blockSize = ceil(log(n) / log(256))
        print("n = {0}, blocksize = {1}".format(self.n, self.blockSize))

    def modPow(a, b, m):
        if m == 1:
            return 0
        c = 1
        for i in range(b):
            c = (c * a) % m
        return c



    def n2b(n, base):
        b = []
        while n > 0:
            sh = n >> base
            sh = sh << base
            b.append(n - sh)
            n = sh >> base
        return b        

    def rsaEncodeNumber(self, key, x):
        return RSAEncoder.modPow(x, key, self.n)

    def resizeBytes(data, n):
        sequence = []
        carry = 0
        accum = 0
        from math import log2, ceil, log
        wordBytes = ceil(log(n) / log(256))
        pos = 0


        # wordI = 0
        # for i in range(len(data)):
            
        #     accum = (accum << 8) + int(data[i])
        #     wordI += 1

        #     if wordI == wordBytes or accum >= n:
        #         digit = accum % n
        #         accum -= digit
        #         sequence.append(digit)
        #         wordI = 0

        # while accum != 0:
        #     digit = accum % n
        #     accum -= digit
        #     sequence.append(accum)


        return sequence


    def rsaEncode(self, key, data):

        code = []

        block = [0]*self.blockSize
        bi = 0
        dataNum = 0
        encodedNum = 0

        for i in range(len(data)):
            if bi >= self.blockSize:
                encodedNum = RSAEncoder.modPow(dataNum, key, self.n)
                print("data = {0}, encoded = {1}".format(dataNum, encodedNum))
                for j in range(self.blockSize):
                    sh = encodedNum >> 8
                    sh = sh << 8                    
                    code.append(encodedNum - sh)
                    encodedNum = sh >> 8
                dataNum = 0
                bi = 0

            block[bi] = data[i]
            dataNum = (dataNum << 8) + int(data[i])
            #print(data[i], int(data[i]), dataNum, dataNum << 8 + int(data[i]))
            bi += 1

        return bytes(code)


encoder = RSAEncoder()
encoder.rsaGenerateKeypair()

print(RSAEncoder.resizeBytes([123,61,17], 1024))


def x2base(x, n):
    digits = []
    while x != 0:
        d = x % n
        x //= n
        digits.append(d)
    return digits

def base2x(digits, n):
    x = 0
    p = 1
    for i in range(len(digits)):
        x += digits[i] * p
        p *= n
    return x

def d2a(digits, alphabet):
    s = ""
    for i in range(len(digits)):
        s += chr(alphabet[digits[i]]) 
    return s

def a2d(text, alphabetmap):
    return [alphabetmap[ord(text[i])] for i in range(len(text))]


print(x2base(123, 16))
print(base2x(x2base(123, 16), 16))

alphabet = [i for i in range(ord('a'), ord('z')+1)] + \
           [i for i in range(ord('A'), ord('Z')+1)] + \
           [i for i in range(ord('0'), ord('9')+1)] + \
           [ord(' '), ord('-'), ord('.'), ord(','), ord(';')]
alphabetmap = {alphabet[i]:i for i in range(len(alphabet))}


s = "test"
s_digits = a2d(s, alphabetmap)
print("d2a({0}) = {1}".format(s_digits, d2a(s_digits, alphabet)))

s_num = base2x(s_digits, len(alphabet))
s_num_splitted = x2base(s_num, encoder.n)

s_num_merged = base2x(s_num_splitted, encoder.n)
s_digits_merged = x2base(s_num_merged, len(alphabet))
s_merged = d2a(s_digits_merged, alphabet)

print("s_num = {0}, s_num_splitted = {1}".format(s_num, s_num_splitted))
print("s_num_merged = {0}, s_digits_merged = {1}, s_merged = {2}".format(s_num_merged, s_digits_merged, s_merged))

# encode
x_num_splitted = [encoder.rsaEncodeNumber(encoder.keyD, s_num_splitted[i]) for i in range(len(s_num_splitted))]

# decode
d_num_splitted = [encoder.rsaEncodeNumber(encoder.keyE, x_num_splitted[i]) for i in range(len(s_num_splitted))]

d_num_merged = base2x(d_num_splitted, encoder.n)
d_digits_merged = x2base(d_num_merged, len(alphabet))
d_merged = d2a(d_digits_merged, alphabet)
print("d_num_merged = {0}, d_digits_merged = {1}, d_merged = {2}".format(d_num_merged, d_digits_merged, d_merged))

# 
# x = base2x(a2d("test"), len(alphabet))
# xsplitted = x2base(x, encoder.n)

# tmerged = base2x(xsplitted, encoder.n)
# t = d2a(x2base(tmerged, len(alphabet)), alphabetMap)
# #t = x2base(tmerged, len(alphabet))

# xe = encoder.rsaEncodeNumber(encoder.keyD, x)
# xd = encoder.rsaEncodeNumber(encoder.keyE, xe)

# print("x = {0} xe = {1} xd = {2} t = '{3}'".format(x, xe, xd, t))

# def readFileBytes(fileName):
#     f = open(fileName, "rb")
#     buffer = []
#     c = f.read(1)
#     while c:
#         buffer.append(int(c[0]))
#         c = f.read(1)
#     return bytes(buffer)


# original = readFileBytes("original.txt")
# print("=== Original ===")
# print(original.decode("ascii"))

# enc = encoder.rsaEncode(encoder.keyD, original)
# print("\n=== Encoded ===")
# print(enc.decode("ascii"))

# print("\n=== Decoded ===")
# dec = encoder.rsaEncode(encoder.keyE, enc)
# print(dec.decode("ascii"))


# %%
