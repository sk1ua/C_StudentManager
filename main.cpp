//
// Created by 15857 on 24-7-3.
//
//
// Created by 15857 on 24-7-1.
//
#include <stdio.h>
#include <unistd.h>
#define N 10

typedef struct {
    char ID[50]; //学号
    char name[50]; //姓名
    char major[50]; //专业
    int classNo; //班级
    int score[3]; //三门课成绩
} STU;

STU student[N];
int studentCount = 0;

void Input(STU *p, int n) {
    FILE *file = fopen("studentInit.dat","r");
    if(file == NULL) {
        perror("无法打开文件");
        return;
    }
    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%s %s %s %d %d %d %d", p[i].ID, p[i].name, p[i].major,
                   &p[i].classNo, &p[i].score[0], &p[i].score[1], &p[i].score[2])) {
            studentCount++;
                   }
    }
    fclose(file);
}

void Output(STU *p) {
    printf("-------------------------\n");
    printf("学号: %s\n",p->ID);
    printf("姓名: %s\n",p->name);
    printf("专业: %s\n",p->major);
    printf("班级: %d\n",p->classNo);
    printf("课1: %d\n",p->score[0]);
    printf("课2: %d\n",p->score[1]);
    printf("课3: %d\n",p->score[2]);
    printf("-------------------------\n");
}

STU Fetch(int studentIndex) {
    FILE *file = fopen("studentInit.dat", "r");
    STU temp;
    if (file == NULL) {
        perror("无法打开文件");
        return temp;
    }
    for (int i = 0; i <= studentIndex; i++) {
        if (fscanf(file, "%s %s %s %d %d %d %d", temp.ID, temp.name, temp.major,
                   &temp.classNo, &temp.score[0], &temp.score[1], &temp.score[2])) {
            printf("读取学生信息时出错：第 %d 行\n", i + 1);
            break;
                   }
    }
    fclose(file);
    return temp;
}

void menu() {
    int choice, n, index;
    while (1) {
        printf("-------------------------\n");
        printf("学生管理系统\n");
        printf("1. 从文件中输入学生信息\n");
        printf("2. 显示所有学生信息\n");
        printf("3. 随机读取某个学生信息\n");
        printf("4. 退出\n");
        printf("-------------------------\n");
        printf("选择一个选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("输入学生数量: ");
            scanf("%d", &n);
            Input(student, n);
            break;

            case 2:
                for (int i = 0; i < studentCount; i++) {
                    Output(&student[i]);
                }
            break;

            case 3:
                printf("输入要读取的学生索引: ");
            scanf("%d", &index);
            if (index >= 0 && index < studentCount) {
                STU s = Fetch(index);
                Output(&s);
            } else {
                printf("无效的索引\n");
            }
            break;

            case 4:
                printf("退出系统\n");
            return;

            default:
                printf("无效选项\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
