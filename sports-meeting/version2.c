//
// Created by 蕖之昼 on 2026/3/13.
//
#include <stdio.h>
//version 2.0
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

int score(int type,int rank) {
    if (type%2==1) {
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

void input_events(event *e,school schools[],int school_number,int male_event_number,int event_id) {
    //录入event信息
    int type=0;
    printf("event type 1 means get top5\n");
    printf("event type 2 means get top3\n");
    while (type==0) {
        printf("Please enter the event type (1 or 2):");
        scanf("%d",&type);
        if (type!=1&&type!=2) {
            type=0;
            printf("Please enter a valid type!\n\n");
        }
    }
    e->event_id=event_id;
    int num=(type%2==1)?5:3;
    e->number=num;
    for (int i=0;i<num;i++) {
        printf("Please enter the name of rank %d:",i+1);
        scanf("%s",e->name[i]);
        printf("Please enter the school ID of rank %d:",i+1);
        scanf("%d",&e->school_id[i]);
        e->score[i]=score(type,i);
        int score=e->score[i];
        //根据event信息录入school信息
        int index=e->school_id[i]-1;
        if (index>=0&&index<school_number) {
            if (event_id>=1&&event_id<=male_event_number) {
                schools[index].male_score+=score;
            }else if (event_id>male_event_number) {
                schools[index].female_score+=score;
            }else {
                printf("Please enter a valid event ID\n");
            }
            schools[index].total_score+=score;
        }else {
            printf("Please enter a valid school ID\n");
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

void print_single_school_score(school schools[],int school_number,event events[],int event_number,int school_id) {
    printf("School ID: %d\n",schools[school_id].school_id);
    printf("=================================\n");
    printf("| Event | Rank |  Name   | Score |\n");
    printf("=================================\n");
    int found=0;
    for (int j=0;j<event_number;j++) {
        for (int k=0;k<events[j].number;k++) {
            if (events[j].school_id[k]==schools[school_id].school_id) {
                printf("|   %d   |  %d   |  %-6s |   %d   |\n",events[j].event_id,k+1,events[j].name[k],events[j].score[k]);
                found=1;
            }
        }
    }
    if (!found) {
        printf("|           No Awards           |\n");
    }
    printf("=================================\n");
    printf("Total Scores: %d\n\n", schools[school_id].total_score);
}
//有些项目取前五名，得分顺序为 7，5，3，2，1；还有些项目只取前三名，得分顺序为 5，3，2。
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
//录入赛事信息
    for (int i=0;i<total_event;i++) {
        printf("Please input data for event%d\n",i+1);
        int event_id=i+1;
        input_events(&events[i],schools,school_number,male_event,event_id);
    }
//选择打印成绩单
    while (1) {
        int op=0;
        printf("Enter 0 to print all reports\n");
        printf("Enter x to print the report of school x\n");
        printf("Enter -1 to END PRINTING\n");
        printf("Please enter your option:");
        scanf("%d",&op);
        if (op==-1) {
            return 0;
        }else if (op==0) {
            print_school_scores(schools,school_number,events,total_event);
            print_total_score_report(schools,school_number);
        }else if (op>0&&op<=school_number){
            print_single_school_score(schools,school_number,events,total_event,op-1);
        }else {
            printf("Please input a valid option\n");
        }
    }
}
