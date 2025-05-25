// mini_e

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_SIZE 8200

typedef struct
{
    long long data[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack *s)
{
    s->top = -1;
}

int isEmpty(Stack *s)
{
    return (s->top == -1);
}

int isFull(Stack *s)
{
    return (s->top == MAX_SIZE - 1);
}

void push(Stack *s, long long val)
{
    if (isFull(s))
    {
        return;
    }
    s->data[++(s->top)] = val;
}

long long pop(Stack *s)
{
    if (isEmpty(s))
    {
        return 0;
    }
    return s->data[(s->top)--];
}

long long peek(Stack *s)
{
    if (isEmpty(s))
    {
        return 0;
    }
    return s->data[s->top];
}

int precedence(char op)
{
    switch (op)
    {
    case '*':
    case '/':
    case '%':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return 0;
    }
}

void infixtopostfix(char *infix, char *postfix)
{
    Stack s;
    initStack(&s);
    int i = 0, j = 0;

    while (infix[i] != '\0')
    {
        if (isdigit(infix[i]))
        {
            while (isdigit(infix[i]))
            {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
            continue;
        }
        if (isspace(infix[i]))
        {
            i++;
            continue;
        }

        char c = infix[i];
        if (c == '(')
        {
            push(&s, c);
        }
        else if (c == ')')
        {
            while (!isEmpty(&s) && peek(&s) != '(')
            {
                postfix[j++] = pop(&s);
                postfix[j++] = ' ';
            }
            if (!isEmpty(&s) && peek(&s) == '(')
            {
                pop(&s);
            }
        }
        else
        {
            while (!isEmpty(&s) && precedence(c) <= precedence(peek(&s)))
            {
                postfix[j++] = pop(&s);
                postfix[j++] = ' ';
            }
            push(&s, c);
        }
        i++;
    }

    while (!isEmpty(&s))
    {
        postfix[j++] = pop(&s);
        postfix[j++] = ' ';
    }
    postfix[j] = '\0';
}

long long postfixeval(char *postfix)
{
    Stack s;
    initStack(&s);
    // int i = 0;

    char *token = strtok(postfix, " ");
    while (token != NULL)
    {
        if (isdigit(token[0]))
        {
            long long val = atoll(token);
            push(&s, val);
        }
        else
        {
            long long b = pop(&s);
            long long a = pop(&s);
            long long result = 0;
            switch (token[0])
            {
            case '+':
                result = a + b;
                break;
            case '-':
                result = a - b;
                break;
            case '*':
                result = a * b;
                break;
            case '/':
                result = a / b;
                break;
            case '%':
                result = a % b;
                break;
            }
            push(&s, result);
        }
        token = strtok(NULL, " ");
    }

    return pop(&s);
}

int main()
{
    for (int i = 0; i < 3; i++)
    {
        char infix[MAX_SIZE];
        char postfix[MAX_SIZE];

        if (fgets(infix, sizeof(infix), stdin) == NULL)
        {
            break;
        }

        infix[strcspn(infix, "\n")] = '\0';

        infixtopostfix(infix, postfix);

        int j = 0;
        while (postfix[j] != '\0')
        {
            if (postfix[j] != ' ')
            {
                printf("%c", postfix[j]);
            }
            j++;
        }

        long long result = postfixeval(postfix);

        printf("=%lld\n", result);
    }

    return 0;
}