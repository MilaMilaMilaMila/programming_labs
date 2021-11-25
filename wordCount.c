#include <stdio.h>
#include <string.h>

int lines_counter(){
    int cur_symbol = 0, lines_cnt = 1;

    while(cur_symbol != EOF){

        cur_symbol = getchar();
        lines_cnt += cur_symbol == '\n';

    }

    return lines_cnt;

}

int bytes_counter(){
    int cur_symbol = 0, bytes_cnt = 0;

    while (cur_symbol != EOF) {
        cur_symbol = getchar();
        
        if (cur_symbol == -1){
            return bytes_cnt;
        }
        bytes_cnt += 1 + (cur_symbol == '\n');
    }

    return bytes_cnt;
    
}

int words_counter(){
    int cur_symbol = 0, flag = 0, word_cnt = 0;

    while (cur_symbol != EOF){

        cur_symbol = getchar();

        if (cur_symbol != ' ' && cur_symbol != '\n' && cur_symbol != '\t'){
            flag = 1;
        }
        else{

            word_cnt += flag; 
            flag = 0;
        }
    }

    return word_cnt;

}

/*
 * 3 arguments expected: WordCont.exe [OPTION] filename
 * argv[0] - program
 * argv[1] - option
 * argv[2] - file
 */
int main(int argc, char *argv[]) {

    if (argc != 3){
        printf("Wrong amount of arguments.");
        return 0;
    }

    if(freopen(argv[2], "r", stdin) != NULL){

        if(!strcmp(argv[1], "-l") || !strcmp(argv[1], "--lines")){

            printf("Amount of lines: %d", lines_counter());
            return 0;

        }
        if(!strcmp(argv[1], "-c") || !strcmp(argv[1], "--bytes")){

            printf("Amount of bytes: %d", bytes_counter());
            return 0;

        }
        if(!strcmp(argv[1], "-w") || !strcmp(argv[1], "--words")){

            printf("Amount of words: %d", words_counter());
            return 0;

        }

        printf("Wrong request name.");

        FILE *fp = freopen(argv[2], "r", stdin); fclose(fp);

    }
    else{
        printf("File couldn't be open.");
    }
	return 0;
}