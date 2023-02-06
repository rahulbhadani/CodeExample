def calculate_y(a, b, x, y_prev):
    n = len(a)
    m = len(b)
    y_k = (sum(a[i] * x[k-i] for i in range(n)) - sum(b[j] * y_prev[k-j] for j in range(1, m))) / b[0]
    return y_k

a = [0, 0.001469317075123, -3.745071772821195e-06, -0.001465572003350]
b =  [1, -3.012587441009907, 3.004825736988187, -0.992382105542905]
x = [1.0, 3.0, 4.0, 6.0, 8.0, 10.0]
y_prev = [0, 0, 0, 0]

for k in range(len(x)):
    y_k = calculate_y(a, b, x, y_prev)
    y_prev.append(y_k)
    print(f"y[{k}] = {y_k}")


