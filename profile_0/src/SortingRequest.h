#pragma once
#include <iostream>
// sort
#include <algorithm>
#include <chrono>
#include <memory>
#include <random>
#include <vector>
// memset
#include <cstring>
#include "IRequest.h"

using namespace std;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

class SortingRequest : public IRequest
{
public:
    explicit SortingRequest(int id, logFunc f) : IRequest(id, f)
    {
    }

    void exec() noexcept override
    {
        _logFunc("-->CompileShaderRequest: exec()\n");

        unique_ptr<char[]> s(new char[L]);
        ::memset(s.get(), 'a', L * sizeof(char));
        minstd_rand rgen;
        for (unsigned int i = 0; i < L / 1024; ++i)
        {
            s[rgen() % (L - 1)] = 'a' + (rgen() % ('z' - 'a' + 1));
        }
        s[L - 1] = 0;
        vector<const char *> vs(N);
        for (unsigned int i = 0; i < N; ++i)
        {
            vs[i] = &s[rgen() % (L - 1)];
        }

        system_clock::time_point t1 = system_clock::now();
        size_t count = 0;
        std::sort(vs.begin(), vs.end(), [&](const char *a, const char *b)
                  { ++count; 
                  return compare(a, b, L); });
        system_clock::time_point t2 = system_clock::now();
        cout << "Sort time: " << duration_cast<milliseconds>(t2 - t1).count() << "ms (" << count << " comparisons)" << endl;

        _logFunc("<--CompileShaderRequest: exec()\n");
    }

private:
    bool compare(const char *s1, const char *s2, unsigned int l)
    {
        if (s1 == s2)
            return false;
        for (unsigned int i1 = 0, i2 = 0; i1 < l; ++i1, ++i2)
        {
            if (s1[i1] != s2[i2])
                return s1[i1] > s2[i2];
        }
        return false;
    }
    static constexpr unsigned int L = 1 << 18, N = 1 << 14;
};