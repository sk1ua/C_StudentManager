//
// Created by 15857 on 24-7-3.
//
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"

#define N 100

typedef struct {
    int* indices; // 用于保存最高分学生索引的数组
    int count;    // 最高分学生的数量
} MaxIndices;

STU student[N];
int studentCount = 0;

void Input(STU *p, int n) {
    FILE *file = fopen("studentInit.dat","r");
    if(file == NULL) {
        perror("无法打开文件");
        return;
    }
    if(n > N) {
        printf("输入的学生数量过大\n");
        fclose(file);
        return;
    }
    for (int i = 0; i < n; i++) {
        if (fread(&p[i], sizeof(STU), 1, file) != 1) {
            printf("读取学生信息时出错\n");
            fclose(file);
            return;
        }
        studentCount++;
    }
    printf("录入学生信息成功\n");
    fclose(file);
}

void InputToFile(const char* filename, STU* students, int numStudents) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    size_t written = fwrite(students, sizeof(STU), numStudents, file);
    if (written != numStudents) {
        perror("Failed to write all student data to file");
    }

    fclose(file);
}

void Output(STU *p) {
    //printf("|------------|----------------------|----------------------|--------|--------|--------|--------|\n");
    printf("| %-10s | %-20s | %-20s | %-6d | %-6d | %-6d | %-6d |\n", p->ID, p->name, p->major, p->classNo, p->score[0], p->score[1], p->score[2]);
    //printf("|------------|----------------------|----------------------|--------|--------|--------|--------|\n");
}
void Outputtitle() {
    //printf("|------------|----------------------|----------------------|--------|--------|--------|--------|\n");
    printf("|     ID     |         Name         |        Major         | Class  | Score1 | Score2 | Score3 |\n");
    //printf("|------------|----------------------|----------------------|--------|--------|--------|--------|\n");
}


STU Fetch(int studentIndex) {
    FILE *file = fopen("studentInit.dat", "r");
    STU temp = {0};
    if (file == NULL) {
        perror("无法打开文件");
        return temp;
    }
    if (fseek(file, studentIndex * sizeof(STU), SEEK_SET) != 0) {
        perror("无法定位到文件位置");
        fclose(file);
        return temp;
    }
    if (fread(&temp, sizeof(STU), 1, file) != 1) {
        perror("未找到指定索引的学生信息");
        temp = (STU){0};
    }
    fclose(file);
    return temp;
}


MaxIndices Max(STU *p, int scoreIndex) {
    MaxIndices result;
    result.count = 0;
    int maxScore = -1;

    for (int i = 0; i < studentCount; i++) {
        if (p[i].score[scoreIndex] > maxScore) {
            maxScore = p[i].score[scoreIndex];
        }
    }

    for (int i = 0; i < studentCount; i++) {
        if (p[i].score[scoreIndex] == maxScore) {
            result.count++;
        }
    }

    result.indices = (int*)malloc(result.count * sizeof(int));
    if (result.indices == NULL) {
        perror("内存分配失败");
        exit(1);
    }

    int j = 0;
    for (int i = 0; i < studentCount; i++) {
        if (p[i].score[scoreIndex] == maxScore) {
            result.indices[j++] = i;
        }
    }

    return result;
}

void SortSelect(STU *p) {
    for (int i = 0; i < studentCount - 1; i++) {
        int minIndex = i;
        float minAvgScore = (p[i].score[0] + p[i].score[1] + p[i].score[2]) / 3.0;

        for (int j = i + 1; j < studentCount; j++) {
            float avgScoreJ = (p[j].score[0] + p[j].score[1] + p[j].score[2]) / 3.0;
            if (avgScoreJ < minAvgScore) {
                minIndex = j;
                minAvgScore = avgScoreJ;
            }
        }

        if (minIndex != i) {
            STU temp = p[i];
            p[i] = p[minIndex];
            p[minIndex] = temp;
        }
    }
}

void SortWithClass(STU *p) {
    printf("请输入班级序号(1-3): ");
    int classNo;
    int read = scanf("%d", &classNo);
    if (read != 1) {
        printf("输入错误，请重新输入\n");
        while (getchar() != '\n');
        return;
    }
    if (classNo < 1 || classNo > 3) {
        printf("无效的班级序号\n");
        return;
    }

    int classStudentCount = 0;
    for (int i = 0; i < studentCount; i++) {
        if (p[i].classNo == classNo) {
            classStudentCount++;
        }
    }

    STU classStudents[classStudentCount];
    int index = 0;
    for (int i = 0; i < studentCount; i++) {
        if (p[i].classNo == classNo) {
            classStudents[index++] = p[i];
        }
    }

    for (int i = 0; i < classStudentCount - 1; i++) {
        for (int j = 0; j < classStudentCount - i - 1; j++) {
            int totalScoreJ = classStudents[j].score[0] + classStudents[j].score[1] + classStudents[j].score[2];
            int totalScoreJ1 = classStudents[j + 1].score[0] + classStudents[j + 1].score[1] + classStudents[j + 1].score[2];
            if (totalScoreJ > totalScoreJ1) {
                STU temp = classStudents[j];
                classStudents[j] = classStudents[j + 1];
                classStudents[j + 1] = temp;
            }
        }
    }

    Outputtitle();
    for (int i = 0; i < classStudentCount; i++) {
        Output(&classStudents[i]);
    }
}
void SortWithMajor(STU * p) {
    printf("请输入专业名称(ComputerScience/Software/Network): ");
    char major[50];
    int read = scanf("%s", major);
    if (read != 1) {
        printf("输入错误，请重新输入\n");
        while (getchar() != '\n');
        return;
    }
    if (strcmp(major, "ComputerScience") != 0 && strcmp(major, "Software") != 0 && strcmp(major, "Network") != 0) {
        printf("无效的专业名称\n");
        return;
    }
    int majorStudentCount = 0;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(p[i].major, major) == 0) {
            majorStudentCount++;
        }
    }

    STU majorStudents[majorStudentCount];
    int index = 0;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(p[i].major, major) == 0) {
            majorStudents[index++] = p[i];
        }
    }

    for (int i = 0; i < majorStudentCount - 1; i++) {
        for (int j = 0; j < majorStudentCount - i - 1; j++) {
            int totalScoreJ = majorStudents[j].score[0] + majorStudents[j].score[1] + majorStudents[j].score[2];
            int totalScoreJ1 = majorStudents[j + 1].score[0] + majorStudents[j + 1].score[1] + majorStudents[j + 1].score[2];
            if (totalScoreJ > totalScoreJ1) {
                STU temp = majorStudents[j];
                majorStudents[j] = majorStudents[j + 1];
                majorStudents[j + 1] = temp;
            }
        }
    }

    Outputtitle();
    for (int i = 0; i < majorStudentCount; i++) {
        Output(&majorStudents[i]);
    }

}


void menu() {
    int choice, n, index;
    Input(student, 10);
    while (1) {
        printf("--------学生管理系统菜单--------\n");
        printf("1. 从文件中输入学生信息\n");
        printf("2. 显示所有学生信息\n");
        printf("3. 查看指定学生信息\n");
        printf("4. 返回某个课程的最高分学生\n");
        printf("5. 按平均分排序\n");
        printf("6. 按特定条件查询\n");
        printf("7. 退出系统\n");
        printf("----------------------------\n");
        printf("选择一个选项: ");
        int read = scanf("%d", &choice);
        if (read != 1) {
            printf("输入错误，请重新输入\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1:
                printf("输入学生数量: ");
            scanf("%d", &n);
            for(int i = 0; i < n; i++) {
                printf("输入学生信息: ");
                printf("学号: ");
                scanf("%s", student[studentCount].ID);
                printf("姓名: ");
                scanf("%s", student[studentCount].name);
                printf("专业: ");
                scanf("%s", student[studentCount].major);
                printf("班级: ");
                scanf("%d", &student[studentCount].classNo);
                printf("课程1成绩: ");
                scanf("%d", &student[studentCount].score[0]);
                printf("课程2成绩: ");
                scanf("%d", &student[studentCount].score[1]);
                printf("课程3成绩: ");
                scanf("%d", &student[studentCount].score[2]);
                studentCount++;
            }
            InputToFile("studentInit.dat", student, studentCount);
            printf("录入学生信息成功\n");
            break;

            case 2:
                Outputtitle();
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
                printf("输入课程序号: (1-3)");
            scanf("%d", &index);
            index--;
            if (index >= 0 && index < 3) {
                MaxIndices maxIndices = Max(student, index);
                if (maxIndices.count > 0) {
                    Outputtitle();
                    for (int i = 0; i < maxIndices.count; i++) {
                        Output(&student[maxIndices.indices[i]]);
                    }
                } else {
                    printf("未找到最高分学生\n");
                }
                free(maxIndices.indices);
            } else {
                printf("无效的课程序号\n");
            }
            break;

            case 5:
                int sortChoice;
            printf("1.所有人 2.班级 3.专业\n");
            printf("选择一个选项: ");
            read = scanf("%d", &sortChoice);
            if (read != 1) {
                printf("输入错误，请重新输入\n");
                while (getchar() != '\n');
                continue;
            }
            switch (sortChoice) {
                case 1:
                    SortSelect(student);
                printf("按平均分排序后的学生信息:\n");
                for (int i = 0; i < studentCount; i++) {
                    Output(&student[i]);
                }
                break;
                case 2:
                    SortWithClass(student);
                break;
                case 3:
                    SortWithMajor(student);
                    break;
                default:
                    printf("无效选项\n");
            }
            break;
            case 6: {
                printf("按特定条件查询\n");
                printf("1.按班级查询 2.按分数段查询\n");
                printf("选择一个选项: ");
                read = scanf("%d", &sortChoice);
                if (read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                switch (sortChoice) {
                    case 1:
                        SortWithClass(student);
                    break;
                    case 2: {
                        printf("输入总分分数段(1-300): ");
                        int totalScore;
                        read = scanf("%d", &totalScore);
                        if (read != 1) {
                            printf("输入错误，请重新输入\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        if (totalScore < 1 || totalScore > 300) {
                            printf("无效的分数段\n");
                            continue;
                        }
                        int count = 0;
                        Outputtitle();
                        for (int i = 0; i < studentCount; i++) {
                            int total = student[i].score[0] + student[i].score[1] + student[i].score[2];
                            if (total >= totalScore) {
                                Output(&student[i]);
                                count++;
                            }
                        }
                        printf("共找到%d个学生\n", count);

                    }
                    break;
                    default:
                        printf("无效选项\n");
                }
            }
            break;

            case 7:
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
