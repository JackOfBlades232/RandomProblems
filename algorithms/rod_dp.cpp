#include <iostream>
#include <cstring>
#include <alloca.h>

void print(int n, int res, int *sol)
{
    std::cout << "Best = " << res << '\n';
    std::cout << "Solution: ";
    for (int id = n - 1;; id = id - sol[id] - 1) {
        std::cout << sol[id] + 1 << ' ';
        if (id == sol[id])
            break;
    }

    std::cout << '\n';
}

void rec(int n, int c, int *p)
{
    int *r, *s;
    r = (int *)alloca(n * sizeof(*r));
    s = (int *)alloca(n * sizeof(*s));

    memset(r, 0xFF, n * sizeof(*r));
    memset(s, 0, n * sizeof(*s));

    auto helper = [&](const auto &self, int n) -> int {
        if (r[n - 1] < 0) {
            for (int i = 0; i < n - 2; ++i) {
                int val = self(self, i + 1)  + p[n - i - 2] + c;
                if (val > r[n - 1]) {
                    r[n - 1] = val;
                    s[n - 1] = n - i - 2;
                }
            }

            if (p[n - 1] > r[n - 1]) {
                r[n - 1] = p[n - 1];
                s[n - 1] = n - 1;
            }
        }

        return r[n - 1]; 
    };

    auto solve = [&helper](int n) -> int { return helper(helper, n); };

    int res = solve(n);
    print(n, res, s);
}

void proc(int n, int c, int *p)
{
    int *r, *s;
    r = (int *)alloca(n * sizeof(*r));
    s = (int *)alloca(n * sizeof(*s));

    memset(r, 0xFF, n * sizeof(*r));
    memset(s, 0, n * sizeof(*s));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i - 1; ++j) {
            int val = r[j] + p[i - j - 1] + c;
            if (val > r[i]) {
                r[i] = val;
                s[i] = i - j - 1;
            }
        }

        if (p[i] > r[i]) {
            r[i] = p[i];
            s[i] = i;
        }
    }

    int res = r[n - 1];
    print(n, res, s);
}

int main()
{
    int n, c;
    int *p;
    std::cin >> n;
    std::cin >> c;
    p = (int *)alloca(n * sizeof(*p));

    for (int i = 0; i < n; ++i)
        std::cin >> p[i];

    std::cout << "Proc:\n";
    proc(n, c, p);
    std::cout << "Rec:\n";
    rec(n, c, p);
    return 0;
}
