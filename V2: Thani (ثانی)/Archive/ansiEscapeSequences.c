
#include <stdio.h>

int main() {

    printf("\033[1;33mBold Yellow text\033[0m\n");
    printf("\033[9;33mYellow Strikethrough Text\033[0m\n");
    printf("\033[1;9;33mBold Yellow Strikethrough Text\033[0m\n");
    printf("\033[1;34mBold Blue text\033[0m\n");
    printf("\033[9;34mBlue Strikethrough Text\033[0m\n");
    printf("\033[1;9;34mBold Blue Strikethrough Text\033[0m\n\n");

    printf("\033[1;33;40mYellow Text, Grey highlight\033[0m\n");
    printf("\033[1;34;40mBlue Text, Grey highlight\033[0m\n");  // limit
    printf("🔴\n");

    // 1: bold, 3: italic, 4: underline, 9: strikethrough
    // chain multiple formats by ;

    printf("\n\033[3m1 22 333 4444 55555\033[0m\n");
    printf("\033[4m1 22 333 4444 55555\033[0m\n");
    printf("\033[9m1 22 333 4444 55555\033[0m\n");
    printf("\033[1;4;9;31mBold, Underlined Red\033[0m\n");
    
    return 0;
}
