from Ztransform import Ztransform
X = [1.0, 3.0, 4.0, 6.0, 8.0, 10.0]
zz = Ztransform([0, 0.001469317075123, -3.745071772821195e-06, -0.001465572003350], [1, -3.012587441009907, 3.004825736988187, -0.992382105542905], len(X))
#zz.x = [0, 1, 3, 2]
#zz.y = [3, 2, 1, 6]


for x in X:
    print("Input = {}".format(x))
    f = zz.control(x)
    print("Output = {}".format(f))
    print("------------")

print(zz.y)

# print("===========================")
# print("TEST2")
# print("===========================")
# zz = Ztransform([1, 2, 3], [2, 3, 4], 10)
# X = [4, 5, 6, 7, 8]

# for x in X:
#     print("Input = {}".format(x))
#     f = zz.control(x)
#     print("Output = {}".format(f))
#     print("------------")

# print(zz.y)
