//
// Created by Sk1ua on 24-7-3.
//

// ������Ҫ��ͷ�ļ�
#include <algorithm>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ����ѧ���ṹ�嶨���ͷ�ļ�
#include "student.h"

#define N 100  // �������ѧ������

// �������ڱ�����߷�ѧ�������Ľṹ��
typedef struct {
    int* indices; // ���ڱ�����߷�ѧ������������
    int count;    // ��߷�ѧ��������
} MaxIndices;

STU student[N]; // ѧ������
int studentCount = 0; // ��ǰѧ������
char NowFileName[255] = "StudentInit.dat"; // ��ǰ�ļ���

// ���ļ��ж���ѧ����Ϣ
void Input(const char* filename, STU *p, int maxStudents) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("�޷����ļ�");
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
    printf("�ɹ����� %d ��ѧ����Ϣ\n", count);

    fclose(file);
}

// ��ѧ����Ϣд���ļ�
void InputToFile(const char* filename, STU* students, int numStudents) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("�޷����ļ�");
        return;
    }

    size_t written = fwrite(students, sizeof(STU), numStudents, file);
    if (written != numStudents) {
        perror("д���ļ�ʱ����");
    }

    fclose(file);
}

// ��׼�����ʽ
void Output(STU *p) {
    printf("| %-10s | %-20s | %-20s | %-6d | %-6d | %-6d | %-6d | %-6d |\n",
           p->ID, p->name, p->major, p->classNo, p->score[0], p->score[1], p->score[2],
           p->score[0] + p->score[1] + p->score[2]);
}

// ��׼�����ͷ
void Outputtitle() {
    printf("|     ID     |         Name         |        Major         | Class  | Score1 | Score2 | Score3 | Total  |\n");
}

// ������������ѧ����Ϣ
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
// ����ĳ�����ɼ�ѧ��
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
// ��ѯ���༶���ɼ�
void Search(STU *p, int classNo, char s, int scoreSum) {
    int cnt = 0;
    for (int i = 0; i < studentCount; i++) {
        int totalScore = p[i].score[0] + p[i].score[1] + p[i].score[2];
        if (p[i].classNo == classNo && ((s == '>' && totalScore > scoreSum) || (s == '<' && totalScore < scoreSum))) {
            Output(&p[i]);
        }
    }
    printf("���ҵ�%d��ѧ��",cnt);
}

// ����IDɾ��ѧ����Ϣ
void DeleteStudent(char* deleteID) {
    int tempCount = 0;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(student[i].ID, deleteID) != 0) {
            student[tempCount++] = student[i];
        }
    }
    if (tempCount != studentCount) {
        studentCount = tempCount;
        printf("ѧ��Ϊ %s ��ѧ����Ϣ��ɾ����\n", deleteID);
    } else {
        printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����\n", deleteID);
    }
}

// ѡ������ȫ��ƽ���ɼ�����
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

// ð�����򣬰༶ƽ���ɼ�����
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
    printf("���ҵ�%d��ѧ��\n",count);
    char filename[50];
    printf("���������������ļ���:");
    int read = scanf("%s",filename);
    if (read != 1) {
        printf("�����������������\n");
        while (getchar() != '\n');
        return;
    }
    InputToFile(filename,stu_class_ave,count);
    printf("�������Ϣ�ɹ�����%s\n",filename);
}

// �������򣬸���רҵ�Ϳγ�����
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
    printf("���ҵ�%d��ѧ��\n",count);
    char filename[50];
    printf("���������������ļ���:");
    int read = scanf("%s",filename);
    if (read != 1) {
        printf("�����������������\n");
        while (getchar() != '\n');
        return;
    }
    InputToFile(filename,stu_major_subject,count);
    printf("�������Ϣ�ɹ�����%s\n",filename);
}

// ����ID�ֵ�������
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

// ¼��ѧ����Ϣ
void Save(STU *p, int n) {
    for (int i = 0; i < n; i++) {
        printf("����ѧ����Ϣ: \n");
        printf("ѧ��: ");
        scanf("%s", p[studentCount].ID);
        getchar();
        printf("����: ");
        scanf("%s", p[studentCount].name);
        getchar();
        printf("רҵ: ");
        scanf("%s", p[studentCount].major);
        getchar();
        printf("�༶: ");
        scanf("%d", &p[studentCount].classNo);
        getchar();
        printf("�γ�1�ɼ�: ");
        scanf("%d", &p[studentCount].score[0]);
        getchar();
        printf("�γ�2�ɼ�: ");
        scanf("%d", &p[studentCount].score[1]);
        getchar();
        printf("�γ�3�ɼ�: ");
        scanf("%d", &p[studentCount].score[2]);
        getchar();
        studentCount++;
    }
    // д���ļ�
    InputToFile("studentInit.dat", p, studentCount);
    printf("¼��ѧ����Ϣ�ɹ�\n");
    SortByID(p, studentCount);
}

// ���˵�����
void menu() {
    int choice, n, index;
    printf("��Ĭ�ϴ�\"studentInit.dat\"�ļ�\n");
    Input("studentInit.dat",student, 10);
    SortByID(student, 10);
    while (1) {
        printf("--------ѧ������ϵͳ�˵�--------\n");
        printf("1. ���µ�ѧ����Ϣ�����ļ���\n");
        printf("2. ���ļ���ɾ��ѧ����Ϣ\n");
        printf("3. ��ʾ����ѧ����Ϣ\n");
        printf("4. ָ��ѧ����Ϣ����(����,ѧ��)\n");
        printf("5. ����ĳ���γ̵���߷�ѧ��\n");
        printf("6. ����(������ƽ��������/���༶����/��רҵ,��ĳ�ſ�����)\n");
        printf("7. ���ض�������ѯ(�༶,������)\n");
        printf("8. ��յ�ǰϵͳ�д����ѧ����Ϣ�ļ�\n");
        printf("9. �����µ�ѧ���ļ���Ϣ\n");
        printf("10.�˳�ϵͳ\n");
        printf("----------------------------\n");
        if(studentCount)printf("���ڴ򿪵���\"%s\"�ļ�\n",NowFileName);
        else printf("δ��ѧ����Ϣ�ļ�\n");
        printf("��ǰ�Ѿ�¼��%d��ѧ������Ϣ.\n", studentCount);
        printf("ѡ��һ��ѡ��: ");
        int read = scanf("%d", &choice);
        if (read != 1) {
            printf("�����������������\n");
            while (getchar() != '\n');
            continue;
        }

        // �����û�ѡ��ִ����Ӧ����
        switch (choice) {
            case 1: {
                printf("����ѧ������: ");
                scanf("%d", &n);
                Save(student,n);
                system("pause");
                system("cls");
            }
            break;
            case 2: {
                char deleteID[50];
                printf("������Ҫɾ����ѧ��ѧ��: ");
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
                printf("ѡ���ѯ���ͣ�\n");
                printf("1. ����ѧ�Ų�ѯ(example: IDXXX)\n");
                printf("2. ����������ѯ(example: Jack)\n");
                printf("ѡ��һ��ѡ��: ");
                int read = scanf("%d", &queryType);
                if (read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (queryType != 1 && queryType != 2) {
                    printf("��Ч�Ĳ�ѯ����\n");
                    continue;
                }

                printf("�����ѯ����: ");
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
                printf("���ҵ�%d��ѧ��\n", j);
                if (!found) {
                    printf("δ�ҵ�ƥ���ѧ����Ϣ\n");
                }
                system("pause");
                system("cls");
            }
            break;

            case 5:
                printf("����γ����: (1-3)");
            scanf("%d", &index);
            index--;
            if (index >= 0 && index < 3) {6

                MaxIndices maxIndices = Max(student, index);
                if (maxIndices.count > 0) {
                    Outputtitle();
                    for (int i = 0; i < maxIndices.count; i++) {
                        Output(&student[maxIndices.indices[i]]);
                    }
                    printf("���ҵ�%d���γ����%d����߷�ѧ��\n", maxIndices.count, index + 1);
                } else {
                    printf("δ�ҵ���߷�ѧ��\n");
                }
                free(maxIndices.indices);
            } else {
                printf("��Ч�Ŀγ����\n");
            }
            system("pause");
            system("cls");
            break;

            case 6:
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
                case 1: {
                    SortSelect(student);
                    printf("��ƽ����������ѧ����Ϣ:\n");
                    Outputtitle();
                    for (int i = 0; i < studentCount; i++) {
                        Output(&student[i]);
                    }
                    char filename[50];
                    printf("���������������ļ���:");
                    int read = scanf("%s",filename);
                    if (read != 1) {
                        printf("�����������������\n");
                        while (getchar() != '\n');
                        return;
                    }
                    InputToFile(filename,student,studentCount);
                    printf("�������Ϣ�ɹ�����%s\n",filename);
                    break;
                }
                case 2: {
                    int classNo;
                    printf("����༶��(1-3): ");
                    scanf("%d", &classNo);
                    Sort_buble(student, studentCount, classNo);
                    break;
                }
                case 3: {
                    char major[50];
                    printf("1.ComputerScinece 2.Software 3.NetWork\n");
                    printf("����רҵ(���������רҵ����):");
                    int read = scanf("%s", major);
                    getchar();
                    if (read != 1) {
                        printf("�����������������\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    if (strcmp(major, "ComputerScience") != 0 && strcmp(major, "Software") != 0 && strcmp(major, "NetWork") != 0) {
                        printf("��Ч��רҵ����\n");
                        return;
                    }
                    printf("����γ����(1-3): ");
                    int subjectIndex;
                    read = scanf("%d", &subjectIndex);
                    if (read != 1) {
                        printf("�����������������\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    subjectIndex--;
                    if (subjectIndex >= 0 && subjectIndex < 3) {
                        Sort_insert(student, studentCount, major, subjectIndex);
                    } else {
                        printf("��Ч�Ŀγ����\n");
                    }
                }
                break;
                default:
                    printf("��Чѡ��\n");
                system("pause");
                system("cls");
            }
            break;

            case 7: {
                printf("���ض�������ѯ\n");
                printf("������༶���(1-3): ");
                int classNo;
                read = scanf("%d", &classNo);
                if (read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (classNo < 1 || classNo > 3) {
                    printf("��Ч�İ༶���\n");
                    continue;
                }
                getchar();
                printf("�����ѯ����(> �� <): ");
                char s;
                read = scanf("%c", &s);
                if (read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (s != '>' && s != '<') {
                    printf("��Ч�Ĳ�ѯ����\n");
                    continue;
                }
                printf("�������(0-300): ");
                int scoreSum;
                read = scanf("%d", &scoreSum);
                if (read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                if(scoreSum < 0 || scoreSum > 300) {
                    printf("��Ч�ķ���\n");
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
            printf("��ǰ�ڴ��е�ѧ����Ϣ����ա�\n");
            system("pause");
            system("cls");
            break;

            case 9: {
                char newFilename[256];
                printf("�������µ�ѧ����Ϣ�ļ���: ");
                read =  scanf("%255s", newFilename);
                if (read != 1) {
                    printf("�����������������\n");
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
