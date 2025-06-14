#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAXT 1000005

typedef long long ll;

// 每個群組只存 3 個值：
// init：插入時的原始 value
// cnt ：這個群組裡有幾顆
// op3：插入時已做過的 Type3 次數
typedef struct 
{
    ll init;
    ll cnt;
    ll op3;
    ll rank;
} Group;


Group *st;
int top = 0;            
ll total_cnt = 0;       // S 裡目前有幾顆 diamond
ll accu3 = 0;           // 做過幾次 Type3
ll M;                   // 從輸入讀入的常數 M



// Type1：刪除所有 < v，然後插入 Ni 個 v
void op1(ll Ni, ll v)
{
    ll removed = 0;

    // 1) 整群 pop 掉所有「最低那群最小值 < v」的群組
    while (top > 0) 
    {
        Group *g = &st[top - 1];
        ll delta = accu3 - g->op3;
        // 最小那顆的值
        ll max_val = g->init + delta * (M - g->rank + 1);
        if (max_val < v) 
        {
            removed += g->cnt;
            total_cnt -= g->cnt;
            top--;
        }
        else break;  
    }

    // 2) 
    if (top > 0) 
    {
        Group *g = &st[top - 1];
        ll delta = accu3 - g->op3;
        ll max_val = g->init + delta * (M - g->rank + 1);
        ll min_val = g->init + delta * (M - (g->rank + g->cnt - 1) + 1);
        if (min_val < v && max_val >= v) 
        {
            // 解 inequality: init_v + delta*(M - j + 1) >= v => j <= M - ceil((v - init_v)/delta) + 1
            ll k = (v - g->init + delta - 1) / delta;     
            ll keep_upto = M - k + 1;                       
            ll num_keep = keep_upto - g->rank + 1;   
            ll num_rem = g->cnt - num_keep;               
            removed += num_rem;
            total_cnt -= num_rem;
            g->cnt = num_keep;
        }
    }

    if (top > 0 && st[top-1].init == v && st[top-1].op3 == accu3) 
    {
        st[top-1].cnt += Ni;
        total_cnt += Ni;
    }
    else 
    {
        st[top].init = v;
        st[top].cnt = Ni;
        st[top].op3 = accu3;
        st[top].rank = total_cnt + 1;
        top++;
        total_cnt += Ni;
    }
    printf("%lld\n", removed);
}

// Type2：query
void op2(ll p)
{
    int left = 0, right = top - 1;
    // int pos = -1;  
    while (left <= right) 
    {
        int mid = (left + right) >> 1;
        Group *g = &st[mid];
        ll delta   = accu3 - g->op3;
        ll max_val = g->init + delta * (M - g->rank + 1);
        if (max_val >= p) 
        {
            // pos = mid;
            left = mid + 1;
        } 
        else right = mid - 1;
    }
    int limit = left;  // [0..limit-1] 的群組都要檢查
    ll ans = 0;
    for (int i = 0; i < limit; i++) 
    {
        Group *g = &st[i];
        ll delta   = accu3 - g->op3;
        ll max_val = g->init + delta * (M - g->rank + 1);
        ll min_val = g->init + delta * (M - (g->rank + g->cnt - 1) + 1);
        if (delta == 0) 
        {
            if (g->init == p) ans += g->cnt;
        } 
        else 
        {
            if (p <= max_val && p >= min_val && (max_val - p) % delta == 0)
                ans += 1;
        }
    }
    printf("%lld\n", ans);
    
}


void op3()
{
    accu3++;
}

int main()
{
    int T;
    scanf("%d %lld", &T, &M);
    st = malloc(sizeof(Group) * (T + 5));
    for(int i = 0; i < T; i++)
    {
        int op;
        scanf("%d", &op);
        if(op == 1)
        {
            ll Ni, v;
            scanf("%lld %lld", &Ni, &v);
            op1(Ni, v);
        } 
        else if(op == 2)
        {
            ll p;
            scanf("%lld", &p);
            op2(p);
        } 
        else op3();
    }
    return 0;
}
