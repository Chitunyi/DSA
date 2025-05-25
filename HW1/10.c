#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef long long ll;

// treap -> priority *rand()*

typedef struct Node
{
    ll key;
    int count;
    int size;
    int priority;
    struct Node *left;
    struct Node *right;
} Node;

int getsize(Node *node)
{
    return node ? node->size : 0;
}

void updatesize(Node *node)
{
    if (node)
    {
        node->size = getsize(node->left) + getsize(node->right) + node->count;
    }
}

Node *newnode(ll key, int count)
{
    Node *node = malloc(sizeof(Node));
    node->key = key;
    node->count = count;
    node->size = count;
    node->priority = rand();
    node->left = NULL;
    node->right = NULL;
    return node;
}

void split(Node *root, ll key, Node **left, Node **right)
{
    if (root == NULL)
    {
        *left = NULL;
        *right = NULL;
        return;
    }
    if (root->key < key)
    {
        split(root->right, key, &root->right, right);
        *left = root;
    }
    else
    {
        split(root->left, key, left, &root->left);
        *right = root;
    }
    updatesize(root);
}

Node *merge(Node *left, Node *right)
{
    if (!left || !right)
        return left ? left : right;

    if (left->priority > right->priority)
    {
        left->right = merge(left->right, right);
        updatesize(left);
        return left;
    }
    else
    {
        right->left = merge(left, right->left);
        updatesize(right);
        return right;
    }
}

Node *insert(Node *root, ll key, int count)
{
    if (root == NULL)
    {
        return newnode(key, count);
    }

    if (key == root->key)
    {
        root->count += count;
        updatesize(root);
        return root;
    }
    else if (key < root->key)
    {
        root->left = insert(root->left, key, count);

        if (root->left->priority > root->priority)
        {
            Node *L = root->left;
            root->left = L->right;
            L->right = root;
            updatesize(root);
            updatesize(L);
            return L;
        }
    }
    else
    {
        root->right = insert(root->right, key, count);

        if (root->right->priority > root->priority)
        {
            Node *R = root->right;
            root->right = R->left;
            R->left = root;
            updatesize(root);
            updatesize(R);
            return R;
        }
    }

    updatesize(root);
    return root;
}

int query(Node *root, ll key)
{
    if (root == NULL)
    {
        return 0;
    }
    if (key == root->key)
    {
        return root->count;
    }
    else if (key < root->key)
    {
        return query(root->left, key);
    }
    else
    {
        return query(root->right, key);
    }
}

void freeall(Node *root)
{
    if (root == NULL)
    {
        return;
    }

    int capacity = 1024;
    int top = 0;
    Node **stack = malloc(capacity * sizeof(Node *));

    stack[top++] = root;

    while (top > 0)
    {
        Node *node = stack[--top];

        if (node->left)
        {
            if (top == capacity)
            {
                capacity *= 2;
                stack = realloc(stack, capacity * sizeof(Node *));
            }
            stack[top++] = node->left;
        }
        if (node->right)
        {
            if (top == capacity)
            {
                capacity *= 2;
                stack = realloc(stack, capacity * sizeof(Node *));
            }
            stack[top++] = node->right;
        }
        free(node);
    }
    free(stack);
}

void traverse(Node *root, ll *arr, int *index)
{
    int stackCapacity = 128;
    int stackSize = 0;
    Node **stack = malloc(stackCapacity * sizeof(Node *));
    Node *curr = root;
    while (curr != NULL || stackSize > 0)
    {
        while (curr != NULL)
        {
            if (stackSize == stackCapacity)
            {
                stackCapacity *= 2;
                stack = realloc(stack, stackCapacity * sizeof(Node *));
            }
            stack[stackSize++] = curr;
            curr = curr->right;
        }
        curr = stack[--stackSize];
        // 將此節點的 key 重複 count 次存入陣列
        for (int i = 0; i < curr->count; i++)
        {
            arr[(*index)++] = curr->key;
        }
        curr = curr->left;
    }
    free(stack);
}

void recalc(Node *root)
{
    if (!root)
    {
        return;
    }
    int capacity = 128;
    int stackSize = 0;
    Node **stack = malloc(capacity * sizeof(Node *));
    int *visited = malloc(capacity * sizeof(int));
    Node *curr = root;
    while (curr || stackSize > 0)
    {
        if (curr)
        {
            if (stackSize == capacity)
            {
                capacity *= 2;
                stack = realloc(stack, capacity * sizeof(Node *));
                visited = realloc(visited, capacity * sizeof(int));
            }
            stack[stackSize] = curr;
            visited[stackSize] = 0;
            stackSize++;
            curr = curr->left;
        }
        else
        {
            curr = stack[stackSize - 1];
            if (visited[stackSize - 1] == 0)
            {
                visited[stackSize - 1] = 1;
                curr = curr->right;
            }
            else
            {
                updatesize(curr);
                stackSize--;
                curr = NULL;
            }
        }
    }
    free(stack);
    free(visited);
}

Node *buildtreap(ll *arr, int n)
{
    Node **stack = malloc(n * sizeof(Node *));
    Node *root = NULL;

    int top = -1;
    for (int i = 0; i < n; i++)
    {
        Node *curr = newnode(arr[i], 1);
        while (top >= 0 && stack[top]->priority < curr->priority)
        {
            curr->left = stack[top];
            top--;
        }
        if (top >= 0)
        {
            stack[top]->right = curr;
        }
        stack[++top] = curr;
    }
    root = stack[0];
    free(stack);

    recalc(root);
    return root;

    for (int i = 0; i < n; i++)
    {
        Node *curr = newnode(arr[i], 1);
        while (top >= 0 && stack[top]->priority < curr->priority)
        {
            curr->left = stack[top];
            top--;
        }
        if (top >= 0)
        {
            stack[top]->right = curr;
        }
        stack[++top] = curr;
    }

    root = stack[0];
    free(stack);
    recalc(root);

    return root;
}

int main()
{
    int T;
    ll M;
    scanf("%d %lld", &T, &M);
    Node *treap = NULL;

    srand((unsigned)time(NULL));

    for (int i = 0; i < T; i++)
    {
        int n;
        scanf("%d", &n);
        if (n == 1)
        {
            int N;
            ll v;
            scanf("%d %lld", &N, &v);
            Node *left = NULL;
            Node *right = NULL;
            split(treap, v, &left, &right);

            int removed = getsize(left);
            freeall(left);

            treap = right;
            printf("%d\n", removed);
            treap = insert(treap, v, N);
        }
        else if (n == 2)
        {
            ll p;
            scanf("%lld", &p);
            int cnt = query(treap, p);
            printf("%d\n", cnt);
        }
        else if (n == 3)
        {
            int num = getsize(treap);
            if (num == 0)
            {
                continue;
            }

            ll *arr = malloc(num * sizeof(ll));
            int index = 0;
            traverse(treap, arr, &index);

            for (int j = 0; j < num; j++)
            {
                arr[j] = arr[j] + M - j;
            }

            for (int k = 0; k < num / 2; k++)
            {
                ll tmp = arr[k];
                arr[k] = arr[num - k - 1];
                arr[num - k - 1] = tmp;
            }

            freeall(treap);
            treap = buildtreap(arr, num);
            free(arr);
        }
    }

    freeall(treap);
    return 0;
}
