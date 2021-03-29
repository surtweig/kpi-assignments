
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



class StreamEncoder:

    def __init__(self, keystreamGenerator):
        self.keystreamGenerator = keystreamGenerator

    def encodeBytes(self, c):
        return bytes([c[i] ^ self.keystreamGenerator.getChar() for i in range(len(c))])
   
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



