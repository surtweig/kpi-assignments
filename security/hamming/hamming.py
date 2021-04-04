#%%
# Hamming code

def hammingEncode(size, payload):

    code = [0]*size

    checksum = 0
    checksumIndices = []
    ci = 1
    i = 0
    # calculating checksum bits positions
    while ci < size:
        checksumIndices.append(ci-1)
        ci <<= 1

    ci = 0 # checksum index
    pi = 0 # payload index
    for i in range(size):
        # checking if [i] is data or checksum bit
        if ci >= len(checksumIndices) or i != checksumIndices[ci]:
            #print("{0}: payload #{1} = {2}".format(i+1, pi, payload[pi]))
            code[i] = payload[pi]
            if payload[pi]: # adding every payload bit==1 position to checksum
                checksum ^= i+1
            pi += 1
        else:            
            #print("{0}: checksum #{1}".format(i+1, ci))
            ci += 1

    #print("\nchecksum bits:\n")
    # writing checksum
    for i in range(len(checksumIndices)):
        ci = checksumIndices[i]
        code[ci] = (int)(checksum & (1 << i) > 0)
        #print("{0}: checksum #{1} = {2}".format(ci+1, i, code[ci]))

    return code

def hammingCheck(code):
    fullChecksum = 0
    for i in range(len(code)):
        if code[i]:
            fullChecksum ^= i+1
            #print("check {0} == 1 fullchecksum = {1:b} pos = {2:b}".format(i+1, fullChecksum, i+1))
    return fullChecksum

def hammingDecode(code):

    # error correction
    fullChecksum = hammingCheck(code)
    if fullChecksum != 0:
        if fullChecksum-1 < len(code):
            print("decode correction {0}".format(fullChecksum-1))
            code[fullChecksum-1] = 1 - code[fullChecksum-1]
        else:
            print("decode error: error position {0} is out of range ({1})".format(fullChecksum-1, len(code)))

    payload = []
    pi = 0
    ci = 1
    for i in range(len(code)):
        if i != ci-1:
            #print("decode payload[{0}] at {1}".format(pi, i+1))
            payload.append(code[i])
        else:
            ci <<= 1

    return payload
            




# %%
