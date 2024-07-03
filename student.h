//
// Created by 15857 on 24-7-3.
//

#ifndef STUDENT_H
#define STUDENT_H
typedef struct {
    char ID[50]; //学号
    char name[50]; //姓名
    char major[50]; //专业
    int classNo; //班级
    int score[3]; //三门课成绩
} STU;
#endif //STUDENT_H
