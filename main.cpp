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
    int* indices; // ���ڱ�����߷�ѧ������������
    int count;    // ��߷�ѧ��������
} MaxIndices;

STU student[N];
int studentCount = 0;

void Input(STU *p, int n) {
    FILE *file = fopen("studentInit.dat","r");
    if(file == NULL) {
        perror("�޷����ļ�");
        return;
    }
    if(n > N) {
        printf("�����ѧ����������\n");
        fclose(file);
        return;
    }
    for (int i = 0; i < n; i++) {
        if (fread(&p[i], sizeof(STU), 1, file) != 1) {
            printf("��ȡѧ����Ϣʱ����\n");
            fclose(file);
            return;
        }
        studentCount++;
    }
    printf("¼��ѧ����Ϣ�ɹ�\n");
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
        perror("�޷����ļ�");
        return temp;
    }
    if (fseek(file, studentIndex * sizeof(STU), SEEK_SET) != 0) {
        perror("�޷���λ���ļ�λ��");
        fclose(file);
        return temp;
    }
    if (fread(&temp, sizeof(STU), 1, file) != 1) {
        perror("δ�ҵ�ָ��������ѧ����Ϣ");
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
        perror("�ڴ����ʧ��");
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
    printf("������༶���(1-3): ");
    int classNo;
    int read = scanf("%d", &classNo);
    if (read != 1) {
        printf("�����������������\n");
        while (getchar() != '\n');
        return;
    }
    if (classNo < 1 || classNo > 3) {
        printf("��Ч�İ༶���\n");
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
    printf("������רҵ����(ComputerScience/Software/Network): ");
    char major[50];
    int read = scanf("%s", major);
    if (read != 1) {
        printf("�����������������\n");
        while (getchar() != '\n');
        return;
    }
    if (strcmp(major, "ComputerScience") != 0 && strcmp(major, "Software") != 0 && strcmp(major, "Network") != 0) {
        printf("��Ч��רҵ����\n");
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
        printf("--------ѧ������ϵͳ�˵�--------\n");
        printf("1. ���ļ�������ѧ����Ϣ\n");
        printf("2. ��ʾ����ѧ����Ϣ\n");
        printf("3. �鿴ָ��ѧ����Ϣ\n");
        printf("4. ����ĳ���γ̵���߷�ѧ��\n");
        printf("5. ��ƽ��������\n");
        printf("6. ���ض�������ѯ\n");
        printf("7. �˳�ϵͳ\n");
        printf("----------------------------\n");
        printf("ѡ��һ��ѡ��: ");
        int read = scanf("%d", &choice);
        if (read != 1) {
            printf("�����������������\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1:
                printf("����ѧ������: ");
            scanf("%d", &n);
            for(int i = 0; i < n; i++) {
                printf("����ѧ����Ϣ: ");
                printf("ѧ��: ");
                scanf("%s", student[studentCount].ID);
                printf("����: ");
                scanf("%s", student[studentCount].name);
                printf("רҵ: ");
                scanf("%s", student[studentCount].major);
                printf("�༶: ");
                scanf("%d", &student[studentCount].classNo);
                printf("�γ�1�ɼ�: ");
                scanf("%d", &student[studentCount].score[0]);
                printf("�γ�2�ɼ�: ");
                scanf("%d", &student[studentCount].score[1]);
                printf("�γ�3�ɼ�: ");
                scanf("%d", &student[studentCount].score[2]);
                studentCount++;
            }
            InputToFile("studentInit.dat", student, studentCount);
            printf("¼��ѧ����Ϣ�ɹ�\n");
            break;

            case 2:
                Outputtitle();
                for (int i = 0; i < studentCount; i++) {
                    Output(&student[i]);
                }
            break;

            case 3:
                printf("����Ҫ��ȡ��ѧ������: ");
            scanf("%d", &index);
            if (index >= 0 && index < studentCount) {
                STU s = Fetch(index);
                Output(&s);
            } else {
                printf("��Ч������\n");
            }
            break;

            case 4:
                printf("����γ����: (1-3)");
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
                    printf("δ�ҵ���߷�ѧ��\n");
                }
                free(maxIndices.indices);
            } else {
                printf("��Ч�Ŀγ����\n");
            }
            break;

            case 5:
                int sortChoice;
            printf("1.������ 2.�༶ 3.רҵ\n");
            printf("ѡ��һ��ѡ��: ");
            read = scanf("%d", &sortChoice);
            if (read != 1) {
                printf("�����������������\n");
                while (getchar() != '\n');
                continue;
            }
            switch (sortChoice) {
                case 1:
                    SortSelect(student);
                printf("��ƽ����������ѧ����Ϣ:\n");
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
                    printf("��Чѡ��\n");
            }
            break;
            case 6: {
                printf("���ض�������ѯ\n");
                printf("1.���༶��ѯ 2.�������β�ѯ\n");
                printf("ѡ��һ��ѡ��: ");
                read = scanf("%d", &sortChoice);
                if (read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                switch (sortChoice) {
                    case 1:
                        SortWithClass(student);
                    break;
                    case 2: {
                        printf("�����ַܷ�����(1-300): ");
                        int totalScore;
                        read = scanf("%d", &totalScore);
                        if (read != 1) {
                            printf("�����������������\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        if (totalScore < 1 || totalScore > 300) {
                            printf("��Ч�ķ�����\n");
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
                        printf("���ҵ�%d��ѧ��\n", count);

                    }
                    break;
                    default:
                        printf("��Чѡ��\n");
                }
            }
            break;

            case 7:
                printf("�˳�ϵͳ\n");
            return;

            default:
                printf("��Чѡ��\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
