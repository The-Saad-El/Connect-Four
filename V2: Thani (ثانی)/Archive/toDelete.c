// 7:56 2/10/25

#include <stdio.h>


void printPodium(char p1[10], int score, char p2[10], int score2, char p3[10], int score3)
{
    // this took long...
    char mscore[10];
    sprintf(mscore, "(%d)", score);

    printf("\n\n\n");
    printf("%27s%9s\n", " ", p1);
    printf("%27s%10s\n", " ", mscore);
    printf("%27s---------------\n", " ");          // 13 dashes
    printf("%27s|%6s\033[1;33m1\033[0m%6s|\n", " ", " ", " ");
    printf("%27s|%13s|\n", " ", " ");
    printf("%27s|%13s|\n", " ", " ");

    printf("%25s  |%13s|\n", p2, " ");
    sprintf(mscore, "(%d)", score2);
    printf("%25s  |%13s|\n", mscore, " ");
    printf("%13s--------------|%13s|\n", " ", " ");
    printf("%13s|%6s\033[1;33m2\033[0m%6s|%13s|\n", " ", " ", " ", " ");
    printf("%13s|%13s|%13s|\n", " ", " ", " ");

    printf("%13s|%13s|%13s|  %s\n", " ", " ", " ", p3);
    sprintf(mscore, "(%d)", score3);
    printf("%13s|%13s|%13s|  %s\n", " ", " ", " ", mscore);
    printf("%13s|%13s|%13s|--------------\n", " ", " ", " ");
    printf("%13s|%13s|%13s|%6s\033[1;33m3\033[0m%6s|\n", " ", " ", " ", " ", " ");
    printf("%13s|%13s|%13s|%13s|\n", " ", " ", " ", " ");
    printf("%13s|%13s|%13s|%13s|\n", " ", " ", " ", " ");
    printf("%13s|%13s|%13s|%13s|\n", " ", " ", " ", " ");
    printf("%13s|%13s|%13s|%13s|\n", " ", " ", " ", " ");
    //printf("%13s-------------------------------------------", " ");
    printf("\n\n\n\n:>");
}

int main()
{
    printPodium("muhammad Saad", 123, "Shayan", 21, "huzaifa bhai", 99999);


    return 0;
}