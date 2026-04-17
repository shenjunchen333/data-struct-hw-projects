#include <stdio.h>
#include <stdlib.h>
#define MAXRC 21
#define MAXSIZE 400
typedef int datatype;
//三元组顺序表
typedef struct{
    int   i,j;
    datatype v;
}triple;

typedef struct {
    triple data[MAXSIZE+1];
    int m,n,t;
}tripletable;
//带行逻辑链接信息的三元组表
typedef struct{
    triple data[MAXSIZE+1];
    int     rpos[MAXRC+1];
    int    m,n,t;
}rtripletable;
//输入用三元组表示的稀疏矩阵
void input_matrix_tripletable(tripletable *t) {
    printf("Input the triples by row priority:\n");
    printf("Enter the number of rows,columns and non-zero elements:");
    scanf("%d %d %d",&t->m,&t->n,&t->t);
    for (int k=1;k<=t->t;k++) {
        printf("(?,?,?)\n");
        printf("Row:");
        scanf("%d",&t->data[k].i);
        printf("Column:");
        scanf("%d",&t->data[k].j);
        printf("Data:");
        scanf("%d",&t->data[k].v);
    }
}
//三元组表带上行逻辑链接(乘法需要)
void triple_to_rtriple(tripletable t,rtripletable *r) {
    r->m=t.m;
    r->n=t.n;
    r->t=t.t;
    for (int i=1;i<=t.t;i++) {
        r->data[i]=t.data[i];
    }
    //求得rpos
    int num[MAXRC]={0};
    for (int k=1;k<=t.t;k++) {
        num[t.data[k].i]++;
    }
    r->rpos[1]=1;
    for (int R=2;R<=r->m;R++) {
        r->rpos[R]=num[R-1]+r->rpos[R-1];
    }
}
//矩阵乘法，存入c
void mul(rtripletable a,rtripletable b,rtripletable *c){
    c->m=a.m;
    c->n=b.n;
    c->t=0;
    int ccol=0;
    int ctemp[MAXRC]={0};
    if (a.t*b.t!=0) {
        for (int arow=1;arow<=a.m;arow++) {
            for (int col=1;col<=c->n;col++)ctemp[col]=0;
            c->rpos[arow]=c->t+1;
            int tp=0;
            if (arow<a.m) {
                tp=a.rpos[arow+1];
            }else {
                tp=a.t+1;
            }
            for (int p=a.rpos[arow];p<tp;p++) {
                int brow=a.data[p].j;
                int t=0;
                if (brow<b.m) {
                    t=b.rpos[brow+1];
                }else {
                    t=b.t+1;
                }
                for (int q=b.rpos[brow];q<t;q++) {
                    ccol=b.data[q].j;
                    ctemp[ccol]+=a.data[p].v*b.data[q].v;
                }
            }
            for (ccol=1;ccol<=c->n;ccol++) {
                if(ctemp[ccol]){
                    c->t++;
                    c->data[c->t].i=arow;
                    c->data[c->t].j=ccol;
                    c->data[c->t].v=ctemp[ccol];
                }
            }
        }
    }
}
//打印矩阵（乘法）
void print_Smatrix(rtripletable t) {
    int A[MAXRC][MAXRC]={0};
    for (int k=1;k<=t.t;k++) {
        A[t.data[k].i][t.data[k].j]=t.data[k].v;
    }
    for (int i=1;i<=t.m;i++) {
        if (i==1||i==t.m) {
            printf("+");
        }else {
            printf("|");
        }
        for (int j=1;j<=t.n;j++) {
            printf("%4d ",A[i][j]);
        }
        if (i==1||i==t.m) {
            printf("+\n");
        }else {
            printf("|\n");
        }
    }
}
//十字链表
typedef struct OLNode {
    int i,j;
    datatype e;
    struct OLNode *right,*down;
}OLNode,*Olink;

typedef struct {
    Olink *rhead, *chead;
    int mu,nu,tu;
}Crosslist;
//创建十字链表
int create_cross_list(Crosslist *m,tripletable t) {
    m->mu=t.m;
    m->nu=t.n;
    m->tu=t.t;
    //分配行、列头指针两个数组
    m->rhead=(Olink*)malloc((m->mu+1)*sizeof(Olink));
    m->chead=(Olink*)malloc((m->nu+1)*sizeof(Olink));
    //分配失败
    if (!m->rhead||!m->chead) {
        return 0;
    }
    //初始化
    for (int i=1;i<=m->mu;i++) {
        m->rhead[i]=NULL;
    }
    for (int j=1;j<=m->nu;j++) {
        m->chead[j]=NULL;
    }
    //按行优先插入结点
    for (int k=1;k<=t.t;k++) {
        OLNode *p=(OLNode*)malloc(sizeof(OLNode));
        p->i=t.data[k].i;
        p->j=t.data[k].j;
        p->e=t.data[k].v;
        //插入行列表，生成结点
        int i=t.data[k].i;
        int j=t.data[k].j;
        if (m->rhead[i]==NULL||m->rhead[i]->j>j) {
            p->right=m->rhead[i];
            m->rhead[i]=p;
        }else { //寻查在行表中的插入位置
            OLNode *q=m->rhead[i];
            while (q->right&&q->right->j<j) {
                q=q->right;
            }
            p->right=q->right;
            q->right=p;
        }//行插入已完成
        if (m->chead[j]==NULL||m->chead[j]->i>i) {
            p->down=m->chead[j];
            m->chead[j]=p;
        }else {//寻查在列表中的插入位置
            OLNode *q=m->chead[j];
            while (q->down&&q->down->i<i) {
                q=q->down;
            }
            p->down=q->down;
            q->down=p;
        }//列插入已完成
    }
    return 1;
}
//销毁十字链表
void destroy_cross_list(Crosslist *m) {
    for (int i=1;i<=m->mu;i++) {
        OLNode *p=m->rhead[i];
        OLNode *q;
        while (p) {
            q=p;
            p=p->right;
            free(q);
        }
    }
    free(m->rhead);
    free(m->chead);
}
//十字链表转换成三元组表输出
void cross_to_triple(Crosslist m,tripletable *t) {
    t->m=m.mu;
    t->n=m.nu;
    t->t=0;
    for (int i=1;i<=m.mu;i++) {
        OLNode *p=m.rhead[i];
        while (p) {
            t->t++;
            t->data[t->t].i=p->i;
            t->data[t->t].j=p->j;
            t->data[t->t].v=p->e;
            p=p->right;
        }
    }
}
//矩阵加法，修改a
void add(Crosslist *a,Crosslist b) {
    for (int i=1;i<=b.mu;i++) {
        OLNode *pb=b.rhead[i];
        while (pb) {
            OLNode *pa=a->rhead[i];
            OLNode *pre=NULL;
            datatype e=pb->e;
            //在A中找（i，j）结点
            int j=pb->j;
            while (pa&&pa->j<j) {
                pre=pa;
                pa=pa->right;
            }
            if (pa&&pa->j==j) {//位置存在
                pa->e+=e;
                if (pa->e==0) {//植为0，删除结点
                    //先删行链表
                    if (pre==NULL) {
                        a->rhead[i]=pa->right;
                    }else {
                        pre->right=pa->right;
                    }
                    //再删列链表
                    OLNode *colpre=NULL;
                    OLNode *colp=a->chead[j];
                    while (colp&&colp->i<i) {
                        colpre=colp;
                        colp=colp->down;
                    }
                    if (colpre==NULL) {
                        a->chead[j]=pa->down;
                    }else {
                        colpre->down=pa->down;
                    }
                    free(pa);
                    a->tu--;
                }
            }else {//位置不存在，插入新结点
                OLNode *newNode=(OLNode*)malloc(sizeof(OLNode));
                newNode->i=i;
                newNode->j=j;
                newNode->e=e;
                //先插入行链表
                if (pre==NULL) {
                    newNode->right=a->rhead[i];
                    a->rhead[i]=newNode;
                }else {
                    newNode->right=pre->right;
                    pre->right=newNode;
                }
                //再插入列链表
                OLNode *colpre=NULL;
                OLNode *colp=a->chead[j];
                while (colp&&colp->i<i) {
                    colpre=colp;
                    colp=colp->down;
                }
                if (colpre==NULL) {
                    newNode->down=a->chead[j];
                    a->chead[j]=newNode;
                }else {
                    newNode->down=colpre->down;
                    colpre->down=newNode;
                }
                a->tu++;
            }
            pb=pb->right;
        }
    }
}
//矩阵减法，修改a
void sub(Crosslist *a,Crosslist b) {
for (int i=1;i<=b.mu;i++) {
        OLNode *pb=b.rhead[i];
        while (pb) {
            OLNode *pa=a->rhead[i];
            OLNode *pre=NULL;
            datatype e=pb->e*(-1);
            //在A中找（i，j）结点
            int j=pb->j;
            while (pa&&pa->j<j) {
                pre=pa;
                pa=pa->right;
            }
            if (pa&&pa->j==j) {//位置存在
                pa->e+=e;
                if (pa->e==0) {//植为0，删除结点
                    //先删行链表
                    if (pre==NULL) {
                        a->rhead[i]=pa->right;
                    }else {
                        pre->right=pa->right;
                    }
                    //再删列链表
                    OLNode *colpre=NULL;
                    OLNode *colp=a->chead[j];
                    while (colp&&colp->i<i) {
                        colpre=colp;
                        colp=colp->down;
                    }
                    if (colpre==NULL) {
                        a->chead[j]=pa->down;
                    }else {
                        colpre->down=pa->down;
                    }
                    free(pa);
                    a->tu--;
                }
            }else {//位置不存在，插入新结点
                OLNode *newNode=(OLNode*)malloc(sizeof(OLNode));
                newNode->i=i;
                newNode->j=j;
                newNode->e=e;
                //先插入行链表
                if (pre==NULL) {
                    newNode->right=a->rhead[i];
                    a->rhead[i]=newNode;
                }else {
                    newNode->right=pre->right;
                    pre->right=newNode;
                }
                //再插入列链表
                OLNode *colpre=NULL;
                OLNode *colp=a->chead[j];
                while (colp&&colp->i<i) {
                    colpre=colp;
                    colp=colp->down;
                }
                if (colpre==NULL) {
                    newNode->down=a->chead[j];
                    a->chead[j]=newNode;
                }else {
                    newNode->down=colpre->down;
                    colpre->down=newNode;
                }
                a->tu++;
            }
            pb=pb->right;
        }
    }
}
//从三元组表打印矩阵
void print_matrix(tripletable t) {
    int A[MAXRC][MAXRC]={0};
    for (int k=1;k<=t.t;k++) {
        A[t.data[k].i][t.data[k].j]=t.data[k].v;
    }
    for (int i=1;i<=t.m;i++) {
        if (i==1||i==t.m) {
            printf("+");
        }else {
            printf("|");
        }
        for (int j=1;j<=t.n;j++) {
            printf("%4d ",A[i][j]);
        }
        if (i==1||i==t.m) {
            printf("+\n");
        }else {
            printf("|\n");
        }
    }
}
//快速转置
void FastTranspose(tripletable a,tripletable *b) {
    b->m=a.n;
    b->n=a.m;
    b->t=a.t;
    if (b->t) {
        int num[MAXRC]={0};
        for (int k=1;k<=a.t;k++) {
            num[a.data[k].j]++;
            //求原矩阵每一列非零元个数，即结果矩阵每一行非零元个数
        }
        int cpot[MAXRC]={0};
        cpot[1]=1;
        for (int col=2;col<=a.n;col++) {
            cpot[col]=cpot[col-1]+num[col-1];
        }
        for (int p=1;p<=a.t;p++) {
            int col=a.data[p].j;
            int q=cpot[col];
            b->data[q].i=a.data[p].j;
            b->data[q].j=a.data[p].i;
            b->data[q].v=a.data[p].v;
            cpot[col]++;
        }
    }
}
//错误检查
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//主函数
int main() {
    int op=0;
    while (op!=5) {
        printf("Enter 1 for '+'\n");
        printf("Enter 2 for '-'\n");
        printf("Enter 3 for '*'\n");
        printf("Enter 4 for 'transpose'\n");
        printf("Enter 5 for EXIT\n");
        printf("Your option:");
        while (scanf("%d", &op) != 1 || (op!=1 && op!=2 && op!=3 && op!=4 && op!=5)) {
            printf("Invalid input. Please enter a valid op: ");
            clear_input_buffer();
        }
        clear_input_buffer();
        if(op==1) {//加法运算（十字链表）
            tripletable T1, T2;
            printf("Enter the first matrix:\n");
            input_matrix_tripletable(&T1);
            printf("Enter the second matrix:\n");
            input_matrix_tripletable(&T2);
            if (T1.m != T2.m || T1.n != T2.n) {
                printf("Error:the rows or columns don't match.\n");
                return 0;
            }
            Crosslist m1, m2;
            create_cross_list(&m1,T1);
            create_cross_list(&m2,T2);
            add(&m1,m2);
            tripletable res;
            cross_to_triple(m1,&res);
            printf("The add matrix is:\n");
            print_matrix(res);
            destroy_cross_list(&m1);
            destroy_cross_list(&m2);
        }else if(op==2) {//减法运算（十字链表）
            tripletable T1, T2;
            printf("Enter the first matrix:\n");
            input_matrix_tripletable(&T1);
            printf("Enter the second matrix:\n");
            input_matrix_tripletable(&T2);
            if (T1.m != T2.m || T1.n != T2.n) {
                printf("Error:the rows or columns don't match.\n");
                return 0;
            }
            Crosslist m1, m2;
            create_cross_list(&m1,T1);
            create_cross_list(&m2,T2);
            sub(&m1,m2);
            tripletable res;
            cross_to_triple(m1,&res);
            printf("The sub matrix is:\n");
            print_matrix(res);
            destroy_cross_list(&m1);
            destroy_cross_list(&m2);
        }else if(op==3) {//乘法运算（带行逻辑链接信息）
            tripletable T1, T2;
            printf("Enter the first matrix:\n");
            input_matrix_tripletable(&T1);
            printf("Enter the second matrix:\n");
            input_matrix_tripletable(&T2);
            if (T1.n!=T2.m) {
                printf("Error:the first columns don't match the second rows.\n");
                return 0;
            }
            rtripletable A,B,C;
            triple_to_rtriple(T1,&A);
            triple_to_rtriple(T2,&B);
            mul(A,B,&C);
            printf("The multiple matrix is:\n");
            print_Smatrix(C);
        }else if(op==4) {//快速转置（书本方法）
            tripletable T1, T2;
            printf("Enter the matrix:\n");
            input_matrix_tripletable(&T1);
            FastTranspose(T1,&T2);
            printf("The transpose matrix is:\n");
            print_matrix(T2);
        }else if(op==5) {
            return 0;
        }
    }
}
