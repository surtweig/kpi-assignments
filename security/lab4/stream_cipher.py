
class KeystreamGenerator:

    def __init__(self, seed):
        from random import Random
        self.seed = seed
        self.generator = Random(self.seed)

    def reset(self):
        self.generator.seed(self.seed)

    def getChar(self):
        return self.generator.randint(0, 255)

    def getString(self, size):
        return [self.getChar() for i in range(size)]

class LCGKeystreamGenerator(KeystreamGenerator):

    def __init__(self, start):
        self.m = 2<<32
        self.a = 1664525
        self.c = 1013904223
        self.t = start
        self.start = start

    def reset(self):
        self.t = self.start

    def getChar(self):
        self.t = (self.a * self.t + self.c) % self.m
        return self.t % 256

class StreamEncoder:

    def __init__(self, keystreamGenerator):
        self.keystreamGenerator = keystreamGenerator

    def encodeChar(self, c):
        return c ^ self.keystreamGenerator.getChar()

    def encodeBytes(self, b):
        return bytes([self.encodeChar(b[i]) for i in range(len(b))])
   
    def encodeFile(self, fileNameIn, fileNameOut):
        fileIn = open(fileNameIn, 'rb')
        fileOut = open(fileNameOut, 'wb')

        c = fileIn.read(1)
        while c:
            cenc = self.encodeBytes(c)
            fileOut.write(cenc)
            c = fileIn.read(1)

        fileIn.close()
        fileOut.close()



def readFileBytes(fileName):
    f = open(fileName, "rb")
    buffer = []
    c = f.read(1)
    while c:
        buffer.append(int(c[0]))
        c = f.read(1)
    return bytes(buffer)