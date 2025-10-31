import random
import bisect


class SmoothSpline:
    def __init__(self, x, y, weights=None, p=0.5):
        if not (0.0 <= p <= 1.0):
            raise ValueError("p must be in [0, 1]")

        n = len(x)
        if n == 0:
            self.x = []
            self.g = []
            return
        if len(y) != n:
            raise ValueError("x and y sizes differ")

        if weights is None:
            weights = [1.0] * n
        elif len(weights) != n:
            raise ValueError("weights size differs from x and y")

        self.x = x[:]

        if n == 1:
            self.g = [y[0]]
            return

        h = []
        for i in range(n - 1):
            dx = x[i + 1] - x[i]
            if dx <= 0:
                raise ValueError("x must be strictly increasing")
            h.append(dx)

        lower = [0.0] * n
        main = [0.0] * n
        upper = [0.0] * n
        rs = [0.0] * n

        lower[0] = 0.0
        main[0] = (1 - p) * weights[0] + p / h[0]
        upper[0] = -p / h[0]
        rs[0] = (1 - p) * weights[0] * y[0]

        for k in range(1, n - 1):
            lower[k] = -p / h[k - 1]
            main[k] = (1 - p) * weights[k] + p / h[k - 1] + p / h[k]
            upper[k] = -p / h[k]
            rs[k] = (1 - p) * weights[k] * y[k]

        lower[n - 1] = -p / h[n - 2]
        main[n - 1] = (1 - p) * weights[n - 1] + p / h[n - 2]
        upper[n - 1] = 0.0
        rs[n - 1] = (1 - p) * weights[n - 1] * y[n - 1]

        self.g = self._thomas(lower, main, upper, rs)

    @staticmethod
    def _thomas(lower, main, upper, rs):
        n = len(rs)
        result = [0.0] * n
        upper_ext = [0.0] * n
        rs_ext = [0.0] * n

        upper_ext[0] = upper[0] / main[0]
        rs_ext[0] = rs[0] / main[0]

        for i in range(1, n):
            denom = main[i] - lower[i] * upper_ext[i - 1]
            if denom == 0:
                raise ZeroDivisionError("Singular tridiagonal system")
            m = 1.0 / denom
            upper_ext[i] = upper[i] * m
            rs_ext[i] = (rs[i] - lower[i] * rs_ext[i - 1]) * m

        result[-1] = rs_ext[-1]
        for i in range(n - 2, -1, -1):
            result[i] = rs_ext[i] - upper_ext[i] * result[i + 1]

        return result

    def __call__(self, x_val):
        n = len(self.x)
        if n == 0:
            return 0.0
        if n == 1:
            return self.g[0]

        if x_val <= self.x[0]:
            return self.g[0]
        if x_val >= self.x[-1]:
            return self.g[-1]

        k = bisect.bisect_right(self.x, x_val) - 1
        h_k = self.x[k + 1] - self.x[k]
        return self.g[k] * (self.x[k + 1] - x_val) / h_k + self.g[k + 1] * (x_val - self.x[k]) / h_k