from stream_cipher import StreamEncoder

class CaesarEncoder(StreamEncoder):
    def __init__(self, shift, alphabet):
        self.shift = shift
        self.alphabet = alphabet
        self.alphabetMap = {alphabet[i]:i for i in range(len(alphabet))}

    def encodeChar(self, c):
        if c in self.alphabetMap:
            return self.alphabet[(self.alphabetMap[c] + self.shift + len(self.alphabet)) % len(self.alphabet)]
        else:
            print("'{0}' is not in the alphabet".format(chr(c)))

    def createDecoder(self):
        return CaesarEncoder(-self.shift, self.alphabet)        
