import time
import math

class ZTF:
    def __init__(self, num, den):
        self.num = num
        self.den = den

        self.Ts = 0.05

        self.rnum = list(reversed(num))
        self.rden = list(reversed(den))
        self.zx = [0.0] * len(den)
        self.zy = [0.0] * len(den)

        self.nsize = len(num)
        self.dsize = len(den)

    def print_coefficients(self):
        print("Numerator Coefficients:", self.rnum)
        print("Denominator Coefficients:", self.rden)

    def processing(self, x):
        self.zx[0] = x

        y = sum(self.rnum[i] * self.zx[self.dsize - self.nsize + i] for i in range(self.nsize))
        y -= sum(self.rden[i] * self.zy[i] for i in range(self.dsize))
        y /= self.rden[0]

        self.zy[0] = y
        self.zx = self.zx[-1:] + self.zx[:-1]
        self.zy = self.zy[-1:] + self.zy[:-1]

        self.zy[0] = 0.0

        return y

class PID:
    def __init__(self, P, I, D, N, Ts):
        self.P = P
        self.I = I
        self.D = D
        self.N = N
        self.Ts = Ts

        self.ztf1 = ZTF([P], [1.0])
        self.ztf2 = ZTF([I * Ts], [-1.0, 1.0])
        self.ztf3 = ZTF([-D * N, D * N], [(N * Ts - 1), 1.0])

    def processing(self, error):
        return self.ztf1.processing(error) + self.ztf2.processing(error) + self.ztf3.processing(error)

# if __name__ == "__main__":
#     P = 12.2285752621432
#     I = 7.40871870543199
#     D = 4.88097496311707
#     N = 37.0569659936971
#     Ts = 0.05

#     pid = PID(P, I, D, N, Ts)
#     start_time = time.time()
#     out = 0
#     x = 0

#     print("Input, Output")
#     while True:
#         elapsed_time = time.time() - start_time
#         if elapsed_time >= 10.0:
#             break

#         t = elapsed_time
#         x = 3 * math.sin(t)
#         out = pid.processing(x)
#         print("{}, {}".format(x, out))
#         time.sleep(Ts)
