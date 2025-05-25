#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define HEAP_INIT_CAP 100
#define MAXN 300005
#define LOGN 20

int depth[MAXN];
long long dist[MAXN];
int parent[LOGN][MAXN];

int n, m, q;

typedef struct Edge
{
    int dest;
    int weight;
    struct Edge *next;
} Edge;

typedef struct BikeEntry
{
    int student;
    // 記分子跟分母？
    unsigned long long num; // 分子
    unsigned long long den; // 分母
} BikeEntry;

typedef struct ParkingSlot
{
    int capacity;
    int bikeCount;
    BikeEntry bikes[32];
    bool occupied[16];
} ParkingSlot;

typedef struct BikeStatus
{
    int slot;        // 紀錄停車格編號or -1
    int indexInSlot; // 在停車格中的index
    int status;      // 0: 停放中；1: 被拖走了
} BikeStatus;

// 應該用min-heap管理t+l_s
typedef struct Notification
{
    int student;
    unsigned long long time; // t + ℓ_s[s]
} Notification;

typedef struct
{
    Notification *data;
    int size;
    int capacity;
} MinHeap;

int comparerational(unsigned long long a, unsigned long long b, unsigned long long c, unsigned long long d)
{
    __int128_t left = (__int128_t)a * d;
    __int128_t right = (__int128_t)b * c;
    if (left < right)
        return -1;
    else if (left > right)
        return 1;
    else
        return 0;
}

MinHeap *createHeap()
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->data = (Notification *)malloc(HEAP_INIT_CAP * sizeof(Notification));
    heap->size = 0;
    heap->capacity = HEAP_INIT_CAP;
    return heap;
}

void swap(Notification *a, Notification *b)
{
    Notification temp = *a;
    *a = *b;
    *b = temp;
}

void heapPush(MinHeap *heap, Notification temp)
{
    if (heap->size >= heap->capacity)
    {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, sizeof(Notification) * heap->capacity);
    }
    int i = heap->size++;
    heap->data[i] = temp;
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (heap->data[parent].time <= heap->data[i].time)
        {
            break;
        }
        swap(&heap->data[parent], &heap->data[i]);
        i = parent;
    }
}

Notification heapTop(MinHeap *heap)
{
    return heap->data[0];
}

void heapPop(MinHeap *heap)
{
    if (heap->size == 0)
        return;
    heap->data[0] = heap->data[--heap->size];
    int i = 0;
    while (1)
    {
        int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
        if (left < heap->size && heap->data[left].time < heap->data[smallest].time)
            smallest = left;
        if (right < heap->size && heap->data[right].time < heap->data[smallest].time)
            smallest = right;
        if (smallest == i)
            break;
        swap(&heap->data[i], &heap->data[smallest]);
        i = smallest;
    }
}

bool heapEmpty(MinHeap *heap)
{
    return heap->size == 0;
}

void addEdge(Edge **head, int u, int v, int w)
{
    Edge *newEdge = (Edge *)malloc(sizeof(Edge));
    newEdge->dest = v;
    newEdge->weight = w;
    newEdge->next = head[u];
    head[u] = newEdge;
}

ParkingSlot *slots;
BikeStatus *ubike;
int *l_s;
Edge **graph;
MinHeap *S;

unsigned long long gcd(unsigned long long a, unsigned long long b)
{
    while (b)
    {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/*
   park停車👍
   1. 先看p
   2. 看p-1,p+1,p-2,...；p=1, p+1,p+2,...
   3. 整數都滿了，停分數，應該要把整數跟分數都重新排序
*/
void park(int s, int x, int p, int tt)
{
    ParkingSlot *slot = &slots[x];

    if (!slot->occupied[p])
    {
        BikeEntry bike;
        bike.student = s;
        bike.num = p;
        bike.den = 1;
        slot->bikes[slot->bikeCount] = bike;
        slot->bikeCount++;
        slot->occupied[p] = true;
        ubike[s].slot = x;
        ubike[s].indexInSlot = slot->bikeCount - 1;
        ubike[s].status = 0;
        if (tt == 1)
        {
            printf("%d parked at (%d, %llu).\n", s, x, bike.num);
        }
        return;
    }
    else
    {
        bool found = false;
        int bestPos = 0;
        int minDiff = 1 << 30;
        for (int i = 1; i <= slot->capacity; i++)
        {
            if (!slot->occupied[i])
            {
                int diff = abs(i - p);
                if (diff < minDiff || (diff == minDiff && i < bestPos))
                {
                    minDiff = diff;
                    bestPos = i;
                    found = true;
                }
            }
        }
        if (found)
        {
            // 重新停車
            park(s, x, bestPos, tt);
            return;
        }
    }
    // 所有整數位滿了->開始搞分數->考慮整數跟分數運算
    // 搞一個紀錄所有整數跟分數，反正最多30格而已，去你的
    BikeEntry allBike[32];
    int totalcount = slot->bikeCount;
    for (int j = 0; j < totalcount; j++)
    {
        allBike[j] = slot->bikes[j];
    }

    for (int k = 0; k < totalcount; k++)
    {
        for (int l = k; l < totalcount; l++)
        {
            if (comparerational(allBike[k].num, allBike[k].den, allBike[l].num, allBike[l].den) > 0)
            {
                BikeEntry temp = allBike[k];
                allBike[k] = allBike[l];
                allBike[l] = temp;
            }
        }
    }

    unsigned long long p_num = p, p_den = 1;
    BikeEntry neighbor;
    if (comparerational(p_num, p_den, allBike[0].num, allBike[0].den) == 0)
    {
        neighbor = allBike[1];
    }
    else
    {
        neighbor = allBike[0];
        for (int a = 0; a < totalcount; a++)
        {
            if (comparerational(allBike[a].num, allBike[a].den, p_num, p_den) < 0)
            {
                neighbor = allBike[a];
            }
            else
            {
                break;
            }
        }
    }

    unsigned long long new_num = p * neighbor.den + neighbor.num;
    unsigned long long new_den = 2 * neighbor.den;
    unsigned long long g = gcd(new_num, new_den);
    new_num /= g;
    new_den /= g;

    BikeEntry bike;
    bike.student = s;
    bike.num = new_num;
    bike.den = new_den;
    slot->bikes[slot->bikeCount] = bike;
    slot->bikeCount++;
    ubike[s].slot = x;
    ubike[s].indexInSlot = slot->bikeCount - 1;
    ubike[s].status = 0;

    // printf("%d want to parked beside (%d, %llu/%llu).\n", s, x, neighbor.num, neighbor.den);
    // printf("Add new car %d parked at (%d, %llu/%llu).\n", s, x, bike.num, bike.den);
    if (tt == 1)
    {
        printf("%d parked at (%d, %llu/%llu).\n", s, x, new_num, new_den);
    }
}

void dfs(int u, int p)
{
    parent[0][u] = p;
    if (p == -1)
        depth[u] = 0;
    else
        depth[u] = depth[p] + 1;
    for (Edge *e = graph[u]; e != NULL; e = e->next)
    {
        int v = e->dest;
        if (v == p)
        {
            continue;
        }
        dist[v] = dist[u] + e->weight;
        dfs(v, u);
    }
}

void computeParent(int n)
{
    for (int k = 1; k < LOGN; k++)
    {
        for (int i = 0; i < n; i++)
        {
            if (parent[k - 1][i] != -1)
                parent[k][i] = parent[k - 1][parent[k - 1][i]];
            else
                parent[k][i] = -1;
        }
    }
}

int lca(int u, int v)
{
    if (depth[u] < depth[v])
    {
        int temp = u;
        u = v;
        v = temp;
    }
    int d = depth[u] - depth[v];
    for (int k = 0; k < LOGN; k++)
    {
        if (d & (1 << k))
            u = parent[k][u];
    }
    if (u == v)
        return u;
    for (int k = LOGN - 1; k >= 0; k--)
    {
        if (parent[k][u] != parent[k][v])
        {
            u = parent[k][u];
            v = parent[k][v];
        }
    }
    return parent[0][u];
}

long long computeDistance(int u, int v)
{
    int anc = lca(u, v);
    return dist[u] + dist[v] - 2 * dist[anc];
}

/*
   move
   1. 找出原停車格（orig_x）跟index
   2. 若車在整數位置，移除時須更新 occupied 陣列。
   3. 移除後call computeDistance，應該用LCA，先不管rebuild
   4. 最後call park
*/
// int computeDistance(int x, int y)
// {
//     // TODO: 實作樹上兩點距離的計算
//     // 先不算
//     return 0;
// }

void move(int s, int y, int p)
{
    int orig_x = ubike[s].slot;
    if (orig_x == y)
    {
        printf("%d moved to %d in 0 seconds.\n", s, y);
        return;
    }
    int orig_index = ubike[s].indexInSlot;
    ParkingSlot *oldSlot = &slots[orig_x];

    if (oldSlot->bikes[orig_index].den == 1)
    {
        int pos = oldSlot->bikes[orig_index].num;
        oldSlot->occupied[pos] = false;
    }

    oldSlot->bikes[orig_index] = oldSlot->bikes[oldSlot->bikeCount - 1];
    oldSlot->bikeCount--;

    if (orig_index < oldSlot->bikeCount)
    {
        int swappedStudent = oldSlot->bikes[orig_index].student;
        ubike[swappedStudent].indexInSlot = orig_index;
    }

    ubike[s].slot = -1;

    int travelTime = computeDistance(orig_x, y);
    printf("%d moved to %d in %d seconds.\n", s, y, travelTime);

    int ff = 0;

    park(s, y, p, ff);
}

/*
   clear
   1. 移除x所有車，依t+l_s[s]放入S
   2. 重置occupied
*/
void clearSlot(int x, unsigned long long t)
{
    ParkingSlot *slot = &slots[x];
    for (int i = 0; i < slot->bikeCount; i++)
    {
        int s = slot->bikes[i].student;
        Notification nt;
        nt.student = s;
        nt.time = t + l_s[s];
        heapPush(S, nt);
        ubike[s].slot = -1;
    }
    slot->bikeCount = 0;
    for (int i = 1; i <= slot->capacity; i++)
    {
        slot->occupied[i] = false;
    }
}

/*
   rearrange
   1. 清除x中所有非整數停車位置的車 放入S
   2. 更新occupied？
*/
void rearrange(int x, unsigned long long t)
{
    ParkingSlot *slot = &slots[x];
    int keptCount = 0;
    int removed = 0;
    for (int i = 0; i < slot->bikeCount; i++)
    {
        BikeEntry entry = slot->bikes[i];
        if (entry.den != 1)
        {
            Notification nt;
            nt.student = entry.student;
            nt.time = t + l_s[entry.student];
            heapPush(S, nt);
            ubike[entry.student].slot = -1;
            removed++;
        }
        else
        {
            slot->bikes[keptCount++] = entry;
        }
    }
    slot->bikeCount = keptCount;
    for (int i = 1; i <= slot->capacity; i++)
        slot->occupied[i] = false;
    for (int i = 0; i < slot->bikeCount; i++)
    {
        int pos = slot->bikes[i].num;
        slot->occupied[pos] = true;
    }
    printf("Rearranged %d bicycles in %d.\n", removed, x);
}

/*
   fetch
   1. 從S取時間 ≤ t 的車
   2. count
*/
void fetch(unsigned long long t)
{
    int count = 0;
    while (!heapEmpty(S) && heapTop(S).time <= (unsigned long long)t)
    {
        heapPop(S);
        count++;
    }
    printf("At %lld, %d bicycles was fetched.\n", t, count);
}

/*
   rebuild
   1. 更新w (x, y) 權重d更新兩次（雙向）
   2.
*/
void rebuild(int x, int y, int d)
{
    Edge *e = graph[x];
    while (e != NULL)
    {
        if (e->dest == y)
        {
            e->weight = d;
            break;
        }
        e = e->next;
    }
    e = graph[y];
    while (e != NULL)
    {
        if (e->dest == x)
        {
            e->weight = d;
            break;
        }
        e = e->next;
    }
}

int main()
{
    scanf("%d %d %d", &n, &m, &q);

    slots = (ParkingSlot *)malloc(n * sizeof(ParkingSlot));
    for (int i = 0; i < n; i++)
    {
        int c_x;
        scanf("%d", &c_x);
        slots[i].capacity = c_x;
        slots[i].bikeCount = 0;
        for (int j = 1; j <= c_x; j++)
        {
            slots[i].occupied[j] = false;
        }
    }

    l_s = (int *)malloc(sizeof(int) * m);
    for (int j = 0; j < m; j++)
    {
        scanf("%d", &l_s[j]);
    }

    ubike = (BikeStatus *)malloc(m * sizeof(BikeStatus));
    for (int k = 0; k < m; k++)
    {
        ubike[k].slot = -1;
        ubike[k].indexInSlot = -1;
        ubike[k].status = -1;
    }

    graph = (Edge **)malloc(n * sizeof(Edge *));
    for (int k = 0; k < n; k++)
    {
        graph[k] = NULL;
    }
    for (int i = 0; i < n - 1; i++)
    {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        addEdge(graph, u, v, w);
        addEdge(graph, v, u, w);
    }

    S = createHeap();

    dfs(0, -1);
    computeParent(n);

    for (int j = 0; j < q; j++)
    {
        int op;
        scanf("%d", &op);
        if (op == 0)
        {
            int s, x, p;
            scanf("%d %d %d", &s, &x, &p);
            park(s, x, p, 1);
        }
        else if (op == 1)
        {
            int s, y, p;
            scanf("%d %d %d", &s, &y, &p);
            move(s, y, p);
        }
        else if (op == 2)
        {
            int x;
            unsigned long long t;
            scanf("%d %lld", &x, &t);
            clearSlot(x, t);
        }
        else if (op == 3)
        {
            int x;
            unsigned long long t;
            scanf("%d %lld", &x, &t);
            rearrange(x, t);
        }
        else if (op == 4)
        {
            unsigned long long t;
            scanf("%lld", &t);
            fetch(t);
        }
        else if (op == 5)
        {
            // rebuild 操作
            // 不做了
            int x, y, d;
            scanf("%d %d %d", &x, &y, &d);
            rebuild(x, y, d);
        }
    }

    return 0;
}
