#include <alloca.h>
#include <iostream>
#include <cstring>

struct matrix_t {
    int r, c;
    float *val;

    float *Row(int row_id) { return val + row_id * c; }
    const float *Row(int row_id) const { return val + row_id * c; }
};

#define ALLOCA_MATRIX_VAL(m_) (float *)alloca((m_).r * (m_).c * sizeof(*(m_).val))

struct sol_entry_t {
    unsigned pivot;
    unsigned cost;
};

struct sol_table_t {
    sol_entry_t *val; // "pyramid" with lowest level of n-1

    sol_entry_t &At(int group_size, int group_start, int n) {
        int level_id = group_size - 1;
        return val[level_id * n - level_id * (level_id - 1) / 2 + group_start];
    }

    const sol_entry_t &At(int group_size, int group_start, int n) const {
        return const_cast<sol_table_t *>(this)->At(group_size, group_start, n);
    }

    int ReqSize(int n) const { return n * (n + 1) / 2; }
};

bool matrix_multiply(const matrix_t &m1, const matrix_t &m2, matrix_t &out)
{
    if (m1.c != m2.r || out.r != m1.r || out.c != m2.c)
        return false;

    int common_dim = m1.c;

    for (int i = 0; i < m1.r; ++i)
        for (int j = 0; j < m2.c; ++j) {
            float &dest = out.Row(i)[j];
            dest = 0.f;

            for (int k = 0; k < common_dim; ++k)
                dest += m1.Row(i)[k] * m2.Row(k)[j];
        }

    return true;
}

void find_optimal_sol_strategy(const matrix_t *matrices, sol_table_t &sol_table, int n)
{
    memset(sol_table.val, 0x0, n * sizeof(*sol_table.val));
    memset(sol_table.val + n, 0xFF, (sol_table.ReqSize(n) - n) * sizeof(*sol_table.val));

    for (int group_size = 2; group_size <= n; ++group_size)
        for (int group_start = 0; group_start <= n - group_size; ++group_start) {
            sol_entry_t &entry = sol_table.At(group_size, group_start, n);

            for (int pivot = 1; pivot < group_size; ++pivot) {
                unsigned cost =
                    sol_table.At(pivot, group_start, n).cost +
                    sol_table.At(group_size - pivot, group_start + pivot, n).cost +
                    matrices[group_start].r * matrices[group_start + pivot].r * matrices[group_start + group_size - 1].c;
                
                if (cost < entry.cost) {
                    entry.cost = cost;
                    entry.pivot = pivot;
                }
            }
        }
}

void multiply_matrices_with_strategy(const matrix_t *matrices,
                                     const sol_table_t &sol_table, int n,
                                     matrix_t &res)
{
    auto helper = [&](const auto &self, int group_size, int group_start, matrix_t &out) -> void {
        if (group_size == 1) // Memcpy could be omitted with uglier code
            memcpy(out.val, matrices[group_start].val, out.r * out.c * sizeof(*out.val));
        else {
            const sol_entry_t &entry = sol_table.At(group_size, group_start, n);
            matrix_t m1 = {matrices[group_start].r, matrices[group_start + entry.pivot].r, nullptr};
            matrix_t m2 = {matrices[group_start + entry.pivot].r, matrices[group_start + group_size - 1].c, nullptr};
            m1.val = ALLOCA_MATRIX_VAL(m1);
            m2.val = ALLOCA_MATRIX_VAL(m2);

            self(self, entry.pivot, group_start, m1);
            self(self, group_size - entry.pivot, group_start + entry.pivot, m2);

            matrix_multiply(m1, m2, out);
        }
    };

    helper(helper, n, 0, res);
}

void print_result(const matrix_t &res, const sol_table_t &sol_table, int n)
{
    std::cout << "Grouping:\n";
    auto helper = [&](const auto &self, int group_size, int group_start) -> void {
        if (group_size == 1)
            std::cout << 'M' << group_start << ' ';
        else {
            const sol_entry_t &entry = sol_table.At(group_size, group_start, n);
            std::cout << "( ";
            self(self, entry.pivot, group_start);
            std::cout << "* ";
            self(self, group_size - entry.pivot, group_start + entry.pivot);
            std::cout << ") ";
        }
    };
    helper(helper, n, 0);

    std::cout << "\nCost: " << sol_table.At(n, 0, n).cost << '\n';
    std::cout << "Solution:\n";

    for (int i = 0; i < res.r; ++i) {
        for (int j = 0; j < res.c; ++j)
            std::cout << res.Row(i)[j] << ' ';
        std::cout << '\n';
    }
}

int main()
{
    int n;
    matrix_t *matrices;

    std::cin >> n;
    matrices = (matrix_t *)alloca(n * sizeof(*matrices));

    for (int i = 0; i < n; ++i) {
        matrix_t &m = matrices[i]; 
        std::cin >> m.r >> m.c;
        m.val = ALLOCA_MATRIX_VAL(m);

        for (int j = 0; j < m.r * m.c; ++j)
            std::cin >> m.val[j];
    }

    sol_table_t table = {};
    table.val = (sol_entry_t *)alloca(table.ReqSize(n) * sizeof(*table.val));

    find_optimal_sol_strategy(matrices, table, n);

    matrix_t result;
    result.r = matrices[0].r;
    result.c = matrices[n - 1].c;
    result.val = ALLOCA_MATRIX_VAL(result);

    multiply_matrices_with_strategy(matrices, table, n, result);

    print_result(result, table, n);

    return 0;
}
