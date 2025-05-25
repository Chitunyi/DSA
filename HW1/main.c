#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef long long ll;

typedef struct
{
    ll base;
    ll diff;
    ll cnt;
    ll op3_stamp;
    ll rank;
} Node;

Node *nodeList = NULL;
int nodeCount = 0;
int capacity = 0;

ll global_op3 = 0;
ll totalCount = 0;
ll M;

void ensureCapacity(int n)
{
    if (n > capacity)
    {
        capacity = n * 2;
        nodeList = realloc(nodeList, capacity * sizeof(Node));
        if (nodeList == NULL)
        {
            fprintf(stderr, "Memory allocation error\n");
            exit(1);
        }
    }
}

// void updateNode(int idx)
// {
//     ll delta = global_op3 - nodeList[idx].op3_stamp;
//     if (delta > 0)
//     {
//         nodeList[idx].base += delta * (M - (nodeList[idx].rank - 1));
//         nodeList[idx].diff -= delta;
//         nodeList[idx].op3_stamp = global_op3;
//     }
// }

void updateNode(int idx)
{
    ll delta = global_op3 - nodeList[idx].op3_stamp;
    if (delta > 0)
    {
        nodeList[idx].base += delta * (M - (nodeList[idx].rank - 1)) - (delta * (delta - 1)) / 2;
        nodeList[idx].diff -= delta;
        nodeList[idx].op3_stamp = global_op3;
    }
}

// void op1(ll Ni, ll v)
// {
//     ll removed = 0;

//     while (nodeCount > 0)
//     {
//         int idx = nodeCount - 1;
//         updateNode(idx);

//         ll eff_max = nodeList[idx].base;
//         ll eff_min = nodeList[idx].base + (nodeList[idx].cnt - 1) * nodeList[idx].diff;
//         if (eff_max < v)
//         {
//             removed += nodeList[idx].cnt;
//             totalCount -= nodeList[idx].cnt;
//             nodeCount--;
//         }
//         else if (eff_min >= v)
//         {
//             break;
//         }
//         else
//         {
//             ll k = ((nodeList[idx].base - v) / (-nodeList[idx].diff)) + 1;
//             if (k > nodeList[idx].cnt)
//                 k = nodeList[idx].cnt;
//             ll removed_here = nodeList[idx].cnt - k;
//             removed += removed_here;
//             totalCount -= removed_here;
//             nodeList[idx].cnt = k;
//             break;
//         }
//     }
//     printf("%lld\n", removed);

//     if (nodeCount > 0)
//     {
//         int idx = nodeCount - 1;
//         updateNode(idx);
//         ll eff_min = nodeList[idx].base + (nodeList[idx].cnt - 1) * nodeList[idx].diff;

//         if (nodeList[idx].diff == 0 && nodeList[idx].base == v)
//         {
//             nodeList[idx].cnt += Ni;
//             totalCount += Ni;
//             return;
//         }
//         else if (nodeList[idx].diff != 0 && nodeList[idx].cnt == 1 && nodeList[idx].base == v)
//         {
//             nodeList[idx].diff = 0;
//             nodeList[idx].cnt += Ni;
//             totalCount += Ni;
//             return;
//         }
//         else if (nodeList[idx].diff != 0 && eff_min == v)
//         {
//             nodeList[idx].cnt--;
//             totalCount--;

//             if (nodeCount > 0 && nodeList[nodeCount - 1].diff == 0 && nodeList[nodeCount - 1].base == v)
//             {
//                 // nodeList[nodeCount - 1].cnt += (1 + Ni);
//                 // totalCount += (1 + Ni);
//                 // return;
//                 nodeList[idx].diff = 0;
//                 nodeList[idx].cnt += Ni;
//                 totalCount += Ni;
//                 return;
//             }
//             else
//             {
//                 ensureCapacity(nodeCount + 1);
//                 nodeList[nodeCount].base = v;
//                 nodeList[nodeCount].diff = 0;
//                 nodeList[nodeCount].cnt = 1 + Ni;
//                 nodeList[nodeCount].op3_stamp = global_op3;
//                 nodeList[nodeCount].rank = totalCount + 1;
//                 nodeCount++;
//                 totalCount += (1 + Ni);
//                 return;
//             }
//         }
//     }

//     ensureCapacity(nodeCount + 1);
//     nodeList[nodeCount].base = v;
//     nodeList[nodeCount].diff = 0;
//     nodeList[nodeCount].cnt = Ni;
//     nodeList[nodeCount].op3_stamp = global_op3;
//     nodeList[nodeCount].rank = totalCount + 1;
//     nodeCount++;
//     totalCount += Ni;
// }

void op1(ll Ni, ll v)
{
    ll removed = 0;

    // 先從 S 的尾端移除所有有效值小於 v 的 diamond
    while (nodeCount > 0)
    {
        int idx = nodeCount - 1;
        updateNode(idx); // lazy 更新：會依 global_op3、rank 等調整 base 與 diff

        ll eff_max = nodeList[idx].base;                                                // 該區塊第一顆 diamond 的有效值
        ll eff_min = nodeList[idx].base + (nodeList[idx].cnt - 1) * nodeList[idx].diff; // 尾端 diamond 的有效值

        if (eff_max < v)
        {
            // 整個區塊全部小於 v，全部移除
            removed += nodeList[idx].cnt;
            totalCount -= nodeList[idx].cnt;
            nodeCount--;
        }
        else if (eff_min >= v)
        {
            // 整個區塊的所有 diamond 都大於等於 v，不用移除
            break;
        }
        else
        {
            // 部分 diamond 小於 v：找出區塊中第一個 diamond 其有效值小於 v 的位置，移除其後所有的 diamond
            ll k = ((nodeList[idx].base - v) / (-nodeList[idx].diff)) + 1;
            if (k > nodeList[idx].cnt)
                k = nodeList[idx].cnt;
            ll removed_here = nodeList[idx].cnt - k;
            removed += removed_here;
            totalCount -= removed_here;
            nodeList[idx].cnt = k;
            break;
        }
    }
    printf("%lld\n", removed);

    // 接下來處理插入 Ni 顆有效值為 v 的 diamond
    // 檢查 S 是否有尾區塊，且尾區塊「續接」的預期值是否正好等於 v
    if (nodeCount > 0)
    {
        int idx = nodeCount - 1;
        updateNode(idx);
        // 預期若續接一顆 diamond，其有效值為：
        ll next_expected = nodeList[idx].base + nodeList[idx].cnt * nodeList[idx].diff;
        if (next_expected == v)
        {
            nodeList[idx].cnt += Ni;
            totalCount += Ni;
            return;
        }
    }
    // 若無法合併，則建立一個新區塊，令其初始為常數（diff = 0）
    ensureCapacity(nodeCount + 1);
    nodeList[nodeCount].base = v;
    nodeList[nodeCount].diff = 0;
    nodeList[nodeCount].cnt = Ni;
    nodeList[nodeCount].op3_stamp = global_op3;
    nodeList[nodeCount].rank = totalCount + 1;
    nodeCount++;
    totalCount += Ni;
}

void op2(ll p)
{
    int lo = 0, hi = nodeCount - 1, found = -1;
    while (lo <= hi)
    {
        int mid = (lo + hi) / 2;
        updateNode(mid);
        ll eff_max = nodeList[mid].base;
        ll eff_min = nodeList[mid].base + (nodeList[mid].cnt - 1) * nodeList[mid].diff;
        if (p > eff_max)
        {
            hi = mid - 1;
        }
        else if (p < eff_min)
        {
            lo = mid + 1;
        }
        else
        {
            found = mid;
            break;
        }
    }
    ll ans = 0;
    if (found != -1)
    {
        if (nodeList[found].diff == 0)
        {
            if (nodeList[found].base == p)
                ans = nodeList[found].cnt;
        }
        else
        {
            ll diffAbs = -nodeList[found].diff;
            if ((nodeList[found].base - p) % diffAbs == 0)
            {
                ll pos = (nodeList[found].base - p) / diffAbs;
                if (pos < nodeList[found].cnt)
                    ans = 1;
            }
        }
    }
    printf("%lld\n", ans);
}

void op3()
{
    global_op3++;
}

int main()
{
    // clock_t start = clock();
    int T;
    if (scanf("%d %lld", &T, &M) != 2)
    {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    capacity = T + 10;
    nodeList = malloc(capacity * sizeof(Node));
    if (nodeList == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }
    nodeCount = 0;
    totalCount = 0;
    global_op3 = 0;

    for (int i = 0; i < T; i++)
    {
        int op;
        if (scanf("%d", &op) != 1)
        {
            fprintf(stderr, "Input error at op %d\n", i);
            return 1;
        }
        if (op == 1)
        {
            ll Ni, v;
            if (scanf("%lld %lld", &Ni, &v) != 2)
            {
                fprintf(stderr, "Input error at op1\n");
                return 1;
            }
            op1(Ni, v);
        }
        else if (op == 2)
        {
            ll p;
            if (scanf("%lld", &p) != 1)
            {
                fprintf(stderr, "Input error at op2\n");
                return 1;
            }
            op2(p);
        }
        else if (op == 3)
        {
            op3();
        }
    }

    free(nodeList);

    // clock_t end = clock();
    // double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    // printf("執行時間: %.3f 秒\n", elapsed_time);

    return 0;
}
