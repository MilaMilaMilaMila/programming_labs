#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <locale.h>

size_t getFileSize(char *file)
{
    size_t size;
    FILE *song = fopen(file, "r");
    fseek(song, 0, SEEK_END);
    size = ftell(song);
    fclose(song);
    return size;
}

int toInt(char size[4]) // syncsafe byte в int
{
    return (int)size[0] * 256 * 256 * 256 + (int)size[1] * 256 * 256 + (int)size[2] * 256 + (int)size[3];
}

void toBytes(int x, uint8_t size[4]) // int в syncsafe byte 
{
    int bits = 127;                         // 0111_1111
    size[0] = (x >> 21) & bits;
    size[1] = (x >> 14) & bits;
    size[2] = (x >> 7) & bits;
    size[3] = x & bits;
}

void show(char *song, int isGet, char *wanted)
{
    printf("Tag  | Frame\n");
    printf("-------------------------------------------------------------------\n");

    int i = 10;
    while (song[i])
    {
        char frame[5];
        frame[4] = '\0';

        for (int j = 0; j < 4; j++)
        {
            frame[j] = song[i++]; // считали тэг
        }

        if (isGet == 0 || !strcmp(frame, wanted))
        {
            printf("%s | ", frame);//вывели тэг
        }

        char size[4];
        for (int j = 0; j < 4; j++)
        {
            size[j] = song[i++];//считали размер фрейма
        }

        int frameSize = toInt(size);//перевели размер фрейма в инт для цикла

        i += 2;
        //вывели фрейм
        for (int j = 0; j < frameSize; j++)
        {
            if (isGet == 0 || !strcmp(frame, wanted))
            {
                printf("%c", song[i]);

                if (j == frameSize - 1)
                {
                    printf("\n");
                }
            }
            i += 1;
        }
    }
}

void change(char *tag, char *newFrame, char *fileName, char *song)
{

    FILE *readFile;
    readFile = fopen(fileName, "rb");//исходный файл
    FILE *tempFile;
    tempFile = fopen("temporary.mp3", "wb");//файл для измененного аудио

    int newSize = strlen(newFrame) + 1;//длина нового фрейма
    uint8_t newSB[4];//контейнер для длины нового фрейма в байтах
    toBytes(newSize, newSB);//длина ноыого фрейма в байтах

    int i = 10;
    while (song[i])
    {
        char frame[5];
        frame[4] = '\0';

        for (int j = 0; j < 4; j++)
        {
            frame[j] = song[i + j];//считали тэг
        }

        int flag = !strcmp(frame, tag);//проверка, что это нужный тэг

        if(flag)
        {
            fwrite(song, 1, i, tempFile);//записали в файл то, что не нужно изменять до i
            fwrite(tag, 1, 4, tempFile);//записали в файл изменяемый тэг
            fwrite(newSB, 1, 4, tempFile);//записали в файл новый размер изменяемого тэга
            uint8_t kostil[3];
            kostil[0] = 0;
            kostil[1] = 0;
            kostil[2] = 0;
            fwrite(kostil, 1, 3, tempFile);
            fwrite(newFrame, 1, strlen(newFrame), tempFile);//записали новый фркйм тэга
        }

        i += 4;

        char sizeF[4];//размер старого фрейма
        for (int j = 0; j < 4; j++)
        {
            sizeF[j] = song[i + j];//считали размер старого фрейма
        }

        int frameSize = toInt(sizeF);//перевели размер старого фрейма в инт

        if(flag)
        {
            int defference = frameSize - newSize;//нашли разницу в размерах фреймов
            char HeaderSize[4];//размер метаданных 
            HeaderSize[0] = song[6];
            HeaderSize[1] = song[7];
            HeaderSize[2] = song[8];
            HeaderSize[3] = song[9];
            int intHeadSize = toInt(HeaderSize);//перевели размер метаданных в инт
            intHeadSize -= defference;//нашли новый размер метаданных
            uint8_t newHeaderSize[4];//новый размер метаданных
            toBytes(intHeadSize, newHeaderSize);//нашли новый размер метаданных для записи в файл
            fseek(tempFile, 6, SEEK_SET);//переставиои указатель в файле на начало разера метаданных в заголовке
            fwrite(newHeaderSize, 1, 4, tempFile);//записали новый размер метаданных в заголовок
            fseek(tempFile, i + 4 + 3 + strlen(newFrame), SEEK_SET);//вернули указатель в файле обратно для дозаписи

        }

        i += 6;

        i += frameSize;

        //записали оставшуюся чать файла в измененный
        if(flag)
        {
            int need = getFileSize(fileName) - i;
            char *endFile = (char *)malloc(sizeof(char) * (need));
            fseek(readFile, i, SEEK_SET);
            fread(endFile, 1, need, readFile);
            fwrite(endFile, 1, need, tempFile);
            fclose(tempFile);
            fclose(readFile);
            return;
        }
    }
    //если тэга нет, ничего не меняем
    fwrite(song, 1, getFileSize(fileName), tempFile);
}

int main(int argc, char *argv[])
{
    // argv[1] - filename
    // argv[2] - --show --get если argc 3
    // argv[2] - --set иначе
    // argv[3] - --value

    char *fileName;
    fileName = strpbrk(argv[1], "=") + 1; //после = идет имя файла

    FILE *file;
    file = fopen(fileName, "rb");

    char *song = (char *)malloc(sizeof(char) * getFileSize(fileName));
    fread(song, 1, getFileSize(fileName), file);

    if (argc < 3)
    {
        printf("wrong input format\n");
        return 0;
    }

    if (argc == 3)
    {
        if (strstr(argv[2], "--show"))
        {
            show(song, 0, "");
            return 0;
        }

        if (strstr(argv[2], "--get"))
        {
            show(song, 1, strpbrk(argv[2], "=") + 1);
            return 0;
        }

        return 0;
    }

    if(argc == 4)
    {
        change(strpbrk(argv[2], "=") + 1, strpbrk(argv[3], "=") + 1, fileName, song);
    }

    return 0;
}