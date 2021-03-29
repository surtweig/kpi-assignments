
def reusedKeyAttack(encoded1, encoded2):

    size = min(len(encoded1), len(encoded2))

    
chars = [i for i in range(ord('a'), ord('z')+1)] + \
         [i for i in range(ord('A'), ord('Z')+1)] + \
         [i for i in range(ord('0'), ord('9')+1)] + \
         [ord(' '), ord('-'), ord('.'), ord(','), ord(';')]

def xorTest():
    s1 = "in hesitation, towards K. Bridge."
    s2 = "The landlady, who provided him wi"

    bs1 = bytes(s1, 'ascii')
    bs2 = bytes(s2, 'ascii')

    bsx = bytes([ bs1[i] ^ bs2[i] for i in range(len(bs1))])

    #print(chr(bsx[0] ^ ord('i')))

    for c in chars:
        if (c ^ bsx[0]) in chars:
            print(chr(c), chr(c^bsx[0]))

def BitFlippingAttack(encodedMsg, decodedMsg, adversaryMsg):
    encodedAdversary = []
    for i in range(len(encodedMsg)):
        if i < len(adversaryMsg):
            enc = encodedMsg[i]
            dec = decodedMsg[i]
            adv = adversaryMsg[i]
            encodedAdversary.append(adv ^ enc ^ dec)
    return bytes(encodedAdversary)
