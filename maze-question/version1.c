#include <stdio.h>
#include <stdlib.h>
//错误检查
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//指针型栈结点
typedef struct stacknode {
    int x;
    int y;
    int direct;
    struct stacknode* next;
}stacknode;
//栈与其头指针
typedef struct linkedstack {
    stacknode* top;
}linkedstack;
//初始化栈（栈空）
void initStack(linkedstack* s) {
    s->top = NULL;
}
//判断空栈
int isEmptyStack(linkedstack* s) {
    if (s->top == NULL) {
        return 1;
    }else {
        return 0;
    }
}
//压入栈
void pushStack(linkedstack* s, int x,int y,int direct) {
    stacknode* newNode = (stacknode*)malloc(sizeof(stacknode));
    newNode->x = x;
    newNode->y = y;
    newNode->direct = direct;
    newNode->next = s->top;
    s->top = newNode;
}
//弹出栈
void popStack(linkedstack* s) {
    if (s->top != NULL) {
        stacknode* topNode = s->top;
        s->top = s->top->next;
        free(topNode);
    }
}
//不弹出地获取栈顶元素
int get_top(linkedstack* s,int *x,int *y,int *direct) {
    if (isEmptyStack(s)) {
        return 0;
    }
    *x=s->top->x;
    *y=s->top->y;
    *direct=s->top->direct;
    return 1;
}
//输出路径（反转输出）
void printpath(linkedstack *s) {
    int count=0;
    stacknode* p = s->top;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    p=s->top;
    stacknode **nodes = (stacknode**)malloc((count+1)*sizeof(stacknode*));
    for (int i = count-1; i>=0; i--) {
        nodes[i] = p;
        p = p->next;
    }
    for (int i=0;i<count;i++) {
        printf("(%d,%d,%d)",nodes[i]->x,nodes[i]->y,nodes[i]->direct);
        if (i<(count-1)) {
            printf("-->");
        }
    }
    free(nodes);
}

const int dx[5]={0,0,1,0,-1};
const int dy[5]={0,1,0,-1,0};//中-东-南-西-北

int main() {
    int m;int n;
    printf("Enter the number of rows:");
    while (scanf("%d", &m) != 1 || m <= 0) {
        printf("Invalid input. Please enter a valid number: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    printf("Enter the number of columns:");
    while (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input. Please enter a valid number: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    int map[m+2][n+2];
    //0表示通路，1表示阻碍
    //外围墙壁
    for (int i=0;i<m+2;i++) {
        for (int j=0;j<n+2;j++) {
            if (i==0||i==m+1||j==0||j==n+1) {
                map[i][j] = 1;
            }else {
                map[i][j] = 0;
            }
        }
    }
    //构建地图
    printf("0 for access,1 for not\n");
    for (int i=1;i<m+1;i++) {
        for (int j=1;j<n+1;j++) {
            int data=0;
            printf("Please enter the state of area(%d,%d):",i,j);
            while (scanf("%d", &data) != 1 || (data!=0&&data!=1)) {
                printf("Invalid input. Please enter a valid number: ");
                clear_input_buffer();
            }
            clear_input_buffer();
            map[i][j] = data;
        }
    }
    //获得起点和终点坐标
    int entrance_x;int entrance_y;int exit_x;int exit_y;
    printf("Enter the coordinates of the maze you want to enter\n");
    printf("Enter X coordinate:");
    while (scanf("%d",&entrance_x) != 1||entrance_x<1||entrance_x>m) {
        printf("Invalid input. Please enter a valid number: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    printf("Enter Y coordinate:");
    while (scanf("%d",&entrance_y) != 1||entrance_y<1||entrance_y>n) {
        printf("Invalid input. Please enter a valid number: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    printf("Enter the coordinates of the maze you want to exit\n");
    printf("Enter X coordinate:");
    while (scanf("%d",&exit_x) != 1||exit_x<1||exit_x>m) {
        printf("Invalid input. Please enter a valid number: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    printf("Enter Y coordinate:");
    while (scanf("%d",&exit_y) != 1||exit_y<1||exit_y>n) {
        printf("Invalid input. Please enter a valid number: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    if (map[entrance_x][entrance_y]==1) {
        printf("Can't enter maze yet\n");
        return 0;
    }
    if (map[exit_x][exit_y]==1) {
        printf("Can't exit maze yet\n");
        return 0;
    }
//核心算法
    int passed[m+2][n+2];
    for (int i=0;i<m+2;i++) {
        for (int j=0;j<n+2;j++) {
            passed[i][j]=0;
        }
    }//判断某坐标是否已经过
    linkedstack maze;
    initStack(&maze);
    pushStack(&maze,entrance_x,entrance_y,1);//压入起点坐标
    //核心算法
    while (!isEmptyStack(&maze)) {
        int curX, curY, curD;
        get_top(&maze,&curX,&curY,&curD);
        if (curX==exit_x && curY==exit_y) {
            maze.top->direct=0;
            printpath(&maze);
            printf("\n");
            int op=0;
            while(op!=1) {
                printf("Enter 1 to end:");
                while (scanf("%d",&op) != 1) {
                    printf("Enter 1 to end:");
                    clear_input_buffer();
                }
                clear_input_buffer();
                if (op==1) {
                    return 0;
                }
            }
        }
        int nextX=curX+dx[curD];
        int nextY=curY+dy[curD];
        if (nextX>=1 && nextX<=m && nextY>=1 && nextY<=n && map[nextX][nextY]==0 && passed[nextX][nextY]==0) {
            int nextD=1;
            if (nextX==exit_x && nextY==exit_y) {
                nextD=0;
            }
            pushStack(&maze,nextX,nextY,nextD);
            passed[nextX][nextY]=1;
        }else {
            curD++;
            if (curD<=4) {
                maze.top->direct=curD;
            }else {
                popStack(&maze);
            }
        }
    }
    printf("No accessible way!");
    return 0;
}
