#include <iostream>
#include <string>
#include <vector>

bool chars_are_equal(char c1, char c2)
{
    return  c1 == c2; //|| c1 == '?' || c2 == '?';
}

std::vector<int> contstruct_prefix_table(std::string pattern)
{
    std::vector<int> table(pattern.size()+1);
    table[0] = -1;
    table[1] = 0;
    int prefix_len = 0;
    for (int i = 1; i < pattern.size(); i++) {
        if (chars_are_equal(pattern[i], pattern[prefix_len]))
            table[i+1] = ++prefix_len;
        else {
            while (prefix_len > 0 && !chars_are_equal(pattern[i], pattern[prefix_len])) {
                prefix_len = table[prefix_len];
            } 
            table[i+1] = chars_are_equal(pattern[i], pattern[prefix_len]) ? ++prefix_len : 0;
        }

        //printf("%d\n", table[i+1]);
    }

    return table;
}

int main()
{
    std::string pattern;
    std::string text;

    std::cin >> pattern >> text;
    std::vector<int> table = contstruct_prefix_table(pattern);

    int count = 0;

    int p = 0;
    int t = 0;
    while (t < text.size()) {
        if (chars_are_equal(text[t], pattern[p])) {
            p++;
            t++;
            if (p >= pattern.size()) {
                count++;
                p = table[p];
                printf("%d %d\n", t, p);
            }
        } else if (p == 0) {
            p++;
            t++;
        } else
            p = table[p];
    }

    printf("%d\n", count);

    return 0;
}
