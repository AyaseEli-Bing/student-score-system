/*
 * 学生成绩管理系统（基础版）
 * 只用到：变量、输入输出、if、循环、数组、结构体、函数
 * 没有用：指针、链表、malloc、文件操作
 */
#include <stdio.h>
#include <string.h>   /* 只用了 strcpy / strcmp，用来复制和比较字符串 */

#define MAX_STUDENTS 10   /* 最多能存几名学生，改这一个地方就够了 */

/* 结构体：把"一个学生"的多条数据打包成一个整体 */
struct Student {
    char name[20];   /* 姓名：最多 19 个字符，末尾要留 1 个位置放结束标记 '\0' */
    int  score;      /* 成绩：0 到 100 的整数 */
};

/* 全局数组：一张"成绩单表格"，最多放 MAX_STUDENTS 行 */
struct Student students[MAX_STUDENTS];

/* 全局计数器：目前表格里已经填了几行 */
int student_count = 0;

/* 下面这些是"函数声明"：告诉编译器后面有这些函数，先别报错 */
void print_menu(void);
void clear_line(void);
int  read_valid_score(char name[]);
int  find_index_by_name(char name[]);
int  read_name_from_keyboard(char name[]);
void input_students(void);
void show_all_students(void);
void show_statistics(void);
void query_student(void);
void modify_student(void);
void delete_student(void);
void show_ranking(void);

int main(void) {
    int choice;        /* 用户在菜单里输入的操作编号 */
    int scan_result;   /* 保存 scanf 的返回值，用来判断输入是否合法 */

    printf("=================================\n");
    printf("     学生成绩管理系统（基础版）\n");
    printf("=================================\n");

    while (1) {                /* 一直循环，直到用户选择"退出" */
        print_menu();

        printf("请输入你要执行的操作编号（1-8）：");
        scan_result = scanf("%d", &choice);
        clear_line();

        /* EOF 表示输入流结束（终端里按 Ctrl+D）。 */
        /* 不处理它的话，scanf 会永远返回 EOF，程序就会疯狂重复提示。 */
        if (scan_result == EOF) {
            printf("\n输入已结束，程序退出。\n");
            return 0;
        }
        if (scan_result != 1) {   /* scanf 返回值不是 1，说明输入的不是整数 */
            printf("! 输入无效，请输入 1 到 8 之间的数字。\n");
            continue;          /* 跳过本次循环，重新显示菜单 */
        }

        if (choice == 1) {
            input_students();
        } else if (choice == 2) {
            show_all_students();
        } else if (choice == 3) {
            show_statistics();
        } else if (choice == 4) {
            query_student();
        } else if (choice == 5) {
            modify_student();
        } else if (choice == 6) {
            delete_student();
        } else if (choice == 7) {
            show_ranking();
        } else if (choice == 8) {
            printf("\n再见！程序已退出。\n");
            return 0;          /* 从 main 返回，程序结束 */
        } else {
            printf("! 没有这个功能，请输入 1-8 之间的数字。\n");
        }
    }
}

/* 显示菜单 */
void print_menu(void) {
    printf("\n----------- 菜 单 -----------\n");
    printf("1) 录入学生成绩\n");
    printf("2) 查看所有学生成绩\n");
    printf("3) 查看最高分、最低分、平均分\n");
    printf("4) 按姓名查询成绩\n");
    printf("5) 修改学生成绩\n");
    printf("6) 删除学生记录\n");
    printf("7) 按成绩排名（从高到低）\n");
    printf("8) 退出程序\n");
}

/* 把当前这一行里剩下的字符全部读掉丢弃 */
/* 作用：防止用户输入的多余字符（比如回车、字母）干扰下一次 scanf */
void clear_line(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* 循环体是空的：只是不停地把字符读走，不做任何处理 */
    }
}

/* 公共代码：反复要求输入一个 0~100 的整数，直到合法才返回 */
/* name 只用来把提示语拼成"请输入 张三 的成绩"，方便用户看清在改谁的分数 */
/* 返回值：合法的成绩；如果输入流结束（EOF）则返回 -1，调用方据此放弃本次操作 */
int read_valid_score(char name[]) {
    int score;
    int scan_result;

    while (1) {
        printf("请输入 %s 的成绩（0-100 之间的整数）：", name);
        scan_result = scanf("%d", &score);
        clear_line();

        if (scan_result == EOF) {
            /* EOF = 输入流结束了（终端里按 Ctrl+D），只能放弃本次操作 */
            printf("! 输入已结束，已取消本次操作。\n");
            return -1;
        }
        if (scan_result != 1) {
            /* 输入的不是数字，提示后重新要求输入 */
            printf("! 成绩必须是数字，请重新输入。\n");
            continue;
        }
        if (score < 0 || score > 100) {
            printf("! 成绩必须在 0 到 100 之间，请重新输入。\n");
            continue;
        }
        return score;   /* 到这里说明成绩合法 */
    }
}

/* 公共代码：读一个姓名 */
/* name 必须是长度 20 的字符数组（和各学生姓名、所有 %19s 的写法保持一致） */
/* 返回值：1 = 拿到了内容（内容是否真有这个人由调用方去数组里找）；0 = 输入无效或已结束 */
int read_name_from_keyboard(char name[]) {
    int scan_result;

    scan_result = scanf("%19s", name);   /* 19 = 数组长度 20 减 1，多出来的字符会被 clear_line 丢掉 */
    clear_line();
    if (scan_result != 1) {
        printf("! 输入无效，已返回主菜单。\n");
        return 0;
    }
    return 1;
}

/* 公共代码：按姓名在数组里找学生，返回他所在的下标（第几条记录） */
/* 找不到返回 -1。如果有同名的人，只返回录入在最前面的那一个 */
int find_index_by_name(char name[]) {
    int i;

    for (i = 0; i < student_count; i++) {
        if (strcmp(students[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* 功能 1：录入学生成绩 */
void input_students(void) {
    char temp_name[20];   /* 临时存放姓名，确认数据都合法后再写进数组 */
    int  score;
    int  scan_result;

    printf("\n===== 录入学生成绩（最多 %d 人）=====\n", MAX_STUDENTS);
    printf("说明：姓名输入 0 表示结束录入。\n");

    while (student_count < MAX_STUDENTS) {   /* 表格没满才继续录入 */
        printf("\n请输入学生姓名（输入 0 结束录入）：");
        scan_result = scanf("%19s", temp_name);
        clear_line();
        if (scan_result != 1) {
            /* 输入无效，或输入已结束（EOF）：直接离开函数，回到主菜单 */
            printf("! 输入无效，已返回主菜单。\n");
            return;
        }

        if (strcmp(temp_name, "0") == 0) {   /* strcmp 返回 0 表示两个字符串相同 */
            break;                           /* 用户主动结束录入 */
        }

        /* 成绩必须是 0~100 的整数，不合法就反复要求重新输入（检查交给公共函数） */
        score = read_valid_score(temp_name);
        if (score < 0) {
            return;   /* 输入流结束，放弃本次录入 */
        }

        /* 姓名和成绩都合法了，才写入数组的第 student_count 行 */
        strcpy(students[student_count].name, temp_name);
        students[student_count].score = score;
        student_count = student_count + 1;   /* 人数加 1，下一行才是空着的 */

        printf("√ 已录入第 %d 名学生：%s，成绩 %d 分\n", student_count, temp_name, score);
    }

    if (student_count >= MAX_STUDENTS) {
        printf("\n已经达到 %d 名学生的上限，无法继续录入。\n", MAX_STUDENTS);
    }
}

/* 功能 2：按录入顺序查看所有学生成绩 */
void show_all_students(void) {
    int i;   /* 数组下标，从 0 开始 */

    printf("\n===== 所有学生成绩（按录入顺序）=====\n");

    if (student_count == 0) {   /* 一行数据都没有 */
        printf("暂无学生成绩，请先录入。\n");
        return;
    }

    printf("序号\t姓名\t成绩\n");
    for (i = 0; i < student_count; i++) {   /* 只遍历真正录入过的行 */
        printf("%d\t%s\t%d\n", i + 1, students[i].name, students[i].score);
    }
    printf("共 %d 名学生。\n", student_count);
}

/* 功能 3：统计最高分、最低分、平均分、及格率、分数段人数 */
void show_statistics(void) {
    int    i;
    int    max_score, min_score, total;
    int    max_index, min_index;   /* 记住最高分和最低分在第几个学生 */
    int    fail_count;             /* 不及格（60 分以下）人数 */
    int    seg_90, seg_80, seg_70, seg_60;   /* 各分数段人数 */
    double average;

    if (student_count == 0) {
        printf("\n暂无学生成绩，无法统计。请先使用功能 1 录入。\n");
        return;
    }

    /* 先假设第 0 个学生既是最高也是最低，然后逐个往后比较 */
    max_score = students[0].score;
    max_index = 0;
    min_score = students[0].score;
    min_index = 0;
    total     = 0;
    fail_count = 0;
    seg_90 = 0;
    seg_80 = 0;
    seg_70 = 0;
    seg_60 = 0;

    for (i = 0; i < student_count; i++) {
        if (students[i].score > max_score) {
            max_score = students[i].score;
            max_index = i;
        }
        if (students[i].score < min_score) {
            min_score = students[i].score;
            min_index = i;
        }
        total = total + students[i].score;   /* 累加求总分 */

        /* 按 10 分为一档归类，统计各分数段有多少人 */
        if (students[i].score >= 90) {
            seg_90 = seg_90 + 1;
        } else if (students[i].score >= 80) {
            seg_80 = seg_80 + 1;
        } else if (students[i].score >= 70) {
            seg_70 = seg_70 + 1;
        } else if (students[i].score >= 60) {
            seg_60 = seg_60 + 1;
        } else {
            fail_count = fail_count + 1;
        }
    }

    /* total 和 student_count 都是 int，除法会丢掉小数部分； */
    /* 把其中一个转成 double，就能算出带小数的平均分 */
    average = (double)total / student_count;

    printf("\n===== 成绩统计（共 %d 名学生）=====\n", student_count);
    printf("最高分：%d 分（%s）\n", max_score, students[max_index].name);
    printf("最低分：%d 分（%s）\n", min_score, students[min_index].name);
    printf("平均分：%.2f 分\n", average);   /* %.2f 表示保留两位小数 */

    /* 60 分及以上算及格。人数直接用它做除法会得 0，所以要先把一个数转成 double */
    /* %% 是打印一个真正的百分号 */
    printf("及格率：%.1f%%（%d/%d 人，60 分及以上为及格）\n",
           (double)(student_count - fail_count) / student_count * 100,
           student_count - fail_count, student_count);

    printf("----- 分数段分布 -----\n");
    printf("90-100 分：%d 人\n", seg_90);
    printf("80-89  分：%d 人\n", seg_80);
    printf("70-79  分：%d 人\n", seg_70);
    printf("60-69  分：%d 人\n", seg_60);
    printf("60 分以下：%d 人\n", fail_count);
}

/* 功能 4：按姓名查询成绩 */
void query_student(void) {
    char name[20];
    int  idx;

    printf("\n===== 按姓名查询成绩 =====\n");
    if (student_count == 0) {
        printf("暂无学生成绩，请先录入。\n");
        return;
    }

    printf("请输入要查询的姓名：");
    if (read_name_from_keyboard(name) == 0) {
        return;
    }

    idx = find_index_by_name(name);
    if (idx == -1) {
        printf("! 没有找到姓名为 %s 的学生。\n", name);
        return;
    }
    printf("√ %s 的成绩是 %d 分（第 %d 条记录）\n", name, students[idx].score, idx + 1);
}

/* 功能 5：修改学生成绩（先按姓名找到人，再重新输入成绩覆盖原来的） */
void modify_student(void) {
    char name[20];
    int  idx;
    int  score;

    printf("\n===== 修改学生成绩 =====\n");
    if (student_count == 0) {
        printf("暂无学生成绩，请先录入。\n");
        return;
    }

    printf("请输入要修改的姓名：");
    if (read_name_from_keyboard(name) == 0) {
        return;
    }

    idx = find_index_by_name(name);
    if (idx == -1) {
        printf("! 没有找到姓名为 %s 的学生，无法修改。\n", name);
        return;
    }

    printf("√ 找到 %s，当前成绩 %d 分。\n", name, students[idx].score);
    score = read_valid_score(name);
    if (score < 0) {
        return;   /* 输入流结束，原成绩保持不变 */
    }

    students[idx].score = score;
    printf("√ 修改完成：%s 的成绩现在是 %d 分。\n", name, score);
}

/* 功能 6：删除学生记录 */
/* 做法：找到位置后，把它后面的每一行整体往前挪一格，最后人数减 1 */
void delete_student(void) {
    char name[20];
    char answer[20];
    int  idx;
    int  i;

    printf("\n===== 删除学生记录 =====\n");
    if (student_count == 0) {
        printf("暂无学生成绩，无需删除。\n");
        return;
    }

    printf("请输入要删除的姓名：");
    if (read_name_from_keyboard(name) == 0) {
        return;
    }

    idx = find_index_by_name(name);
    if (idx == -1) {
        printf("! 没有找到姓名为 %s 的学生，无需删除。\n", name);
        return;
    }

    /* 删除是不可恢复的操作，所以先让用户确认一次 */
    printf("将删除：%s，成绩 %d 分（第 %d 条记录）。确认删除请输入 y：",
           name, students[idx].score, idx + 1);
    if (read_name_from_keyboard(answer) == 0) {
        return;
    }
    if (strcmp(answer, "y") != 0) {
        printf("已取消删除，%s 的记录保持不变。\n", name);
        return;
    }

    for (i = idx; i < student_count - 1; i++) {
        students[i] = students[i + 1];   /* 结构体可以整体赋值，等于把下一行抄上来 */
    }
    student_count = student_count - 1;

    printf("√ 已删除 %s，现在共 %d 名学生。\n", name, student_count);
}

/* 功能 7：按成绩从高到低排名 */
/* 先在一份副本上排序，原来"按录入顺序"的表格不受影响，功能 2 还能照常用 */
void show_ranking(void) {
    struct Student sorted[MAX_STUDENTS];   /* 副本：排乱了也不影响原始数据 */
    struct Student temp;                  /* 交换时用来暂存一条记录 */
    int i, j;
    int rank;

    printf("\n===== 成绩排名（从高到低）=====\n");
    if (student_count == 0) {
        printf("暂无学生成绩，请先录入。\n");
        return;
    }

    for (i = 0; i < student_count; i++) {
        sorted[i] = students[i];
    }

    /* 冒泡排序：相邻两条比较，小的往后换 */
    /* 每轮下去，最后 i 个位置已经是有序的，所以内层只需走到 student_count-1-i */
    for (i = 0; i < student_count - 1; i++) {
        for (j = 0; j < student_count - 1 - i; j++) {
            if (sorted[j].score < sorted[j + 1].score) {
                temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    printf("名次\t姓名\t成绩\n");
    for (i = 0; i < student_count; i++) {
        /* 分数比前一名低，名次才跳到自己所在的序号；同分则沿用前一名，即并列 */
        if (i == 0 || sorted[i].score < sorted[i - 1].score) {
            rank = i + 1;
        }
        printf("%d\t%s\t%d\n", rank, sorted[i].name, sorted[i].score);
    }
    printf("共 %d 名学生，同分为并列名次。\n", student_count);
}
