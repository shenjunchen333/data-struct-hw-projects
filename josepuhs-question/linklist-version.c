#include <stdio.h>
#include<stdlib.h>
//定义链表结点
typedef struct Node {
    int id;
    int pwd;
    struct Node *next;
}Node;
//定义链表
typedef struct {
    Node *head;
    Node *tail;
}List;
//初始化链表
void initialization_list(List *ll) {
    ll->head=NULL;
    ll->tail=NULL;
}
//创建链表
void create_list(int id,int pwd,Node **node) {
    *node=(Node*)malloc(sizeof(Node));
    (*node)->id=id;
    (*node)->pwd=pwd;
    (*node)->next=NULL;
}
//在循环链表尾部添加新结点
void append_node(List *ll, Node *new_node) {
    if (ll->head==NULL) {
        ll->head=new_node;
        ll->tail=new_node;
        new_node->next=new_node;//成环
    }else {
        ll->tail->next=new_node;
        new_node->next=ll->head;
        ll->tail=new_node;
    }
}
//根据密码组建链表//一切造物的工已经完毕
void God_End_His_Work(int n,int pwd[],List* ll) {
    for (int i=0;i<n;i++) {
        Node *new_node;
        create_list(i+1,pwd[i],&new_node);
        append_node(ll,new_node);
    }
}
//约瑟夫环过程//无疑之日已至
void josephus(List *ll,int m) {
    if (ll->head==NULL) return;
    Node *cur=ll->head;
    Node *prev=ll->tail;
    int left=1;//剩余人数
    Node *p=ll->head;
    p=p->next;
    while (p!=ll->head) {
        left++;
        p=p->next;
    }
    printf("The order:");
    while (left>0) {
        if (left==1) {
            printf("%d",cur->id);
            free(cur);
            break;
        }else {
            for (int i=1;i<m;i++) {
                prev=cur;
                cur=cur->next;
            }
            printf("%d,",cur->id);
            m=cur->pwd;
            //删除cur结点
            left--;
            prev->next=cur->next;
            Node *temp=cur;
            cur=prev->next;
            free(temp);
        }
    }
}
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
    int pwd[30];
    for(int i=0;i<n;i++) {
        printf("Please enter the password of person%d:",i+1);
        while (scanf("%d",&pwd[i]) != 1||pwd[i]<1) {
            printf("Please enter a valid number!\n");
            printf("Please enter the password of person%d:",i+1);
            clear_input_buffer();
        }
        clear_input_buffer();
    }
    List list;
    initialization_list(&list);
    God_End_His_Work(n,pwd,&list);
    josephus(&list,m);
    return 0;
}
