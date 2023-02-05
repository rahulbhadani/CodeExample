#!/usr/bin/env python
# coding: utf-8


import zmq
import time
import numpy as np
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
# class ZTF:
#     """
#     Z-domain transfer function

#     H(z) = (n0 + n1*z + ... nn*z**n) / (d0 + d1*z + ... dm*z**m)  ->

# 	H(z) = (n0*z**-m + n1*z**-(m-1) + ... + nn*z**(n-m) ) /
# 		   (d0*z**-m + d1*z**-(m-1) + ... + dm)

#     """
#     def __init__(self, num, denom):
#         """
#         num: `list`
#             Coefficients in Numerators in ascending order of powers of Z.

#         denom: `list`
#             Coefficients in Denominators in descending order of powers of Z.


#         """
#         self.num = num
#         self.denom = denom

#         self.nsize = len(num)
#         self.dsize = len(denom)

#         # reverse the coefficients
#         self.rnum = self.num[::-1]
#         self.rdenom = self.denom[::-1]

#         # delay chains
#         self.zx = [0.0]*self.dsize
#         self.zy = [0.0]*self.nsize

#     def processing(self, in_value):
#         self.zx[0] = in_value

#         zx_tmp = self.zx[self.dsize - self.nsize : self.dsize-self.nsize + self.nsize]

#         zx_tmp2 = [a*b for a,b in zip(self.rnum, zx_tmp)]
#         y = sum(zx_tmp2)

#         tmp3 = [a*b for a,b in zip(self.rdenom, self.zy)]
#         y = y - sum(tmp3)
#         y = y/self.rdenom[0];

#         # for the next calculation Z**-1
#         self.zy[0] = y;

#         self.zx = self.zx[-1:] + self.zx[:-1]
#         self.zy = self.zy[-1:] + self.zy[:-1]

#         self.zy[0] = 0.0;
        
#         return y

