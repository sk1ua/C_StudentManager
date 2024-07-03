//
// Created by 15857 on 24-7-3.
//
#include <stdio.h>
#include <stdlib.h>
#define N 10

typedef struct {
    char ID[50]; //ѧ��
    char name[50]; //����
    char major[50]; //רҵ
    int classNo; //�༶
    int score[3]; //���ſγɼ�
} STU;

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
    printf("ѧ��: %s\n",p->ID);
    printf("����: %s\n",p->name);
    printf("רҵ: %s\n",p->major);
    printf("�༶: %d\n",p->classNo);
    printf("��1: %d\n",p->score[0]);
    printf("��2: %d\n",p->score[1]);
    printf("��3: %d\n",p->score[2]);
    printf("-------------------------\n");
}

STU Fetch(int studentIndex) {
    FILE *file = fopen("studentInit.dat", "r");
    STU temp;
    if (file == NULL) {
        perror("�޷����ļ�");
        return temp;
    }
    for (int i = 0; i <= studentIndex; i++) {
        if (fscanf(file, "%s %s %s %d %d %d %d", temp.ID, temp.name, temp.major,
                   &temp.classNo, &temp.score[0], &temp.score[1], &temp.score[2])) {
            printf("��ȡѧ����Ϣʱ������ %d ��\n", i + 1);
            break;
                   }
    }
    fclose(file);
    return temp;
}

MaxIndices Max(STU *p, int scoreIndex) {
    MaxIndices result;
    result.count = 0; // ��ʼ������Ϊ0
    int maxScore = -1; // ��ʼ����߷�Ϊ-1

    // ��һ��ɨ�裺�ҵ���߷�
    for (int i = 0; i < studentCount; i++) {
        if (p[i].score[scoreIndex] > maxScore) {
            maxScore = p[i].score[scoreIndex];
        }
    }

    // �ڶ���ɨ�裺�����ж���ѧ���������߷�
    for (int i = 0; i < studentCount; i++) {
        if (p[i].score[scoreIndex] == maxScore) {
            result.count++;
        }
    }

    // ���ݼ�����̬�������������ڴ�
    result.indices = (int*)malloc(result.count * sizeof(int));
    if (result.indices == NULL) {
        perror("�ڴ����ʧ��");
        exit(1); // ����ڴ����ʧ�ܣ����˳�
    }

    // ������ɨ�裺�洢������߷�ѧ��������
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
            // Swap the students
            STU temp = p[i];
            p[i] = p[minIndex];
            p[minIndex] = temp;
        }
    }
}

void menu() {
    int choice, n, index;
    while (1) {
        printf("-------------------------\n");
        printf("ѧ������ϵͳ\n");
        printf("1. ���ļ�������ѧ����Ϣ\n");
        printf("2. ��ʾ����ѧ����Ϣ\n");
        printf("3. �����ȡĳ��ѧ����Ϣ\n");
        printf("4. ����ĳ���γ̵���߷�ѧ��\n");
        printf("5. ��ƽ��������\n");
        printf("6. �˳�\n");
        printf("-------------------------\n");
        printf("ѡ��һ��ѡ��: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("����ѧ������: ");
            scanf("%d", &n);
            Input(student, n);
            break;

            case 2:
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
                    for (int i = 0; i < maxIndices.count; i++) {
                        Output(&student[maxIndices.indices[i]]);
                    }
                } else {
                    printf("δ�ҵ���߷�ѧ��\n");
                }
                free(maxIndices.indices); // Free the dynamically allocated memory
            } else {
                printf("��Ч�Ŀγ����\n");
            }
            break;

            case 5:
                SortSelect(student);
                printf("��ƽ����������ѧ����Ϣ:\n");
                for (int i = 0; i < studentCount; i++) {
                    Output(&student[i]);
                }
            break;

            case 6:
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
