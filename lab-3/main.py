from smooth_spline.smooth_spline import SmoothSpline
import random

def generate_normal(count, mean, stddev):
    data = []
    for _ in range(count):
        u1 = random.random()
        u2 = random.random()
        z = (-2 * (u1 if u1 > 0 else 1e-10)) ** 0.5 * (2 * 3.141592653589793 * u2)
        data.append(random.gauss(mean, stddev))
    return data


def write_spline(filename, splines, x, d):
    with open(filename, "w") as f:
        f.write(" ".join(str(v) for v in x) + "\n")
        f.write(" ".join(str(v) for v in d) + "\n")
        for sp in splines:
            f.write(" ".join(str(sp(xi)) for xi in x) + "\n")

def main():
    random.seed()

    N = 1157
    x = [float(i) for i in range(N)]
    d = generate_normal(N, 1.05, 5.5)
    w_uniform = [1.0] * N

    sp_0 = SmoothSpline(x, d, w_uniform, 0.0)
    sp_4 = SmoothSpline(x, d, w_uniform, 0.4)
    sp_8 = SmoothSpline(x, d, w_uniform, 0.8)
    sp_99 = SmoothSpline(x, d, w_uniform, 0.99)

    w_variable = w_uniform[:]
    for i in range(200, 700):
        w_variable[i] = 0.5

    sp_0w = SmoothSpline(x, d, w_variable, 0.0)
    sp_4w = SmoothSpline(x, d, w_variable, 0.4)
    sp_8w = SmoothSpline(x, d, w_variable, 0.8)
    sp_99w = SmoothSpline(x, d, w_variable, 0.99)

    write_spline("spline_uniform_weights.txt", [sp_0, sp_4, sp_8, sp_99], x, d)
    write_spline("spline_variable_weights.txt", [sp_0w, sp_4w, sp_8w, sp_99w], x, d)


if __name__ == "__main__":
    main()