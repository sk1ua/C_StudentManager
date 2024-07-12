//
// Created by Sk1ua on 24-7-3.
//

// 包含必要的头文件
#include <algorithm>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 包含学生结构体定义的头文件
#include "student.h"

#define N 100  // 定义最大学生数量

// 定义用于保存最高分学生索引的结构体
typedef struct {
    int* indices; // 用于保存最高分学生索引的数组
    int count;    // 最高分学生的数量
} MaxIndices;

STU student[N]; // 学生数组
int studentCount = 0; // 当前学生数量
char NowFileName[255] = "StudentInit.dat"; // 当前文件名

// 从文件中读入学生信息
void Input(const char* filename, STU *p, int maxStudents) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("无法打开文件");
        return;
    }

    int count = 0;
    while (count < maxStudents) {
        if (fread(&p[count], sizeof(STU), 1, file) != 1) {
            break;
        }
        count++;
    }

    studentCount += count;
    printf("成功读入 %d 个学生信息\n", count);

    fclose(file);
}

// 将学生信息写入文件
void InputToFile(const char* filename, STU* students, int numStudents) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("无法打开文件");
        return;
    }

    size_t written = fwrite(students, sizeof(STU), numStudents, file);
    if (written != numStudents) {
        perror("写入文件时出错");
    }

    fclose(file);
}

// 标准输出格式
void Output(STU *p) {
    printf("| %-10s | %-20s | %-20s | %-6d | %-6d | %-6d | %-6d | %-6d |\n",
           p->ID, p->name, p->major, p->classNo, p->score[0], p->score[1], p->score[2],
           p->score[0] + p->score[1] + p->score[2]);
}

// 标准输出表头
void Outputtitle() {
    printf("|     ID     |         Name         |        Major         | Class  | Score1 | Score2 | Score3 | Total  |\n");
}

// 根据索引返回学生信息
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
// 返回某科最大成绩学生
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

    int j = 0;
    for (int i = 0; i < studentCount; i++) {
        if (p[i].score[scoreIndex] == maxScore) {
            result.indices[j++] = i;
        }
    }

    return result;
}
// 查询，班级，成绩
void Search(STU *p, int classNo, char s, int scoreSum) {
    int cnt = 0;
    for (int i = 0; i < studentCount; i++) {
        int totalScore = p[i].score[0] + p[i].score[1] + p[i].score[2];
        if (p[i].classNo == classNo && ((s == '>' && totalScore > scoreSum) || (s == '<' && totalScore < scoreSum))) {
            Output(&p[i]);
        }
    }
    printf("查找到%d个学生",cnt);
}

// 根据ID删除学生信息
void DeleteStudent(char* deleteID) {
    int tempCount = 0;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(student[i].ID, deleteID) != 0) {
            student[tempCount++] = student[i];
        }
    }
    if (tempCount != studentCount) {
        studentCount = tempCount;
        printf("学号为 %s 的学生信息已删除。\n", deleteID);
    } else {
        printf("未找到学号为 %s 的学生。\n", deleteID);
    }
}

// 选择排序，全体平均成绩排序
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

// 冒泡排序，班级平均成绩排序
void Sort_buble(STU *p, int n, int ClassNo) {
    STU stu_class_ave[N];
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (p[i].classNo == ClassNo) {
            stu_class_ave[count++] = p[i];
        }
    }

    float averages[count];
    for (int i = 0; i < count; i++) {
        averages[i] = (stu_class_ave[i].score[0] + stu_class_ave[i].score[1] + stu_class_ave[i].score[2]) / 3.0;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (averages[j] < averages[j + 1]) {
                float tempAvg = averages[j];
                averages[j] = averages[j + 1];
                averages[j + 1] = tempAvg;

                STU tempStu = stu_class_ave[j];
                stu_class_ave[j] = stu_class_ave[j + 1];
                stu_class_ave[j + 1] = tempStu;
            }
        }
    }
    Outputtitle();
    for (int i = 0; i < count; i++) {
        Output(&stu_class_ave[i]);
    }
    printf("查找到%d个学生\n",count);
    char filename[50];
    printf("请输入你想存入的文件名:");
    int read = scanf("%s",filename);
    if (read != 1) {
        printf("输入错误，请重新输入\n");
        while (getchar() != '\n');
        return;
    }
    InputToFile(filename,stu_class_ave,count);
    printf("排序后信息成功存入%s\n",filename);
}

// 插入排序，根据专业和课程排序
void Sort_insert(STU *p, int n, char *major, int subjectIndex) {
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(p[i].major, major) == 0) {
            count++;
        }
    }

    STU* stu_major_subject = (STU*)malloc(count * sizeof(STU));

    int index = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(p[i].major, major) == 0) {
            stu_major_subject[index++] = p[i];
        }
    }

    for (int i = 1; i < count; i++) {
        STU temp = stu_major_subject[i];
        int j = i - 1;
        while (j >= 0 && stu_major_subject[j].score[subjectIndex] > temp.score[subjectIndex]) {
            stu_major_subject[j + 1] = stu_major_subject[j];
            j--;
        }
        stu_major_subject[j + 1] = temp;
    }
    Outputtitle();
    for (int i = 0; i < count; i++) {
        Output(&stu_major_subject[i]);
    }
    printf("查找到%d个学生\n",count);
    char filename[50];
    printf("请输入你想存入的文件名:");
    int read = scanf("%s",filename);
    if (read != 1) {
        printf("输入错误，请重新输入\n");
        while (getchar() != '\n');
        return;
    }
    InputToFile(filename,stu_major_subject,count);
    printf("排序后信息成功存入%s\n",filename);
}

// 根据ID字典序排序
void SortByID(STU *students, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(students[j].ID, students[j + 1].ID) > 0) {
                STU temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

// 录入学生信息
void Save(STU *p, int n) {
    for (int i = 0; i < n; i++) {
        printf("输入学生信息: \n");
        printf("学号: ");
        scanf("%s", p[studentCount].ID);
        getchar();
        printf("姓名: ");
        scanf("%s", p[studentCount].name);
        getchar();
        printf("专业: ");
        scanf("%s", p[studentCount].major);
        getchar();
        printf("班级: ");
        scanf("%d", &p[studentCount].classNo);
        getchar();
        printf("课程1成绩: ");
        scanf("%d", &p[studentCount].score[0]);
        getchar();
        printf("课程2成绩: ");
        scanf("%d", &p[studentCount].score[1]);
        getchar();
        printf("课程3成绩: ");
        scanf("%d", &p[studentCount].score[2]);
        getchar();
        studentCount++;
    }
    // 写入文件
    InputToFile("studentInit.dat", p, studentCount);
    printf("录入学生信息成功\n");
    SortByID(p, studentCount);
}

// 主菜单函数
void menu() {
    int choice, n, index;
    printf("已默认打开\"studentInit.dat\"文件\n");
    Input("studentInit.dat",student, 10);
    SortByID(student, 10);
    while (1) {
        printf("--------学生管理系统菜单--------\n");
        printf("1. 将新的学生信息输入文件中\n");
        printf("2. 从文件中删除学生信息\n");
        printf("3. 显示所有学生信息\n");
        printf("4. 指定学生信息查找(姓名,学号)\n");
        printf("5. 查找某个课程的最高分学生\n");
        printf("6. 排序(所有人平均分排序/按班级排序/按专业,按某门课排序)\n");
        printf("7. 按特定条件查询(班级,分数段)\n");
        printf("8. 清空当前系统中存入的学生信息文件\n");
        printf("9. 读入新的学生文件信息\n");
        printf("10.退出系统\n");
        printf("----------------------------\n");
        if(studentCount)printf("现在打开的是\"%s\"文件\n",NowFileName);
        else printf("未打开学生信息文件\n");
        printf("当前已经录入%d个学生的信息.\n", studentCount);
        printf("选择一个选项: ");
        int read = scanf("%d", &choice);
        if (read != 1) {
            printf("输入错误，请重新输入\n");
            while (getchar() != '\n');
            continue;
        }

        // 根据用户选择执行相应功能
        switch (choice) {
            case 1: {
                printf("输入学生数量: ");
                scanf("%d", &n);
                Save(student,n);
                system("pause");
                system("cls");
            }
            break;
            case 2: {
                char deleteID[50];
                printf("请输入要删除的学生学号: ");
                scanf("%s", deleteID);
                DeleteStudent(deleteID);
                system("pause");
                system("cls");
            }
            break;
            case 3:
                Outputtitle();
                for (int i = 0; i < studentCount; i++) {
                    Output(&student[i]);
                }
                system("pause");
                system("cls");
            break;

            case 4: {
                int queryType;
                char query[50];
                printf("选择查询类型：\n");
                printf("1. 根据学号查询(example: IDXXX)\n");
                printf("2. 根据姓名查询(example: Jack)\n");
                printf("选择一个选项: ");
                int read = scanf("%d", &queryType);
                if (read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (queryType != 1 && queryType != 2) {
                    printf("无效的查询类型\n");
                    continue;
                }

                printf("输入查询内容: ");
                scanf("%s", query);

                int found = 0;
                int j = 0;
                for (int i = 0; i < studentCount; i++) {
                    if ((queryType == 1 && strcmp(student[i].ID, query) == 0) ||
                        (queryType == 2 && strcmp(student[i].name, query) == 0)) {
                        if (!found) {
                            Outputtitle();
                        }
                        j++;
                        Output(&student[i]);
                        found = 1;
                        break;
                    }
                }
                printf("查找到%d个学生\n", j);
                if (!found) {
                    printf("未找到匹配的学生信息\n");
                }
                system("pause");
                system("cls");
            }
            break;

            case 5:
                printf("输入课程序号: (1-3)");
            scanf("%d", &index);
            index--;
            if (index >= 0 && index < 3) {6

                MaxIndices maxIndices = Max(student, index);
                if (maxIndices.count > 0) {
                    Outputtitle();
                    for (int i = 0; i < maxIndices.count; i++) {
                        Output(&student[maxIndices.indices[i]]);
                    }
                    printf("查找到%d名课程序号%d的最高分学生\n", maxIndices.count, index + 1);
                } else {
                    printf("未找到最高分学生\n");
                }
                free(maxIndices.indices);
            } else {
                printf("无效的课程序号\n");
            }
            system("pause");
            system("cls");
            break;

            case 6:
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
                case 1: {
                    SortSelect(student);
                    printf("按平均分排序后的学生信息:\n");
                    Outputtitle();
                    for (int i = 0; i < studentCount; i++) {
                        Output(&student[i]);
                    }
                    char filename[50];
                    printf("请输入你想存入的文件名:");
                    int read = scanf("%s",filename);
                    if (read != 1) {
                        printf("输入错误，请重新输入\n");
                        while (getchar() != '\n');
                        return;
                    }
                    InputToFile(filename,student,studentCount);
                    printf("排序后信息成功存入%s\n",filename);
                    break;
                }
                case 2: {
                    int classNo;
                    printf("输入班级号(1-3): ");
                    scanf("%d", &classNo);
                    Sort_buble(student, studentCount, classNo);
                    break;
                }
                case 3: {
                    char major[50];
                    printf("1.ComputerScinece 2.Software 3.NetWork\n");
                    printf("输入专业(请输入具体专业名称):");
                    int read = scanf("%s", major);
                    getchar();
                    if (read != 1) {
                        printf("输入错误，请重新输入\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    if (strcmp(major, "ComputerScience") != 0 && strcmp(major, "Software") != 0 && strcmp(major, "NetWork") != 0) {
                        printf("无效的专业名称\n");
                        return;
                    }
                    printf("输入课程序号(1-3): ");
                    int subjectIndex;
                    read = scanf("%d", &subjectIndex);
                    if (read != 1) {
                        printf("输入错误，请重新输入\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    subjectIndex--;
                    if (subjectIndex >= 0 && subjectIndex < 3) {
                        Sort_insert(student, studentCount, major, subjectIndex);
                    } else {
                        printf("无效的课程序号\n");
                    }
                }
                break;
                default:
                    printf("无效选项\n");
                system("pause");
                system("cls");
            }
            break;

            case 7: {
                printf("按特定条件查询\n");
                printf("请输入班级序号(1-3): ");
                int classNo;
                read = scanf("%d", &classNo);
                if (read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (classNo < 1 || classNo > 3) {
                    printf("无效的班级序号\n");
                    continue;
                }
                getchar();
                printf("输入查询条件(> 或 <): ");
                char s;
                read = scanf("%c", &s);
                if (read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (s != '>' && s != '<') {
                    printf("无效的查询条件\n");
                    continue;
                }
                printf("输入分数(0-300): ");
                int scoreSum;
                read = scanf("%d", &scoreSum);
                if (read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                if(scoreSum < 0 || scoreSum > 300) {
                    printf("无效的分数\n");
                    continue;
                }
                Outputtitle();
                Search(student, classNo, s, scoreSum);
                system("pause");
                system("cls");
            }
            break;
            case 8:
                studentCount = 0;
            printf("当前内存中的学生信息已清空。\n");
            system("pause");
            system("cls");
            break;

            case 9: {
                char newFilename[256];
                printf("请输入新的学生信息文件名: ");
                read =  scanf("%255s", newFilename);
                if (read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    return;
                }
                strcpy(NowFileName, newFilename);
                Input(newFilename, student, N);
                system("pause");
                system("cls");
            }
                break;
            case 10:
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
