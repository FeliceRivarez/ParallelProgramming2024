//g++ ./openMPRowSIMD.cpp -fopenmp -o ./openMPRowSIMD.o -march=native
#include <iostream>
#include <fstream>
#include <chrono>
#include <omp.h>

#include <arm_neon.h>
using namespace std;
using namespace std::chrono;
const int NUM_THREADS=2;

int main()
{
    ofstream result("result.txt");
    fstream testset("testset.txt");
    string a = "114514";
    string b = "1919810";
    int test = 10;
    long long total = 0;
    while (testset >> a)
    {
        testset >> b;
        // a="114514";
        // b="1919810";
        int distance = 0;
        for (int k = 0; k < test; k += 1)
        {
            int m = a.length();
            int n = b.length();
            int **dp = new int *[m + 1];
            for (int i = 0; i < m + 1; i += 1)
            {
                dp[i] = new int[n + 1];
            }
            for (int i = 0; i < m + 1; i += 1)
            {
                dp[i][0] = i;
            }
            for (int i = 0; i < n + 1; i += 1)
            {
                dp[0][i] = i;
            }
            auto start = system_clock::now();
            int **mt = new int *[m + 1];
            for (int i = 0; i < m + 1; i += 1)
            {
                mt[i] = new int[n + 1];
            }
            for (int i = 0; i < m + 1; i += 1)
            {
                mt[i][0] = 0;
            }
            for (int i = 0; i < n + 1; i += 1)
            {
                mt[0][i] = 0;
            }
            int i=0;
            int i1=1;
            #pragma omp parallel num_threads(NUM_THREADS), private(i,i1), default(shared)
            #pragma omp for
            for (int i = 1; i < m + 1; i += 1)
            {
                int nearest = 0;
                for (int i1 = 1; i1 < n + 1; i1 += 1)
                {
                    if (a[i - 1] == b[i1 - 1])
                    {
                        nearest = i1;
                    }
                    mt[i][i1] = nearest;
                }
            }
            for (int i = 1; i < m + 1; i += 1)
            {
                int i1;
                int upper = n + 1 - (n % 4); // here we should use n%4 instead of (n+1)%4, because we started from i1=1 instead of i1=0.
                #pragma omp parallel num_threads(NUM_THREADS), private(i1), default(shared)
                #pragma omp for
                for (int i1 = 1; i1 < upper; i1 += 4)
                {
                    if (mt[i][i1] != 0)
                    {
                        if (a[i - 1] != b[i1 - 1] && a[i - 1] != b[i1] && a[i - 1] != b[i1 + 1] && a[i - 1] != b[i1 + 2])
                        {
                            /*int32x4_t vecA = vld1q_s32(dp[i - 1] + i1);
                            int32x4_t vecB = vld1q_s32(dp[i - 1] + i1 - 1);
                            int32x4_t r = vminq_s32(vecA, vecB);
                            int32x4_t vecC = vmovq_n_s32(dp[i - 1][mt[i][i1]] + i1 - mt[i][i1] - 1);
                            vecC = vsetq_lane_s32(dp[i - 1][mt[i][i1 + 1]] + i1 - mt[i][i1 + 1], vecC, 1);
                            vecC = vsetq_lane_s32(dp[i - 1][mt[i][i1 + 2]] + i1 - mt[i][i1 + 2] + 1, vecC, 2);
                            vecC = vsetq_lane_s32(dp[i - 1][mt[i][i1 + 3]] + i1 - mt[i][i1 + 3] + 2, vecC, 3);
                            r = vminq_s32(r, vecC);
                            vst1q_s32(dp[i] + i1, r);*/

                            /*__m128i t1, t2, t3;
                            t1 = _mm_set_epi32(dp[i - 1][i1 + 2] + 1, dp[i - 1][i1 + 1] + 1, dp[i - 1][i1] + 1, dp[i - 1][i1 - 1] + 1);
                            t2 = _mm_set_epi32(dp[i - 1][i1 + 3] + 1, dp[i - 1][i1 + 2] + 1, dp[i - 1][i1 + 1] + 1, dp[i - 1][i1] + 1);
                            t1 = _mm_min_epi16(t1, t2);
                            t3 = _mm_set_epi32(dp[i - 1][mt[i][i1 + 3] - 1] + i1 - mt[i][i1 + 3] + 2 + 1, dp[i - 1][mt[i][i1 + 2] - 1] + i1 - mt[i][i1 + 2] + 1 + 1, dp[i - 1][mt[i][i1 + 1] - 1] + i1 - mt[i][i1 + 1] + 1, dp[i - 1][mt[i][i1] - 1] + i1 - mt[i][i1] - 1 + 1);
                            t1 = _mm_min_epi16(t1, t3);
                            _mm_storeu_si128((__m128i *)(dp[i] + i1), t1);*/
                            int32x4_t t1, t2;
                            // t1 = vsetq_lane_s32(dp[i - 1][i1 + 2] + 1, t1, 3);
                            // t1 = vsetq_lane_s32(dp[i - 1][i1 + 1] + 1, t1, 2);
                            // t1 = vsetq_lane_s32(dp[i - 1][i1] + 1, t1, 1);
                            // t1 = vsetq_lane_s32(dp[i - 1][i1 - 1] + 1, t1, 0);

                            // t2 = vsetq_lane_s32(dp[i - 1][i1 + 3] + 1, t2, 3);
                            // t2 = vsetq_lane_s32(dp[i - 1][i1 + 2] + 1, t2, 2);
                            // t2 = vsetq_lane_s32(dp[i - 1][i1 + 1] + 1, t2, 1);
                            // t2 = vsetq_lane_s32(dp[i - 1][i1] + 1, t2, 0);
                            t1 = vld1q_s32(dp[i - 1] + i1);
                            t2 = vld1q_s32(dp[i - 1] + i1 - 1);
                            t1 = vminq_s32(t1, t2);

                            int32x4_t t3={dp[i - 1][mt[i][i1] - 1] + i1 - mt[i][i1] - 1,dp[i - 1][mt[i][i1 + 1] - 1] + i1 - mt[i][i1 + 1], dp[i - 1][mt[i][i1 + 2] - 1] + i1 - mt[i][i1 + 2] + 1, dp[i - 1][mt[i][i1 + 3] - 1] + i1 - mt[i][i1 + 3] + 2};
                            // t3 = vsetq_lane_s32(dp[i - 1][mt[i][i1 + 3] - 1] + i1 - mt[i][i1 + 3] + 2, t3, 3);
                            // t3 = vsetq_lane_s32(dp[i - 1][mt[i][i1 + 2] - 1] + i1 - mt[i][i1 + 2] + 1, t3, 2);
                            // t3 = vsetq_lane_s32(dp[i - 1][mt[i][i1 + 1] - 1] + i1 - mt[i][i1 + 1], t3, 1);
                            // t3 = vsetq_lane_s32(dp[i - 1][mt[i][i1] - 1] + i1 - mt[i][i1] - 1, t3, 0);

                            t1 = vminq_s32(t1, t3);
                            int32x4_t sum4=vmovq_n_s32(1);
                            t1=vaddq_s32(t1,sum4);
                            vst1q_s32(dp[i] + i1, t1);
                        }
                        else
                        {
                            int count = 0;
                            while (count < 4)
                            {
                                if (a[i - 1] == b[i1 - 1 + count])
                                {
                                    dp[i][i1 + count] = dp[i - 1][i1 - 1 + count];
                                }
                                else
                                {
                                    dp[i][i1 + count] = min(min(dp[i - 1][i1 + count], dp[i - 1][i1 - 1 + count]), dp[i - 1][mt[i][i1 + count] - 1] + i1 + count - mt[i][i1 + count] - 1) + 1;
                                }
                                count += 1;
                            }
                        }
                    }
                    else
                    {
                        int count = 0;
                        while (count < 4)
                        {
                            if (a[i - 1] == b[i1 - 1 + count])
                            {
                                dp[i][i1 + count] = dp[i - 1][i1 - 1 + count];
                            }
                            else
                            {
                                dp[i][i1 + count] = min(min(dp[i - 1][i1 + count], dp[i - 1][i1 - 1 + count]), dp[i - 1][mt[i][i1 + count]] + i1 + count - mt[i][i1 + count] - 1) + 1;
                            }
                            count += 1;
                        }
                    }

                    // cout<<dp[i][i1]<<" "<<dp[i][i1+1]<<" "<<dp[i][i1+2]<<" "<<dp[i][i1+3]<<" ";
                }
                for (int i1 = upper; i1 < n + 1; i1 += 1)
                {
                    if (a[i - 1] == b[i1 - 1])
                    {
                        dp[i][i1] = dp[i - 1][i1 - 1];
                    }
                    else
                    {
                        dp[i][i1] = min(min(dp[i - 1][i1], dp[i - 1][i1 - 1]), dp[i - 1][mt[i][i1] - 1] + i1 - mt[i][i1] - 1) + 1;
                    }
                    // cout<<dp[i][i1]<<" ";
                }
                // cout<<endl;
            }
            auto duration = duration_cast<microseconds>(system_clock::now() - start);
            cout << duration.count() << endl;
            total += duration.count();
            cout << dp[m][n] << endl;
            distance = dp[m][n];
            for (int i = 0; i < m + 1; i += 1)
            {
                delete dp[i];
            }
            for (int i = 0; i < m + 1; i += 1)
            {
                delete mt[i];
            }
        }
        result << distance << " " << total / 10 << endl;
        total = 0;
    }
    // system("pause");
}
