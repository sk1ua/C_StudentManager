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
    char ID[50]; //ѧ��
    char name[50]; //����
    char major[50]; //רҵ
    int classNo; //�༶
    int score[3]; //���ſγɼ�
} STU;

STU student[N];
int studentCount = 0;

void Input(STU *p, int n) {
    FILE *file = fopen("studentInit.dat","r");
    if(file == NULL) {
        perror("�޷����ļ�");
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

void menu() {
    int choice, n, index;
    while (1) {
        printf("-------------------------\n");
        printf("ѧ������ϵͳ\n");
        printf("1. ���ļ�������ѧ����Ϣ\n");
        printf("2. ��ʾ����ѧ����Ϣ\n");
        printf("3. �����ȡĳ��ѧ����Ϣ\n");
        printf("4. �˳�\n");
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
