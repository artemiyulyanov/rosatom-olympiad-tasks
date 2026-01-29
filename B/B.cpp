#include "cstring"
#include <iostream>

#include "brute.h"

/**
 * Dynamic digit-based programming (Digit DP)
 *
 *
 * The asymptotic complexity of the current algorithm is <b>O(|n|)</b>
 *
 * Applicable to <b>n ≤ 10^18</b>
 *
 * Memory shutdown at <b>n > 10^20</b>
 *
 * **/

typedef long long ll;

ll dp[20][11][3][2][2];

ll count_numbers(const std::string &num) {
    std::memset(dp, 0, sizeof(dp));
    int len = num.size();

    for (int prev = 0; prev <= 10; prev++) {
        for (int state = 0; state <= 2; state++) {
            for (int has_desc = 0; has_desc <= 1; has_desc++) {
                for (int tight = 0; tight <= 1; tight++) {
                    dp[len][prev][state][has_desc][tight] = (has_desc && state == 1) ? 1 : 0;
                }
            }
        }
    }

    for (int pos = len-1; pos >= 0; pos--) {
        for (int prev = 0; prev <= 10; prev++) {
            for (int state = 0; state <= 2; state++) {
                for (int has_desc = 0; has_desc <= 1; has_desc++) {
                    for (int tight = 0; tight <= 1; tight++) {
                        ll ans = 0;
                        int limit = tight ? num[pos]-'0' : 9;

                        for (int digit = 0; digit <= limit; digit++) {
                            int new_tight = (tight && digit == limit);

                            if (prev == 10) {
                                if (digit == 0) ans += dp[pos+1][prev][state][has_desc][new_tight];
                                else ans += dp[pos+1][digit][state][has_desc][new_tight];
                            } else {
                                if (state == 2 && digit < prev) {
                                    ans += dp[pos+1][digit][0][1][new_tight];
                                } else if (state == 1 && digit > prev) {
                                    ans += dp[pos+1][digit][state][has_desc][new_tight];
                                } else if (state == 0) {
                                    if (digit < prev)
                                        ans += dp[pos+1][digit][state][has_desc][new_tight];
                                    else if (digit > prev)
                                        ans += dp[pos+1][digit][1][has_desc][new_tight];
                                }
                            }
                        }

                        dp[pos][prev][state][has_desc][tight] = ans;
                    }
                }
            }
        }
    }

    return dp[0][10][2][0][1];
}

int main() {
    std::string num;
    std::getline(std::cin, num);

    std::cout << count_numbers(num) << std::endl;
    std::cout << count_numbers_brute(std::stoi(num)) << std::endl;

    return 0;
}