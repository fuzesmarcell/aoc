#include <stdio.h>
#include <inttypes.h>

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {

    std::vector<int> a;
    std::vector<int> b;

    std::unordered_map<int, int64_t> similarity;

    for (std::string line; std::getline(std::cin, line);) {
        int aa, bb;
        sscanf(line.c_str(), "%d %d", &aa, &bb);
        // std::cout << aa << ":" << bb << "\n";

        a.push_back(aa);
        b.push_back(bb);

        ++similarity[bb];
    }

    int64_t result2 = 0;
    for (auto v : a) {
        if (similarity.find(v) != similarity.end()) {
            result2 += v*similarity[v];
        }
    }

    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    int64_t result = 0;
    for (uint64_t i = 0; i < a.size(); i++) {
        int64_t distance = a[i]-b[i];
        if (distance < 0)
            distance = -distance;
        result += distance;
    }

    std::cout << "Part 1 Result: " << result << "\n";

    std::cout << "Part 2 Result: " << result2 << "\n";


#if 0
    std::vector<int64_t> a = { 3, 4, 2, 1, 3, 3 };
    std::vector<int64_t> b = { 4, 3, 5, 3, 9, 3 };

    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    int64_t result = 0;
    for (uint64_t i = 0; i < a.size(); i++) {
        int64_t distance = a[i]-b[i];
        if (distance < 0)
            distance = -distance;
        result += distance;
    }

    std::cout << "Result: " << result << "\n";
#endif // 0


    return 0;
}