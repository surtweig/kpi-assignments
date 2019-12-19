from Matrix import Matrix
from threading import Thread, Lock

class F(Thread):
    def __init__(self, size, printLock):
        Thread.__init__(self)
        self.printLock = printLock
        self.result = None
        self.size = size
        self.name = "F"

    def calculate(self):
        return

    def run(self):
        self.printLock.acquire()
        print(self.name + " started.")
        self.printLock.release()

        self.calculate()

        self.printLock.acquire()
        print(self.name + ":\n" + repr(self.result) + "\n")
        self.printLock.release()

# 1.5 C = SORT(A) *(MA*ME) + SORT(B)
class F1(F):
    def __init__(self, size, printLock):
        F.__init__(self, size, printLock)
        self.name = "F1"

    def calculate(self):
        A = Matrix(self.size, 1)
        B = Matrix(self.size, 1)
        MA = Matrix(self.size, self.size)
        ME = Matrix(self.size, self.size)
        A.fillRandom()
        B.fillRandom()
        MA.fillRandom()
        ME.fillRandom()

        mae = MA.multiply(ME)
        A.sort()
        B.sort()
        self.result = mae.multiply(A)
        self.result.add(B)

# 2.5 MG = SORT(MF) * MK + ML
class F2(F):
    def __init__(self, size, printLock):
        F.__init__(self, size, printLock)
        self.name = "F2"

    def calculate(self):
        MF = Matrix(self.size, self.size)
        MK = Matrix(self.size, self.size)
        ML = Matrix(self.size, self.size)
        MF.fillRandom()
        MK.fillRandom()
        ML.fillRandom()
        
        MF.sort()
        self.result = MF.multiply(MK)
        self.result.add(ML)

# 3.5 O = (SORT(MP*MR)*S)
class F3(F):
    def __init__(self, size, printLock):
        F.__init__(self, size, printLock)
        self.name = "F3"

    def calculate(self):
        MP = Matrix(self.size, self.size)
        MR = Matrix(self.size, self.size)
        S = Matrix(self.size, 1)
        MP.fillRandom()
        MR.fillRandom()
        S.fillRandom()

        mpr = MP.multiply(MR)
        mpr.sort()
        self.result = mpr.multiply(S)



printLock = Lock()
tf1 = F1(5, printLock)
tf2 = F2(5, printLock)
tf3 = F3(5, printLock)

#tf1.calculate()
#tf2.calculate()
#tf3.calculate()

tf1.start()
tf2.start()
tf3.start()

tf1.join()
tf2.join()
tf3.join()