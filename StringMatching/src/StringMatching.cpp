#include "StringMatching.h"

extern void StringMatching_test(const std::string &pattern, const std::string &text)
{
    double time = 0;
    std::vector<int> ans;

    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Matching method: BruteForce" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        ans = bruteForceSearch(pattern, text);
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
    }
    std::cout << "Answer: ";
    if (ans.size() == 0)
        std::cout << "No Match." << std::endl;
    else
    {
        for (size_t i = 0; i < ans.size(); i++)
            std::cout << ans[i] << " ";
        std::cout << std::endl;
    }
    std::cout << "Time cost: " << time << "ms" << std::endl;

    ans.clear();
    time = 0;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Matching method: KMP" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        ans = kmpSearch(pattern, text);
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
    }
    std::cout << "Answer: ";
    if (ans.size() == 0)
        std::cout << "No Match." << std::endl;
    else
    {
        for (size_t i = 0; i < ans.size(); i++)
            std::cout << ans[i] << " ";
        std::cout << std::endl;
    }
    std::cout << "Time cost: " << time << "ms" << std::endl;

    ans.clear();
    time = 0;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Matching method: Boyer-Moore" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        ans = boyerMoore(pattern, text);
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
    }
    std::cout << "Answer: ";
    if (ans.size() == 0)
        std::cout << "No Match." << std::endl;
    else
    {
        for (size_t i = 0; i < ans.size(); i++)
            std::cout << ans[i] << " ";
        std::cout << std::endl;
    }
    std::cout << "Time cost: " << time << "ms" << std::endl;
}

extern std::vector<int> bruteForceSearch(const std::string &pattern, const std::string &text)
{
    std::vector<int> answer;

    int n = text.length();
    int m = pattern.length();

    // Scan each possible starting position in text
    for (int i = 0; i <= n - m; ++i)
    {
        int j;
        // Compare substring starting at current position with pattern
        for (j = 0; j < m; ++j)
        {
            if (text[i + j] != pattern[j])
                break;
        }
        // If all characters matched
        if (j == m)
            answer.push_back(i);
    }

    return answer;
}

extern std::vector<int> kmpSearch(const std::string &pattern, const std::string &text)
{
    std::vector<int> answer;

    int n = text.length();
    int m = pattern.length();

    if (m == 0)
    {
        answer.push_back(0);
        return answer;
    }

    // Build LPS (Longest Proper Prefix which is also Suffix) array
    std::vector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }

    // KMP matching phase
    i = 0;     // text index
    int j = 0; // pattern index

    while (i < n)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == m)
        {
            answer.push_back(i - j);
            j = lps[j - 1];
        }
        else if (i < n && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
        }
    }

    return answer;
}

extern std::vector<int> boyerMoore(const std::string &pattern, const std::string &text)
{
    std::vector<int> answer;

    int n = text.length();
    int m = pattern.length();

    if (m == 0)
    {
        answer.push_back(0);
        return answer;
    }

    // Preprocess bad character rule
    std::unordered_map<char, int> badChar;
    for (int i = 0; i < m; ++i)
    {
        badChar[pattern[i]] = i;
    }

    // Preprocess good suffix rule
    // Compute suffix array: suffix[i] = longest common suffix length
    // between pattern[0..i] and the whole pattern
    std::vector<int> suffix(m, 0);
    suffix[m - 1] = m;
    int g = m - 1, f;

    for (int i = m - 2; i >= 0; --i)
    {
        if (i > g && suffix[i + m - 1 - f] < i - g)
        {
            suffix[i] = suffix[i + m - 1 - f];
        }
        else
        {
            if (i < g)
            {
                g = i;
            }
            f = i;
            while (g >= 0 && pattern[g] == pattern[g + m - 1 - f])
            {
                --g;
            }
            suffix[i] = f - g;
        }
    }

    // Compute goodSuffix array
    std::vector<int> goodSuffix(m, m);

    // Case 1: find prefix matching the suffix
    int j = 0;
    for (int i = m - 1; i >= 0; --i)
    {
        if (suffix[i] == i + 1)
        {
            for (; j < m - 1 - i; ++j)
            {
                if (goodSuffix[j] == m)
                {
                    goodSuffix[j] = m - 1 - i;
                }
            }
        }
    }

    // Case 2: find substring matching the matched suffix
    for (int i = 0; i <= m - 2; ++i)
    {
        goodSuffix[m - 1 - suffix[i]] = m - 1 - i;
    }

    // Boyer-Moore main loop
    int s = 0;
    while (s <= n - m)
    {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[s + j])
        {
            --j;
        }
        if (j < 0)
        {
            answer.push_back(s);
            s += 1;
        }
        else
        {
            int badCharShift = j - (badChar.count(text[s + j]) ? badChar[text[s + j]] : -1);
            int goodSuffixShift = goodSuffix[j];
            s += std::max(badCharShift, goodSuffixShift);
        }
    }

    return answer;
}