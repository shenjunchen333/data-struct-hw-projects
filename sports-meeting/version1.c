#include <stdio.h>
//version 1.0
typedef struct{
    int school_id;
    int male_score;
    int female_score;
    int total_score;
}school;

typedef struct{
    int event_id;
    int school_id[5];
    int score[10];
    int number;//获奖人数
    char name[10][20];
}event;

void initialization_event(event events[],int n) {
    for (int i=0;i<n;i++) {
        events[i].number=0;
    }
}

void initialization_school(school schools[],int school_number) {
    for (int i=0;i<school_number;i++) {
        schools[i].school_id=i+1;
        schools[i].male_score=0;
        schools[i].female_score=0;
        schools[i].total_score=0;
    }
}

int score(int event_id,int rank) {
    if (event_id%2==1) {
        switch (rank) {
            case 0:return 7;
            case 1:return 5;
            case 2:return 3;
            case 3:return 2;
            case 4:return 1;
            default:return 0;
        }
    }else {
        switch (rank) {
            case 0:return 5;
            case 1:return 3;
            case 2:return 2;
            default:return 0;
        }
    }
}

void input_events(event *e,school schools[],int school_number,int male_event_number) {
    //录入event信息
    int id;
    printf("Please enter the event ID:");
    scanf("%d",&id);
    e->event_id=id;
    int num=(id%2==1)?5:3;
    e->number=num;
    for (int i=0;i<num;i++) {
        printf("Please enter the name of rank %d:",i+1);
        scanf("%s",e->name[i]);
        printf("Please enter the school ID of rank %d:",i+1);
        scanf("%d",&e->school_id[i]);

        e->score[i]=score(id,i);
        int score=e->score[i];
        //根据event信息录入school信息
        int index=e->school_id[i]-1;
        if (index>=0&&index<school_number) {
            if (id>=1&&id<=male_event_number) {
                schools[index].male_score+=score;
            }else if (id>male_event_number) {
                schools[index].female_score+=score;
            }else {
                printf("Please enter a valid event ID");
            }
            schools[index].total_score+=score;
        }else {
            printf("Please enter a valid school ID");
        }
    }
}

void print_school_scores(school schools[],int school_number,event events[],int event_number) {
    printf("\n===============================================================\n");
    printf("                      School Reports\n");
    printf("===============================================================\n\n");
    for (int i=0;i<school_number;i++) {
        printf("School ID: %d\n",schools[i].school_id);
        printf("=================================\n");
        printf("| Event | Rank |  Name   | Score |\n");
        printf("=================================\n");
        int found=0;
        for (int j=0;j<event_number;j++) {
            for (int k=0;k<events[j].number;k++) {
                if (events[j].school_id[k]==schools[i].school_id) {
                    printf("|   %d   |  %d   |  %-6s |   %d   |\n",events[j].event_id,k+1,events[j].name[k],events[j].score[k]);
                    found=1;
                }
            }
        }
        if (!found) {
            printf("|           No Awards           |\n");
        }
        printf("=================================\n");
        printf("Total Scores: %d\n\n", schools[i].total_score);
    }
}

void print_total_score_report(school schools[],int school_number) {
    printf("\n===============================================================\n");
    printf("                    Total Score Reports\n");
    printf("===============================================================\n\n");
    printf("=================================\n");
    printf("| School | Male | Female | Total |\n");
    printf("=================================\n");
    for (int i=0;i<school_number;i++) {
        printf("|   %d   |  %2d  |  %4d  |   %d   |\n",schools[i].school_id,schools[i].male_score,schools[i].female_score,schools[i].total_score);
    }
    printf("=================================\n");
}

//（奇数）有些项目取前五名，得分顺序为 7，5，3，2，1；（偶数）还有些项目只取前三名，得分顺序为 5，3，2。

int main() {
//输入参赛学校数量
    int school_number=0;
    printf("Please enter the number of schools:");
    scanf("%d",&school_number);
//输入男子项目数量
    int male_event=0;
    printf("Please enter the number of the male events:");
    scanf("%d",&male_event);
//输入女子项目数量
    int female_event=0;
    printf("Please enter the number of the female events:");
    scanf("%d",&female_event);

    int total_event=male_event+female_event;
//初始化school数组分数
    school schools[school_number];
    initialization_school(schools,school_number);
//初始化event数组（总=男＋女）
    event events[total_event];
    initialization_event(events,total_event);
//输入当前赛事的编号
//输入第x名的姓名
//输入第x名的学校
//输入第x+1名的姓名
//输入第x+1名的学校
//。。。。。。
    for (int i=0;i<total_event;i++) {
        printf("Please input data for event%d\n",i+1);
        input_events(&events[i],schools,school_number,male_event);
    }

//打印各个学校的成绩单
    print_school_scores(schools,school_number,events,total_event);
//打印总成绩单
    print_total_score_report(schools,school_number);
    return 0;
}
