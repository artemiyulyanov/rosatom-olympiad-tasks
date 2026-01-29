#include "brute.h"

#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

/**
 * A brute algorithm
 *
 * The asymptotic complexity of the algorithm is <b>O(n * m)</b>
 *
 * Applicable to n ≤ 10^6
 *
 * **/

bool valid_number(const int &n) {
    std::string s = std::to_string(n);
    int len = s.size();
    if (len < 3) return false;

    int min_idx = 0;
    for (int i = 1; i < len; i++) {
        if (s[i] - '0' < s[min_idx] - '0') min_idx = i;
    }

    if (min_idx < 1 || min_idx > len - 2) return false;

    for (int i = 1; i <= min_idx; i++) {
        if (s[i-1] <= s[i]) return false;
    }

    for (int i = min_idx+1; i < len; i++) {
        if (s[i-1] >= s[i]) return false;
    }

    return true;
}

ll count_numbers_brute(const int &n) {
    ll count = 0;

    for (int i = 100; i <= n; i++) {
        if (valid_number(i)) {
            count += 1;
        }
    }

    return count;
}