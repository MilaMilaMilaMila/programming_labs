#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

const int MAXACCESSAMOUNT = 2e6;
const int MAXSIZESTR = 4100;
int errorCounter = 0;
int accessAmount = 0;

int convertTimeInSeconds(char acessStr[])
{
    int sizeAccessStr = strlen(acessStr);

    int totalTime = 0;

    int seconds = 0;
    int minutes = 0;
    int hours = 0;
    int days = 0;

    for(int i = 0; i < sizeAccessStr; i++)
    {
        if(acessStr[i] == '[')
        {
            days = (acessStr[i + 1] - '0') * 10 + acessStr[i + 2] - '0';
            hours = ((acessStr[i + 13] - '0') * 10 + acessStr[i + 14] - '0');
            minutes = ((acessStr[i + 16] - '0') * 10 + acessStr[i + 17] - '0');
            seconds = ((acessStr[i + 19] - '0') * 10 + acessStr[i + 20] - '0');
            totalTime = (days - 1) * 24 * 60 * 60 + hours * 60 * 60 + minutes * 60 + seconds;

            return totalTime;
        }
    }

    return totalTime;

}

void writeRequestInFile(char accessStr[])
{
    int sizeAccessStr = strlen(accessStr);
    int leftSpaces = 5;
    int rightSpaces = 2;
    int firstSymbolInRequest = sizeAccessStr;
    int lastsymbolInRequest = sizeAccessStr;

    for(int i = 0; i < sizeAccessStr; i++)
    {
        if(accessStr[i] == ' ')
        {
            leftSpaces--;
            if(leftSpaces == 0)
            {
                firstSymbolInRequest = i;
                break;
            }
        }  
    }

    for(int i = sizeAccessStr - 1; i >= 0; i--)
    {
        if(accessStr[i] == ' ')
        {
            rightSpaces--;
            if(rightSpaces == 0)
            {
                lastsymbolInRequest = i;
                break;
            }
        }  
    }

    FILE *errorRuquestFile;
    errorRuquestFile = fopen("errorRequests.txt", "a");

    for(int i = firstSymbolInRequest; i <= lastsymbolInRequest; i++)
    {
        fprintf(errorRuquestFile, "%c", accessStr[i]);
    }

    fprintf(errorRuquestFile, "\n");

    fclose(errorRuquestFile);

}

void checkRequstOnError(char accessStr[])
{
    int sizeAccessStr = strlen(accessStr);
    for(int i = sizeAccessStr - 1; i >= 0; i--)
    {
        if (accessStr[i] == ' ')
        {
            if(accessStr[i - 3] == '5')
            {
                errorCounter++;
                writeRequestInFile(accessStr);
                
            }

            break;
        }
    }
}

int findMaxRequestAmountOnInterval(int timePeriod, int arrayAccessTime[])
{
    int leftTimeRequestBound = 0;
    int rightTimeRequestBound = 0;
    int maxRequestAmount = 0;
    int finishLeftTimeRequestBound = 0;
    int finishRightTimeRequestBound = 0;
    
    for(int i = 0; i < accessAmount * 2; i++)
    {
        if(rightTimeRequestBound + 1 >= accessAmount)
        {
            return maxRequestAmount;
        }
 
        if(rightTimeRequestBound + 1 < accessAmount && arrayAccessTime[rightTimeRequestBound + 1] - arrayAccessTime[leftTimeRequestBound] < timePeriod)
        {
            
            rightTimeRequestBound++;

            if(rightTimeRequestBound - leftTimeRequestBound + 1 > maxRequestAmount)
            {
                finishLeftTimeRequestBound = arrayAccessTime[leftTimeRequestBound];
                finishRightTimeRequestBound = arrayAccessTime[rightTimeRequestBound];
                maxRequestAmount = rightTimeRequestBound - leftTimeRequestBound + 1;
            }
            
        }
        else
        {
            
            leftTimeRequestBound++;
            if(leftTimeRequestBound == accessAmount)
            {
                return maxRequestAmount;
            }
            rightTimeRequestBound = fmax(rightTimeRequestBound, leftTimeRequestBound);
        }
        
    }

    int firstDay = finishLeftTimeRequestBound / (24 * 60 * 60) + 1;
    finishLeftTimeRequestBound %= 24 * 60 * 60;
    int firstHour = finishLeftTimeRequestBound / (60 * 60);
    finishLeftTimeRequestBound %= 60 * 60;
    int firstMinutes = finishLeftTimeRequestBound / 60;
    int firstSeconds = finishLeftTimeRequestBound % 60;

    int lastDay = finishRightTimeRequestBound / (24 * 60 * 60) + 1;
    finishRightTimeRequestBound %= 24 * 60 * 60;
    int lastHour = finishRightTimeRequestBound / (60 * 60);
    finishRightTimeRequestBound %= 60 * 60;
    int lastMinutes = finishRightTimeRequestBound / 60;
    int lastSeconds = finishRightTimeRequestBound % 60;

    printf("time of first request in interval [%d/Jul/1995:%d:%d:%d -0400]\n", firstDay, firstHour, firstMinutes, firstSeconds);
    printf("time of last request in interval [%d/Jul/1995:%d:%d:%d -0400]\n", lastDay, lastHour, lastMinutes, lastSeconds);
    return maxRequestAmount;
}

void parsingAccess(int arrayAccessTime[])
{
    FILE *accessFile;
    accessFile = fopen("log.txt", "r");

    char accessStr[MAXSIZESTR];
    int numberOfCurrentAccess = 0;
    
    while(fgets(accessStr, MAXSIZESTR, accessFile))
    { 
        arrayAccessTime[numberOfCurrentAccess++] = convertTimeInSeconds(accessStr);
        checkRequstOnError(accessStr);
    }

    accessAmount = numberOfCurrentAccess;

    printf("Amount of requests with error %d\nList with requests there is in file \"errorRequests.txt\"\n", errorCounter);

    fclose(accessFile);
}

int main(int argc, char *argv[])
{
    printf("enter the time period in seconds\n");
    int timePeriod;
    scanf("%d", &timePeriod);

    int *arrayAccessTime;
    arrayAccessTime = (int*)malloc(MAXACCESSAMOUNT * sizeof(int));

    parsingAccess(arrayAccessTime);

    printf("Max amount of requests with difference less then %d seconds is %d\n", timePeriod, findMaxRequestAmountOnInterval(timePeriod, arrayAccessTime));

    free(arrayAccessTime);

    return 0;
}