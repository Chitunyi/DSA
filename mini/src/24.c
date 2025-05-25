// mini_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define d 52

int k, n, m;
unsigned long long q;

void RabinKarp(unsigned long long *H,
               const char *M,
               int k,
               int len,
               const unsigned long long *powD,
               unsigned long long q)
{
    for (int j = 0; j < len; j++)
    {
        unsigned long long sum = 0;
        for (int i = 0; i < k; i++)
        {
            char c = M[i * len + j];
            int val = (c >= 'A' && c <= 'Z') ? (c - 'A') : (c - 'a' + 26);
            sum = (sum + val * powD[k - 1 - i]) % q;
        }
        H[j] = sum;
    }
}

void buildpi(unsigned long long *Ppp, int m, int *pi)
{
    pi[0] = 0;
    int j = 0;
    for (int i = 1; i < m; i++)
    {
        while (j > 0 && Ppp[i] != Ppp[j])
        {
            j = pi[j - 1];
        }
        if (Ppp[i] == Ppp[j])
        {
            j++;
        }
        pi[i] = j;
    }
}

void kmp(unsigned long long *Tpp, unsigned long long *Ppp, int n, int m, int *pi, int *matches, int *matches_cnt)
{
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        while (j > 0 && Tpp[i] != Ppp[j])
        {
            j = pi[j - 1];
        }
        if (Tpp[i] == Ppp[j])
        {
            j++;
        }
        if (j == m)
        {
            int s = i - m + 1;
            matches[(*matches_cnt)++] = s;
            j = pi[j - 1];
        }
    }
}

bool checkspurious(char *T, char *P, int k, int m, int n, int s)
{
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (T[i * n + (s + j)] != P[i * m + j])
                return true;
        }
    }
    return false;
}

int main()
{
    scanf("%d %d %d %llu", &k, &n, &m, &q);

    char *T = malloc((size_t)k * n * sizeof(char));
    char *P = malloc((size_t)k * m * sizeof(char));

    for (int i = 0; i < k; i++)
    {
        char *buf = malloc((size_t)n + 1);
        scanf("%s", buf);
        for (int j = 0; j < n; j++)
        {
            T[i * n + j] = buf[j];
        }
        free(buf);
    }

    for (int i = 0; i < k; i++)
    {
        char *buf = malloc((size_t)m + 1);
        scanf("%s", buf);
        for (int j = 0; j < m; j++)
        {
            P[i * m + j] = buf[j];
        }
        free(buf);
    }

    unsigned long long *powD = malloc((size_t)k * sizeof(unsigned long long));
    powD[0] = 1;
    for (int i = 1; i < k; i++)
    {
        powD[i] = (powD[i - 1] * d) % q;
    }

    // for (int i = 0; i < k; i++)
    // {
    //     printf("powD[%d] = %llu\n", i, powD[i]);
    // }

    unsigned long long *Tpp = malloc((size_t)n * sizeof(unsigned long long));
    unsigned long long *Ppp = malloc((size_t)m * sizeof(unsigned long long));

    RabinKarp(Tpp, T, k, n, powD, q);
    RabinKarp(Ppp, P, k, m, powD, q);

    for (int j = 0; j < n; j++)
    {
        printf("%llu%c", Tpp[j], (j + 1 == n) ? '\n' : ' ');
    }

    for (int j = 0; j < m; j++)
    {
        printf("%llu%c", Ppp[j], (j + 1 == m) ? '\n' : ' ');
    }

    int *pi = malloc((size_t)m * sizeof(int));
    buildpi(Ppp, m, pi);

    int *matches = malloc(n * sizeof *matches);
    int *spur_matches = malloc(n * sizeof *spur_matches);
    int matches_cnt = 0, spur_cnt = 0;

    kmp(Tpp, Ppp, n, m, pi, matches, &matches_cnt);

    for (int i = 0; i < matches_cnt; i++)
    {
        int s = matches[i];
        if (checkspurious(T, P, k, m, n, s))
        {
            spur_matches[spur_cnt++] = s;
        }
    }

    if (matches_cnt == 0)
    {
        printf("-1\n");
    }
    else
    {
        for (int i = 0; i < matches_cnt; i++)
        {
            printf("%d%c", matches[i], (i + 1 == matches_cnt) ? '\n' : ' ');
        }
    }

    if (spur_cnt == 0)
    {
        printf("-1\n");
    }
    else
    {
        for (int i = 0; i < spur_cnt; i++)
        {
            printf("%d%c", spur_matches[i], (i + 1 == spur_cnt) ? '\n' : ' ');
        }
    }

    free(T);
    free(P);
    free(powD);
    free(Tpp);
    free(Ppp);
    free(pi);
    free(matches);
    free(spur_matches);

    return 0;
}