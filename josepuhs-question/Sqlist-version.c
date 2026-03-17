#include <stdio.h>

typedef struct Node {
    int id;
    int pwd;
    int position;//0表示列外，1表示列内
}Node;

//错误检查发生后清理缓存
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int m;
    int n;
    printf("Please enter the upper limit of counting numbers:");
    while (scanf("%d",&m) != 1||m<1) {
        printf("Please enter a valid number!\n");
        printf("Please enter the upper limit of counting numbers:");
        clear_input_buffer();
    }
    clear_input_buffer();
    printf("Please enter the number of people:");
    while (scanf("%d",&n) != 1||n<1) {
        printf("Please enter a valid number!\n");
        printf("Please enter the upper limit of counting numbers:");
        clear_input_buffer();
    }
    clear_input_buffer();
    Node nodes[30];
    for(int i=0;i<n;i++) {
        nodes[i].id=i+1;
        printf("Please enter the password of person%d:",i+1);
        while (scanf("%d",&nodes[i].pwd) != 1||nodes[i].pwd<1) {
            printf("Please enter a valid number!\n");
            printf("Please enter the password of person%d:",i+1);
            clear_input_buffer();
        }
        clear_input_buffer();
        nodes[i].position=1;
    }
    int left=n;
    int cur_idx=0;
    printf("The order:");
    while (left>0) {
        int count=0;
        while (1) {
            if (nodes[cur_idx].position) {
                count++;
                if (count==m) {
                    break;
                }
            }
            cur_idx=(cur_idx+1)%n;
        }
        printf("%d,",nodes[cur_idx].id);
        m=nodes[cur_idx].pwd;
        nodes[cur_idx].position=0;
        left--;
        cur_idx=(cur_idx+1)%n;
    }
    return 0;
}
