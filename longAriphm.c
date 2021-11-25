#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

const int NUMBSIZE = 35;
const int INF = -100;
const int mod = 1e9;

typedef struct
{
    int numb[35];
}uint1024_t;

void printfValue(uint1024_t x)
{
    int flag = 0;
    for(int i = 0; i < NUMBSIZE; i++)
    {
        if(x.numb[i] == INF)
        {
            continue;
        }
        if(x.numb[i] == 0)
        {
            if(flag)
            {
                printf("000000000 ");
            }
            else
            {
                printf("0");
                return;
            }
        }
        else
        {
            if(x.numb[i] / (mod / 10) == 0 && flag)
            {
                int k = x.numb[i], i = 0;
                while (k)
                {
                    k /= 10;
                    i++;
                }
                for(i = 9 - i; i > 0; i--)
                {
                    printf("0");
                }
                

            }
            printf("%d ", x.numb[i]);
            flag = 1;
        }
    }
}

void putInf(uint1024_t *x)
{
    for(int i = 0; i < NUMBSIZE; i++)
    {
        x->numb[i] = INF;
    }
}

void scanfValue(uint1024_t *x, char number[])
{
    char digit = '0';
    int rankCnt = 1, partCnt = NUMBSIZE - 1;

    putInf(x);

    int lenght = strlen(number);

    if(number[0] == '0' && lenght > 1)
    {
        printf("Wrong number format.");
        return;
    }

    for(int i = lenght - 1; i >= 0; i--)
    {
        digit = number[i];

        if(digit > '9' || digit < '0')
        {
            printf("Wrong number format.");
            return;
        }

        if(x->numb[partCnt]  == INF)
        {
            x->numb[partCnt] = 0;

        }

        x->numb[partCnt] += (digit - '0') * rankCnt;

        rankCnt *= 10;

        if(rankCnt > 1e8)
        {
            rankCnt = 1;
            partCnt--;
        }

        if(partCnt < 0)
        {
            printf("Too large number.");
            return;
        }

    }

}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;

    putInf(&result);

    //printfValue(result);

    int cur = 0;

    for(int i = NUMBSIZE - 1; i >= 0; i--)
    {
        if(x.numb[i] == INF && y.numb[i] == INF)
        {
            break;
        }
        if(x.numb[i] != INF && y.numb[i] != INF)
        {
            cur = x.numb[i] + y.numb[i];
            if(result.numb[i] == INF)
            {
                result.numb[i] = 0;
            }
            result.numb[i] += cur % mod;

            if(cur / mod)
            {
                if(i - 1 < 0)
                {
                    printf("Result is too large.");
                    return result;
                }
                result.numb[i - 1] = 1;
            }
        }
        else
        {
            if(x.numb[i] != INF || y.numb[i] != INF)
            {
                result.numb[i] = fmax(x.numb[i], y.numb[i]);
            }
        }

        cur = 0;

    }

    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;

    putInf(&result);

    int cur = 0;

    for(int i = NUMBSIZE - 1; i >= 0; i--)
    {
        if(x.numb[i] == INF && y.numb[i] == INF)
        {
            break;
        }

        if(y.numb[i] == INF)
        {
            result.numb[i] = x.numb[i];
            continue;
        }

        if(x.numb[i] < y.numb[i])
        {
            result.numb[i] = x.numb[i] + mod - y.numb[i];
            if(i - 1 < 0)
            {
                printf("Result is too large.");
                return result;
            }
            if(y.numb[i - 1] == INF)
            {
                y.numb[i - 1] = 1;
            }
            else
            {
                y.numb[i - 1]++;
            }
        }
        else
        {
            result.numb[i] = x.numb[i] - y.numb[i];
        }
    }

    return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;

    putInf(&result);

    long long cur = 0;

    for(int i = NUMBSIZE - 1; i >= 0; i--)
    {
        if(y.numb[i] == INF)
        {
            break;
        }
        for(int j = NUMBSIZE - 1; j >= 0; j--)
        {
            if(x.numb[j] == INF)
            {
                break;
            }

            if(j - (NUMBSIZE - 1 - i) < 0)
            {
                printf("Result is too large.");
                return result;
            }
            cur = (long long)(x.numb[j]) * (long long)(y.numb[i]);
            if(result.numb[j - (NUMBSIZE - 1 - i)] == INF)
            {
                result.numb[j - (NUMBSIZE - 1 - i)] = cur % mod;
            }
            else
            {
                result.numb[j - (NUMBSIZE - 1 - i)] += cur % mod;
            }
            cur /= mod;
            if(result.numb[j - (NUMBSIZE - 1 - i) - 1] == INF)
            {
                result.numb[j - (NUMBSIZE - 1 - i) - 1] = 0;
            }
            result.numb[j - (NUMBSIZE - 1 - i) - 1] += cur + result.numb[j - (NUMBSIZE - 1 - i)] / mod;
            result.numb[j - (NUMBSIZE - 1 - i)] %= mod;
            if(result.numb[j - (NUMBSIZE - 1 - i) - 1] == 0)
            {
                result.numb[j - (NUMBSIZE - 1 - i) - 1] = INF;
            }
            
            cur = 0;
        }

    }

    return result;
}

uint1024_t generation(unsigned int x)
{
    uint1024_t result;
    putInf(&result);
    result.numb[NUMBSIZE - 1] = x % mod;
    x /= mod;
    result.numb[NUMBSIZE - 2] = x;
    return result;

}

/*
 * 3 arguments expected: program.exe [number] [OPTION] [number]
 * argv[0] - program
 * argv[1] - number1 / int x
 * argv[2] - option
 * argv[3] - number2
 */
int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        unsigned int x = 0, rankCnt = 1, len = strlen(argv[1]);
        for(int i = len - 1; i >= 0; i--)
        {
            x += (argv[1][i] - '0') * rankCnt;
            rankCnt *= 10;
        }
        printfValue(generation(x));
        return 0;
    }
    if (argc != 4)
    {
        printf("Wrong amount of arguments.");
        return 0;
    }
    int option = 2, number1 = 1, number2 = 3;
    uint1024_t numb1, numb2;

    scanfValue(&numb1, argv[number1]);
    scanfValue(&numb2, argv[number2]);
    printfValue(numb1);
    printf("\n");
    printfValue(numb2);
    printf("\n");

    if(!strcmp(argv[option], "+"))
    {
        uint1024_t result = add_op(numb1, numb2);
        printfValue(result);
        return 0;
    }

    if(!strcmp(argv[option], "-"))
    {
        //-
        uint1024_t result;
        int flag = 0;
        for(int i = 0; i < NUMBSIZE; i++)
        {
            if(numb1.numb[i] > numb2.numb[i]){
                result = subtr_op(numb1, numb2);
                flag = 1;
                break;
            }
            if(numb1.numb[i] < numb2.numb[i]){
                result = subtr_op(numb2, numb1);
                flag = 1;
                printf("-");
                break;
            }
        }
        if(flag)
        {
            printfValue(result);
        }
        else
        {
            printf("0");
        }
    }

    if(!strcmp(argv[option], "*"))
    {
        //*
        uint1024_t result = mult_op(numb1, numb2);
        printfValue(result);

    }

    return 0;
}
