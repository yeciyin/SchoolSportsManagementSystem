#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>

using namespace std;

//全局变量
char athleteID[20]; // Athlete ID or Staff ID
char refereeID[50]; // Athlete Name
int isAdmin; // 1: Admin, 0: Not Admin
int group; char department[20];

// 定义结构体类型
typedef struct {
    char username[20];
    char password[20];
    int role; // 0: 管理员, 1: 运动员, 2: 裁判员
    int group;
    char department[20];
} User;

typedef struct {
    char projectName[50];
    int group;
    int nature;
    int maxParticipants;
    int minParticipants;
    char rules[200];
    char referee[50];
} Project;

typedef struct {
    char departmentName[100];
    char departmentID[20];
} Department;

typedef struct {
    char athleteID[20];
    char projectName[50];
    int group;
    int nature;
    int athleteNumber;
    char departmentID[20];
    int approval; // 0: 未审核, 1: 通过, 2: 未通过
} Registration;

typedef struct {
    char projectName[50];
    int group;
    int nature;
    char date[20];
    char time[10];
    char venue[50];
} Schedule;

// 函数声明
void login();
void adminMenu();
void athleteMenu();
void refereeMenu();
void manageProjects();
void manageUsers();
void manageSchdule();
void query();
void addProject();
void modifyProject();
void deleteProject();
void queryByProject();
void addUser();
void modifyUser();
void deleteUser();
void queryByUser();
void queryByDepartment();
void queryByAthlete();
void queryByVenue();
void queryByTime();
void queryReviewProgress();
void queryByProject();
void queryRefereeProjects();
void querySchedule();
void addRegistration();
void modifyRegistration();
void queryAthleteSchedule();
void loginPassword();
void loginWithoutPassword();
void deleteRegistration();
void addSchedule();
void modifySchedule();
void deleteSchedule();
void displayAllDepartments();
void reviewRegistrations();
void registUser();
void countpaticipant(char *projectName, int group, int nature);

//汉化函数
void chPrintNature(int number);
void chPrintGroup(int number);

int authenticate(char *username, char *password, int *role);

// 主函数
int main() {
    printf("欢迎使用运动会管理系统\n");
    login();
    return 0;
}

// 用户登录函数
void login() {
    int choice;
    printf("-------------------------\n");
    printf("1. 管理员\n");
    printf("2. 运动员/裁判员\n");
    printf("3. 注册账户\n");
    printf("4. 退出系统\n");
    printf("请选择身份: ");
    scanf("%d", &choice);
    printf("-------------------------\n");
    switch (choice) {
        case 1:
            loginPassword();
            break;
        case 2:
            loginWithoutPassword();
            break;
        case 3:
            isAdmin = 0;
            registUser();
            login();
            break;
        case 4:
            exit(0);
        default:
            printf("无效的选择\n");
            login();
    }
}

void loginPassword(){
    char username[20], password[20];
    int role;
    printf("-------------------------\n");
    printf("请输入用户名: ");
    scanf("%s", username);
    printf("请输入密码: ");
    char ch;
    int i = 0;
    while ((ch = getch()) != '\r') {
        if (ch == '\b' && i > 0) {
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
    printf("-------------------------\n");

    // 验证用户名和密码
    if (authenticate(username, password, &role)) {
    switch (role) {
        case 0:
            adminMenu();
            break;
        default:
            printf("非法访问\n");
            login();
    }
}else {
        printf("用户名或密码错误\n");
        login();
    }
}

void loginWithoutPassword(){
    char username[20], password[20];
    int role;
    printf("-------------------------\n");
    printf("请输入学号（教工用职工号）（学生裁判用学号）: ");
    scanf("%s", username);
    strcpy(password, "none");
    printf("-------------------------\n");

    // 验证用户名和密码
    if (authenticate(username, password, &role)) {
    switch (role) {
        case 1:
            strcpy(athleteID, username);
            athleteMenu();
            break;
        case 2:
            strcpy(refereeID, username);
            refereeMenu();
            break;
        default:
            printf("无效的用户角色\n");
            login();//其实可以删除，因为这个函数是无密码登录，所以不会出现错误
    }
}else {
        printf("用户名不存在, 是否注册?\n");
        printf("(0. 否, 1. 是): ");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) {
            registUser();
            login();
        } else {
            login();
        }
    }
}

// 验证用户名和密码的函数
int authenticate(char *username, char *password, int *role) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("无法打开用户文件\n");
        return 0;
    }

    User user;
    while (fscanf(file, "%s %s %d %d %s", user.username, user.password, &user.role, &user.group, user.department) != EOF) {
        if (strcmp(username, user.username) == 0 && strcmp(password, user.password) == 0) {
            *role = user.role;
            group = user.group;
            strcpy(department, user.department);
            fclose(file);
            return 1;
        }
    }//这里是验证用户名和密码的地方，如果用户名和密码都正确，就会返回1，否则返回0

    fclose(file);
    return 0;
}

// 管理员菜单
void adminMenu() {
    isAdmin = 1;
    printf("欢迎管理员\n");
    int choice;
    while (1) {
        printf("-------------------------\n");
        printf("1. 管理比赛项目\n");
        printf("2. 管理用户信息\n");
        printf("3. 管理赛程\n");
        printf("4. 查询\n");
        printf("5. 审核报名\n");
        printf("6. 退出登录\n");
        printf("请选择: ");
        scanf("%d", &choice);
        printf("-------------------------\n");
        switch (choice) {
            case 1:
                manageProjects();
                break;
            case 2:
                manageUsers();
                break;
            case 3:
                manageSchdule();
                break;
            case 4:
                query();
                break;
            case 5:
                reviewRegistrations();
                break;
            case 6:
                login();
            default:
                printf("无效的选择\n按任意键继续\n");
                adminMenu();
        }
    }
}

// 审核报名函数
void reviewRegistrations(){
    Registration registration;
    FILE *file = fopen("registrations.txt", "r");
    if (file == NULL) {
        printf("无法打开报名文件\n");
        return;
    }

    printf("待审核的报名:\n");
    int exist = 0;
    while (fscanf(file, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if(registration.approval == 0) {
            exist = 1;
            printf("运动员ID: %s, 项目名称: %s, ", registration.athleteID, registration.projectName);
            printf("项目组别: ");chPrintGroup(registration.group);printf(", ");
            printf("项目性质: ");chPrintNature(registration.nature);printf(", "); 
            printf("项目人数: %d, 学院ID: %s, 审核状态: %d\n", registration.athleteNumber, registration.departmentID, registration.approval);
        }
    }
    rewind(file);

    if(exist){
    char athleteID[20], projectName[50];
    printf("请输入要审核的运动员ID: ");
    scanf("%s", athleteID);
    printf("请输入需审核的项目名称: ");
    scanf("%s", projectName);

    int found = 0;
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("无法打开临时文件\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if ((strcmp(registration.athleteID, athleteID) == 0 && strcmp(registration.projectName, projectName) == 0) && registration.approval == 0) {
            found = 1;
            printf("项目组别: ");chPrintGroup(registration.group);printf(", ");
            printf("项目性质: ");chPrintNature(registration.nature);printf(", "); 
            printf("项目人数: %d, 学院ID: %s\n", registration.athleteNumber, registration.departmentID);
            printf("请输入审核结果 (1: 通过, 2: 未通过): ");
            scanf("%d", &registration.approval);
        }
        fprintf(tempFile, "%s %s %d %d %d %s %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
    }

    fclose(file);
    fclose(tempFile);

    remove("registrations.txt");
    rename("temp.txt", "registrations.txt");

    if (found) {
        printf("审核完成\n");
    } else {
        printf("未找到该报名\n");
    }
    } else {
        printf("已全部审核完毕!\n");
    }
}

// 运动员菜单
void athleteMenu() {
    printf("欢迎运动员\n");
    int choice;
    while (1) {
        printf("-------------------------\n");
        printf("1. 查询比赛项目\n");
        printf("2. 报名\n");
        printf("3. 修改报名\n");
        printf("4. 删除报名\n");
        printf("5. 查询最终参赛项目及赛程安排\n");
        printf("6. 退出登录\n");
        printf("请选择: ");
        scanf("%d", &choice);
        printf("-------------------------\n");
        switch (choice) {
            case 1:
            queryByProject();
            break;
            case 2:
            addRegistration();
            break;
            case 3:
            modifyRegistration();
            break;
            case 4:
            deleteRegistration();
            break;
            case 5:
            queryAthleteSchedule();
            break;
            case 6:
            login();
            default:
            printf("无效的选择\n按任意键继续\n");
            athleteMenu();
        }
    }
}

// 裁判员菜单
void refereeMenu() {
    printf("欢迎裁判员\n");
    int choice;
    while (1) {
        printf("-------------------------\n");
        printf("1. 查询自己担任裁判工作的项目\n");
        printf("2. 查询运动会的赛程安排\n");
        printf("3. 退出登录\n");
        printf("请选择: ");
        scanf("%d", &choice);
        printf("-------------------------\n");
        switch (choice) {
            case 1:
                queryRefereeProjects();
                break;
            case 2:
                querySchedule();
                break;
            case 3:
                login();
            default:
                printf("无效的选择\n按任意键继续\n");
                refereeMenu();
        }
    }
}

// 管理比赛项目函数
void manageProjects() {
    // 增加、修改、删除、查询比赛项目的具体实现
    int choice;
    while (1) {
        printf("-------------------------\n");
        printf("1. 增加比赛项目\n");
        printf("2. 修改比赛项目\n");
        printf("3. 删除比赛项目\n");
        printf("4. 返回\n");
        printf("请选择: ");
        scanf("%d", &choice);
        printf("-------------------------\n");
        switch (choice) {
            case 1:
                addProject();
                break;
            case 2:
                modifyProject();
                break;
            case 3:
                deleteProject();
                break;
            case 4:
                return;
            default:
                printf("无效的选择\n按任意键继续\n");
                manageProjects();
        }
    }
}

// 增加比赛项目函数
void addProject() {
    Project project;
    printf("-------------------------\n");
    printf("请输入项目名称: ");
    scanf("%s", project.projectName);
    FILE *file = fopen("projects.txt", "a");
    if (file == NULL) {
        printf("无法打开项目文件\n");
        return;
    }

    printf("请输入新项目的组别(用数字代替): \n");
    printf("1.教工老年男子组\n");
    printf("2.教工老年女子组\n");
    printf("3.教工中年男子组\n");
    printf("4.教工中年女子组\n");
    printf("5.教工青年男子组\n");
    printf("6.教工青年女子组\n");
    printf("7.研究生男子组\n");
    printf("8.研究生女子组\n");
    printf("9.本科生男子组\n");
    printf("10.本科生女子组\n");
    printf("11.男女混合组\n");
    scanf("%d", &project.group);
    while (project.group < 1 || project.group > 11) {
        printf("请输入一个有效的组别!\n");
        printf("请输入新项目的组别(用数字代替): \n");
        scanf("%d", &project.group);
    }

    printf("请输入新项目的性质(用数字代替): \n");
    printf("1.个人赛\n");
    printf("2.团体赛\n");

    scanf("%d", &project.nature);
    while (project.nature < 1 || project.nature > 2) {
        printf("请输入一个有效的项目性质!\n");
        printf("请输入新项目的性质(用数字代替): \n");
        scanf("%d", &project.nature);
    }

    do {
        printf("请输入参赛人数上限: ");
        scanf("%d", &project.maxParticipants);
        printf("请输入参赛人数下限: ");
        scanf("%d", &project.minParticipants);
        if (project.maxParticipants <= project.minParticipants) {
            printf("参赛人数上限必须大于下限，请重新输入。\n");
        }
    } while (project.maxParticipants <= project.minParticipants);

    printf("请输入比赛规则: ");
    scanf("%s", project.rules);

    int refereeFound = 0;
    User user;
    FILE *userFile = fopen("users.txt", "r");
    if (userFile == NULL) {
        printf("无法打开用户文件\n");
        fclose(file);
        return;
    }

    while (!refereeFound) {
        printf("请为该项目指定裁判员(输入裁判员的学号): ");
        scanf("%s", project.referee);
        rewind(userFile);
        while (fscanf(userFile, "%s %s %d %d %s", user.username, user.password, &user.role, &user.group, user.department) != EOF) {
            if (strcmp(user.username, project.referee) == 0 && user.role == 2) {
                refereeFound = 1;
                break;
            }
        }

        if (!refereeFound) {
            printf("无效的裁判员，请重新输入。\n");
        }
    }

    fclose(userFile);
    fprintf(file, "%s %d %d %d %d %s %s\n", project.projectName, project.group, project.nature, project.maxParticipants, project.minParticipants, project.rules, project.referee);
    fclose(file);
    printf("项目添加成功\n");
    printf("-------------------------\n");
    return;
}

// 修改比赛项目函数
void modifyProject() {
    deleteProject();
    addProject();
}

// 删除比赛项目函数
// void deleteProject() {
//     printf("-------------------------\n");
//     char projectName[50];
//     Project project;
//     FILE *file = fopen("projects.txt", "r");
//     FILE *tempFile = fopen("temp.txt", "w");
//     if (file == NULL || tempFile == NULL) {
//         printf("无法打开项目文件\n");
//         return;
//     }

//     int i = 0;
//     printf("现有项目:\n");
//     while (fscanf(file, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
//         i++;
//         printf("%d. 项目名称：%s, ", i, project.projectName);
//         printf("项目组别: ");chPrintGroup(project.group);printf(", ");
//         printf("项目性质: ");chPrintNature(project.nature);printf("\n");
//     }
//     rewind(file);

//     int number;
//     printf("请输入要删除的项目编号: ");
//     scanf("%d", &number);

//     i = 0;
//     while (fscanf(file, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
//         i++;
//         if (i == number) {
//             continue;
//         } 
//         fprintf(tempFile, "%s %d %d %d %d %s %s", project.projectName, project.group, project.nature, project.maxParticipants, project.minParticipants, project.rules, project.referee);
//     }  
//     fclose(file);
//     fclose(tempFile);

//     remove("projects.txt");
//     rename("temp.txt", "projects.txt");

//     Schedule schedule;
//     FILE *scheduleFile = fopen("schedule.txt", "r");
//     FILE *tempScheduleFile = fopen("temp_schedule.txt", "w");
//     if (scheduleFile == NULL || tempScheduleFile == NULL) {
//         printf("无法打开赛程文件\n");
//         return;
//     }

    
//     while (fscanf(scheduleFile, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
//         judge = 0;
//         if (strcmp(schedule.projectName, projectName) == 0) {
//             for(i=0;i<10;i++){
//                 if(groupnumber[i] == schedule.group && naturenumber[i] == schedule.nature){
//                     judge = 1;
//                     break;
//                 }
//             }
//         }
//         if(judge = 0){
//             fprintf(tempScheduleFile, "%s %d %d %s %s %s", schedule.projectName, schedule.group, schedule.nature, schedule.date, schedule.time, schedule.venue);
//         }
//     }

//     fclose(scheduleFile);
//     fclose(tempScheduleFile);

//     remove("schedule.txt");
//     rename("temp_schedule.txt", "schedule.txt");

//     if (!found) {
//         printf("未找到符合项目\n");
//     } else if (found < 0){
//         printf("未执行操作\n");
//     } else {
//         printf("项目删除成功\n");
//     }
//     printf("-------------------------\n");
// }

void deleteProject() {
    printf("-------------------------\n");
    char projectName[50];
    Project project;
    FILE *file = fopen("projects.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("无法打开项目文件\n");
        return;
    }

    int i = 0;
    while (fscanf(file, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
        i++;
        printf("%d. 项目名称：%s\t", i, project.projectName);
        printf("项目组别: ");chPrintGroup(project.group);printf("\t");
        printf("项目性质: ");chPrintNature(project.nature);printf("\n");
    }
    rewind(file);

    int number;
    printf("请输入要删除的项目编号: ");
    scanf("%d", &number);

    int found = 0;
    i = 0;
    while (fscanf(file, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
        i++;
        if (i != number) {
            fprintf(tempFile, "%s %d %d %d %d %s %s\n", project.projectName, project.group, project.nature, project.maxParticipants, project.minParticipants, project.rules, project.referee);
        } else {
            found = 1;
        }
    }
    
    fclose(file);
    fclose(tempFile);

    remove("projects.txt");
    rename("temp.txt", "projects.txt");

    Schedule schedule;
    FILE *scheduleFile = fopen("schedule.txt", "r");
    FILE *tempScheduleFile = fopen("temp_schedule.txt", "w");
    if (scheduleFile == NULL || tempScheduleFile == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }

    while (fscanf(scheduleFile, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
        if (strcmp(schedule.projectName, projectName) != 0 || schedule.group != project.group || schedule.nature != project.nature) {
            fprintf(tempScheduleFile, "%s %s %s %s\n", schedule.projectName, schedule.date, schedule.time, schedule.venue);
        }
    }

    fclose(scheduleFile);
    fclose(tempScheduleFile);

    remove("schedule.txt");
    rename("temp_schedule.txt", "schedule.txt");

    // 删除运动员报名信息
    Registration registration;
    FILE *regFile = fopen("registrations.txt", "r");
    FILE *tempRegFile = fopen("temp_registrations.txt", "w");
    if (regFile == NULL || tempRegFile == NULL) {
        printf("无法打开报名文件\n");
        return;
    }

    while (fscanf(regFile, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if (strcmp(registration.projectName, projectName) != 0 || registration.group != project.group|| registration.nature != project.nature) {
            fprintf(tempRegFile, "%s %s %d %d %d %s %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
        }
    }

    fclose(regFile);
    fclose(tempRegFile);

    remove("registrations.txt");
    rename("temp_registrations.txt", "registrations.txt");

    if (found) {
        printf("项目删除成功\n");
    } else {
        printf("未找到该项目\n");
    }
    printf("-------------------------\n");
}

// 查询比赛项目函数
void queryByProject() {
    printf("-------------------------\n");
    char projectName[50];
    Project project;
    FILE *file = fopen("projects.txt", "r");
    if (file == NULL) {
        printf("无法打开项目文件\n");
        return;
    }
    
    int i = 0;
    printf("田径&趣味运动会即将开启!\n现设有以下比赛项目:\n");
    while (fscanf(file, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
        i++;
        printf("%d. 项目名称: %s\t\t", i, project.projectName);
        printf("项目组别: "); chPrintGroup(project.group); printf("\t");
        printf("项目性质: "); chPrintNature(project.nature); printf("\n");
    }
    
    rewind(file);
    int number;
    printf("请输入你感兴趣的项目编号: ");
    scanf("%d", &number);

    i = 0;
    while (fscanf(file, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
        i++;
        if (i == number) {
            printf("项目名称: %s\n", project.projectName);
            printf("项目组别: ");chPrintGroup(project.group);printf("\n");
            printf("项目性质: ");chPrintNature(project.nature);printf("\n");
            printf("参赛人数上限: %d\n", project.maxParticipants);
            printf("参赛人数下限: %d\n", project.minParticipants);
            countpaticipant(project.projectName, project.group, project.nature);printf("\n");
            printf("比赛规则: %s\n", project.rules);
            printf("裁判员: %s\n", project.referee);
        }
    }
    fclose(file);
    printf("-------------------------\n");
}

// 管理用户信息函数
void manageUsers() {
    int choice;
    while (1) {
        printf("-------------------------\n");
        printf("1. 增加用户\n");
        printf("2. 修改用户\n");
        printf("3. 删除用户\n");
        printf("4. 返回\n");
        printf("请选择: ");
        scanf("%d", &choice);
        printf("-------------------------\n");
        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                modifyUser();
                break;
            case 3:
                deleteUser();
                break;
            case 4:
                return;
            default:
                printf("无效的选择\n");
                manageUsers();
        }
    }
}

// 增加用户函数
void addUser() {
    printf("-------------------------\n");
    User user;
    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("无法打开用户文件\n");
        return;
    }

    if(isAdmin == 0){
        printf("请输入角色 (1: 运动员, 2: 裁判员): ");
        scanf("%d", &user.role);
        switch (user.role)
        {
        case 1:
            break;
        case 2:
            break;
        default:
            printf("无效的角色\n");
            break;
        }
       
    } else {
        printf("请输入角色 (0: 管理员, 1: 运动员, 2: 裁判员): ");
        scanf("%d", &user.role);
    }

    if(user.role == 0) {
        printf("请输入用户名: ");
        scanf("%s", user.username);
        printf("请输入密码: ");
        scanf("%s", user.password);
    } else if(user.role == 1 || user.role == 2) {
        printf("请输入学号（教工用职工号）（学生裁判用学号）: ");
        scanf("%s", user.username);
        strcpy(user.password, "none");
    } else {
        printf("无效的角色\n");
        fclose(file);
        return;
    }

    User existingUser;
    FILE *existingFile = fopen("users.txt", "r");
    if (existingFile != NULL) {
        while (fscanf(existingFile, "%s %s %d %d %s", existingUser.username, existingUser.password, &existingUser.role, &existingUser.group, existingUser.department) != EOF) {
            if (strcmp(existingUser.username, user.username) == 0) {
                printf("用户已存在\n");
                fclose(existingFile);
                fclose(file);
                return;
            }
        }
        fclose(existingFile);
    }

    fprintf(file, "%s %s %d %d %s\n", user.username, user.password, user.role, user.group, user.department);
    fclose(file);
    printf("用户添加成功\n");
    printf("-------------------------\n");
}

// 修改用户函数
void modifyUser() {
    deleteUser();
    addUser();
}

// 删除用户函数
void deleteUser() {
    printf("-------------------------\n");
    char username[20];
    User user;
    FILE *file = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("无法打开用户文件\n");
        return;
    }
    while (fscanf(file, "%s %s %d %d %s", user.username, user.password, &user.role, &user.group, user.department) != EOF) {
        printf("用户名：%s\n", user.username);
    }
    rewind(file);

    printf("请输入要删除的用户名: ");
    scanf("%s", username);

    int found = 0;
    while (fscanf(file, "%s %s %d %d %s", user.username, user.password, &user.role, &user.group, user.department) != EOF) {
        if (strcmp(user.username, username) != 0) {
            fprintf(tempFile, "%s %s %d %d %s\n", user.username, user.password, user.role, user.group, user.department);
        } else {
            found = 1;
        }
    }
    
    fclose(file);
    fclose(tempFile);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found) {
        printf("用户删除成功\n");
    } else {
        printf("未找到该用户\n");
    }
    
}

// 查询用户函数
void queryByUser() {
    printf("-------------------------\n");
    char username[20];
    User user;
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("无法打开用户文件\n");
        return;
    }

    while (fscanf(file, "%s %s %d %d %s", user.username, user.password, &user.role, &user.group, user.department) != EOF) {
        printf("用户名: %s\t", user.username);
        printf("密码: %s\t", user.password);
        printf("用户角色(0.管理员 1.运动员 2.裁判员): %d\t", user.role);
        printf("用户分组: ");chPrintGroup(user.group);printf("\t");
        printf("学院编号: %s\n", user.department);

    }
    rewind(file);

    printf("请输入要查询的用户名: ");
    scanf("%s", username);

    int found = 0;
    while (fscanf(file, "%s %s %d %d %s", user.username, user.password, &user.role, &user.group, user.department) != EOF) {
        if (strcmp(user.username, username) == 0) {
            found = 1;
            printf("用户名: %s\t", user.username);
            printf("密码: %s\t", user.password);
            printf("角色(0.管理员 1.运动员 2.裁判员): %d\t", user.role);
            printf("用户分组: ");chPrintGroup(user.group);printf("\t");
            printf("学院编号: %s\n", user.department);
            break;
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该用户\n");
    }
    printf("-------------------------\n");
}

//管理赛程函数
void manageSchdule(){
    int choice;
    while (1) {
        printf("-------------------------\n");
        printf("1. 增加赛程\n");
        printf("2. 修改赛程\n");
        printf("3. 删除赛程\n");
        printf("4. 返回\n");
        printf("请选择: ");
        scanf("%d", &choice);
        printf("-------------------------\n");
        switch (choice) {
            case 1:
                addSchedule();
                break;
            case 2:
                modifySchedule();
                break;
            case 3:
                deleteSchedule();
                break;
            case 4:
                return;
            default:
                printf("无效的选择\n");
        }
    }
}

// 添加赛程函数
void addSchedule() {
    printf("-------------------------\n");
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "a");
    if (file == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }

    Project project;
    FILE *projectFile = fopen("projects.txt", "r");
    if (projectFile == NULL) {
        printf("无法打开项目文件\n");
        fclose(file);
        return;
    }

    printf("请选择目标项目:\n");
    while (fscanf(projectFile, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
        printf("%s ", project.projectName);
        chPrintGroup(project.group);printf(" ");
        chPrintNature(project.nature);printf("\n");
    }
    rewind(projectFile);

    int found = 0;
    while (!found) {
        printf("请输入项目名称: ");
        scanf("%s", schedule.projectName);
        printf("请输入项目组别(用数字代替): \n");
        printf("1.教工老年男子组\n");
        printf("2.教工老年女子组\n");
        printf("3.教工中年男子组\n");
        printf("4.教工中年女子组\n");
        printf("5.教工青年男子组\n");
        printf("6.教工青年女子组\n");
        printf("7.研究生男子组\n");
        printf("8.研究生女子组\n");
        printf("9.本科生男子组\n");
        printf("10.本科生女子组\n");
        printf("11.男女混合组\n");
        scanf("%d", &schedule.group);
        printf("请输入项目性质(用数字代替): \n");
        printf("1.个人赛\n");
        printf("2.团体赛\n");
        scanf("%d", &schedule.nature);

        while (fscanf(projectFile, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
            if (strcmp(schedule.projectName, project.projectName) == 0 && schedule.group == project.group && schedule.nature == project.nature) {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("无效的项目名称，请重新输入。\n");
            rewind(projectFile);
        }
    }
    fclose(projectFile);
    while (1) {
        printf("请输入日期 (YYYY-MM-DD): ");
        scanf("%s", schedule.date);

        // Check if the date is in the correct format
        if (strlen(schedule.date) == 10 &&
            schedule.date[4] == '-' &&
            schedule.date[7] == '-' &&
            isdigit(schedule.date[0]) &&
            isdigit(schedule.date[1]) &&
            isdigit(schedule.date[2]) &&
            isdigit(schedule.date[3]) &&
            isdigit(schedule.date[5]) &&
            isdigit(schedule.date[6]) &&
            isdigit(schedule.date[8]) &&
            isdigit(schedule.date[9])) {
            break;
        } else {
            printf("日期格式错误，请重新输入。\n");
        }
    }
    while (1) {
        printf("请输入时间 (HH:MM): ");
        scanf("%s", schedule.time);

        // Check if the time is in the correct format
        if (strlen(schedule.time) == 5 &&
            schedule.time[2] == ':' &&
            isdigit(schedule.time[0]) &&
            isdigit(schedule.time[1]) &&
            isdigit(schedule.time[3]) &&
            isdigit(schedule.time[4])) {
            break;
        } else {
            printf("时间格式错误，请重新输入。\n");
        }
    }
    printf("请输入场地: ");
    scanf("%s", schedule.venue);

    fprintf(file, "%s %d %d %s %s %s\n", schedule.projectName, schedule.group, schedule.nature, schedule.date, schedule.time, schedule.venue);
    fclose(file);
    printf("赛程安排成功\n");
    printf("-------------------------\n");
}

void modifySchedule() {
    deleteSchedule();
    addSchedule();
}

void deleteSchedule() {
    printf("-------------------------\n");
    char projectName[50];
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }

    printf("现有赛程:\n");
    while (fscanf(file, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
        printf("%s ", schedule.projectName);
        chPrintGroup(schedule.group);printf(" ");
        chPrintNature(schedule.nature);printf(" ");
        printf("%s %s %s\n", schedule.date, schedule.time, schedule.venue);
    }
    rewind(file);

    printf("请输入要删除的项目名称: ");
    scanf("%s", projectName);
    printf("请输入要删除的项目组别(用数字代替): \n");
    printf("1.教工老年男子组\n");
    printf("2.教工老年女子组\n");
    printf("3.教工中年男子组\n");
    printf("4.教工中年女子组\n");
    printf("5.教工青年男子组\n");
    printf("6.教工青年女子组\n");
    printf("7.研究生男子组\n");
    printf("8.研究生女子组\n");
    printf("9.本科生男子组\n");
    printf("10.本科生女子组\n");
    printf("11.男女混合组\n");
    scanf("%d", &schedule.group);
    printf("请输入要删除的项目性质(用数字代替): \n");
    printf("1.个人赛\n");
    printf("2.团体赛\n");
    scanf("%d", &schedule.nature);

    int found = 0;
    while (fscanf(file, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
        if (strcmp(schedule.projectName, projectName) != 0 || schedule.group != schedule.group || schedule.nature != schedule.nature) {
            fprintf(tempFile, "%s %d %d %s %s %s\n", schedule.projectName, schedule.group, schedule.nature, schedule.date, schedule.time, schedule.venue);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("schedule.txt");
    rename("temp.txt", "schedule.txt");

    if (found) {
        printf("赛程删除成功\n");
    } else {
        printf("未找到该赛程\n");
    }
    printf("-------------------------\n");
}

// 查询函数
void query() {
    // 查询的具体实现
    int choice;
    while (1) {
        printf("-------------------------\n");
        printf("1. 按比赛项目查询\n");
        printf("2. 按用户查询\n");
        printf("3. 按学院查询\n");
        printf("4. 按运动员查询\n");
        printf("5. 按场地查询\n");
        printf("6. 按时间查询\n");
        printf("7. 返回\n");
        printf("请选择: ");
        scanf("%d", &choice);
        printf("-------------------------\n");
        switch (choice) {
            case 1:
                queryByProject();
                break;
            case 2:
                queryByUser();
                break;
            case 3:
                queryByDepartment();
                break;
            case 4:
                queryByAthlete();
                break;
            case 5:
                queryByVenue();
                break;
            case 6:
                queryByTime();
                break;
            case 7:
                return;
            default:
                printf("无效的选择\n");
        }
    }
}

void queryByDepartment() {
    printf("-------------------------\n");
    char departmentID[20];
    Registration registration;
    FILE *file = fopen("registrations.txt", "r");
    if (file == NULL) {
        printf("无法打开报名文件\n");
        return;
    }

    printf("已有学院ID如下:\n");
    while (fscanf(file, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        printf("学院ID: %s\t", registration.departmentID);
        printf("运动员ID: %s\t", registration.athleteID);
        printf("项目名称: %s\t\t", registration.projectName);
        printf("项目组别: ");chPrintGroup(registration.group);printf("\t");
        printf("项目性质: ");chPrintNature(registration.nature);printf("\t");
        printf("运动员编号: %d\t", registration.athleteNumber);
        printf("审核状态(0为未审核,1未通过,2为未通过): %d\n", registration.approval);
    }
    rewind(file);

    printf("请输入要查询的学院ID: ");
    scanf("%s", departmentID);

    int found = 0;
    printf("该学院运动员的参赛情况如下:\n");
    while (fscanf(file, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if (strcmp(registration.departmentID, departmentID) == 0) {
            found = 1;
            printf("运动员ID: %s\t", registration.athleteID);
            printf("项目名称: %s\t", registration.projectName);
            printf("组别: ");chPrintGroup(registration.group);printf("\t");
            printf("性质: ");chPrintNature(registration.nature);printf("\t");
            printf("运动员编号: %d\t", registration.athleteNumber);
            printf("审核状态: %d\n", registration.approval);        
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该学院的运动员\n");
    }
    printf("-------------------------\n");
}

void queryByAthlete() {
    printf("-------------------------\n");
    char athleteID[20];
    Registration registration;
    FILE *file = fopen("registrations.txt", "r");
    if (file == NULL) {
        printf("无法打开报名文件\n");
        return;
        }

        /*// 用于记录已经显示过的运动员ID
        char displayedAthleteID[20][20];
        int displayedCount = 0;

        while (fscanf(file, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        int alreadyDisplayed = 0;
        for (int i = 0; i < displayedCount; i++) {
            if (strcmp(displayedAthleteID[i], registration.athleteID) == 0) {
            alreadyDisplayed = 1;
            break;
            }
        }
        if (!alreadyDisplayed) {
            printf("运动员ID: %s\n", registration.athleteID);
            strcpy(displayedAthleteID[displayedCount++], registration.athleteID);
        }
        }
        rewind(file);*/

        while (fscanf(file, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        printf("运动员ID: %s\t", registration.athleteID);
        printf("项目名称: %s\t\t", registration.projectName);
        printf("项目组别: ");chPrintGroup(registration.group);printf("\t");
        printf("项目性质: ");chPrintNature(registration.nature);printf("\t");
        printf("运动员编号: %d\t", registration.athleteNumber);
        printf("学院ID: %s\t", registration.departmentID);
        printf("审核状态: %d\n", registration.approval);
        }
        rewind(file);


    printf("请输入要查询的运动员ID: ");
    scanf("%s", athleteID);

    int found = 0;
    printf("该运动员的参赛情况如下:\n");
    while (fscanf(file, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if (strcmp(registration.athleteID, athleteID) == 0) {
            found = 1;
            printf("项目名称: %s\t", registration.projectName);
            printf("项目组别: ");chPrintGroup(registration.group);printf("\t");
            printf("项目性质: ");chPrintNature(registration.nature);printf("\t");
            printf("运动员编号: %d\t", registration.athleteNumber);
            printf("学院ID: %s\t", registration.departmentID);
            printf("审核状态: %d\n", registration.approval);
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该运动员\n");
    }
    printf("-------------------------\n");
}

void queryByVenue() {
    printf("-------------------------\n");
    char venue[50];
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }

    printf("场地如下:\n");
    while (fscanf(file, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
        printf("场地: %s\t\t\t", schedule.venue);
        printf("项目名称: %s\t\t", schedule.projectName);
        printf("组别: ");chPrintGroup(schedule.group);printf("\t");
        printf("性质: ");chPrintNature(schedule.nature);printf("\t");
        printf("日期: %s\t", schedule.date);
        printf("时间: %s\n", schedule.time);
    }
    rewind(file);

    printf("请输入要查询的场地: ");
    scanf("%s", venue);

    int found = 0;
    printf("该场地的赛程如下:\n");
    while (fscanf(file, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
        if (strcmp(schedule.venue, venue) == 0) {
            found = 1;
            printf("项目名称: %s\t", schedule.projectName);
            printf("日期: %s\t", schedule.date);
            printf("时间: %s\n", schedule.time);
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该场地的赛程\n");
    }
    printf("-------------------------\n");
}

void queryByTime() {
    printf("-------------------------\n");
    char date[20], time[10];
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }   

    printf("已有时间如下:\n");
    while (fscanf(file, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
        printf("日期: %s\t", schedule.date);
        printf("时间: %s\t", schedule.time);
        printf("项目名称: %s\t", schedule.projectName);
        printf("组别: ");chPrintGroup(schedule.group);printf("\t");
        printf("性质: ");chPrintNature(schedule.nature);printf("\t");
        printf("场地: %s\n", schedule.venue);
    }
    rewind(file);

    printf("请输入要查询的日期 (YYYY-MM-DD): ");
    scanf("%s", date);
    printf("请输入要查询的时间 (HH:MM): ");
    scanf("%s", time);

    int found = 0;
    printf("该时间的赛程如下:\n");
    while (fscanf(file, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
        if (strcmp(schedule.date, date) == 0 && strcmp(schedule.time, time) == 0) {
            found = 1;
            printf("项目名称: %s\t", schedule.projectName);
            printf("场地: %s\n", schedule.venue);
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该时间的赛程\n");
    }
    printf("-------------------------\n");
}

void queryReviewProgress() {
    printf("-------------------------\n");
    Registration registration;
    FILE *file = fopen("registrations.txt", "r");
    if (file == NULL) {
        printf("无法打开报名文件\n");
        return;
    }

    printf("审核进度:\n");
    while (fscanf(file, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        printf("运动员ID: %s, 项目名称: %s, ", registration.athleteID, registration.projectName);
            printf("项目组别: ");chPrintGroup(registration.group);printf(", ");
            printf("项目性质: ");chPrintNature(registration.nature);printf(", "); 
            printf("项目人数: %d, 学院ID: %s, 审核状态: %d\n", registration.athleteNumber, registration.departmentID, registration.approval);
    }
    fclose(file);
    printf("-------------------------\n");
}

void queryRefereeProjects(){
    printf("-------------------------\n");
    Project project;
    Schedule schedule;
    Registration registration;
    FILE *projectFile = fopen("projects.txt", "r");
    FILE *scheduleFile = fopen("schedule.txt", "r");
    FILE *registrationFile = fopen("registrations.txt", "r");

    if (projectFile == NULL || scheduleFile == NULL || registrationFile == NULL) {
        printf("无法打开文件\n");
        return;
    }

    printf("裁判员 %s 担任裁判工作的项目:\n", refereeID);
    int judege = 0;
    while (fscanf(projectFile, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
        if (strcmp(project.referee, refereeID) == 0) {
            printf("项目名称: %s\t\t", project.projectName);
            printf("项目组别: ");chPrintGroup(project.group);printf("\t");
            printf("项目性质: ");chPrintNature(project.nature);printf("\t");
            while (fscanf(scheduleFile, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
                judege = 0;
                if (strcmp(schedule.projectName, project.projectName) == 0 && schedule.group == project.group && schedule.nature == project.nature) {
                    judege = 1;
                    printf("比赛时间: %s %s\t", schedule.date, schedule.time);
                    printf("比赛地点: %s\n", schedule.venue);
                }           
            }
            rewind(scheduleFile);
        }
    }

    fclose(projectFile);
    fclose(scheduleFile);
    fclose(registrationFile);
    printf("-------------------------\n");
}

void querySchedule(){
    printf("-------------------------\n");
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }
    printf("运动会赛程安排:\n");
    while (fscanf(file, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
        printf("项目名称: %s\t\t", schedule.projectName);
        printf("日期: %s\t", schedule.date);
        printf("时间: %s\t", schedule.time);
        printf("场地: %s\n", schedule.venue);
    }
    fclose(file);
    printf("-------------------------\n");
}

void addRegistration() {
    printf("-------------------------\n");
    Registration registration;
    FILE *file = fopen("registrations.txt", "a");
    if (file == NULL) {
        printf("无法打开报名文件\n");
        return;
    }

    Project project;
    FILE *projectFile = fopen("projects.txt", "r");
    if (projectFile == NULL) {
        printf("无法打开项目文件\n");
        fclose(file);
        return;
    }

    int i = 0;
    printf("可供报名的项目:\n");
    while (fscanf(projectFile, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF) {
        if(project.group == group || project.group == 11){
            i++;
            printf("%d. 项目名称: %s\t\t", i, project.projectName);
            printf("项目组别: "); chPrintGroup(project.group); printf("\t");
            printf("项目性质: "); chPrintNature(project.nature); printf("\n");
        }
    }
    rewind(projectFile);

    int number;
    printf("请输入你想报名的项目编号: ");
    scanf("%d", &number);
    
    i = 0;
    while(fscanf(projectFile, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF){
        if(project.group == group || project.group == 11){
            i++;
            if (i == number) {
                strcpy(registration.projectName, project.projectName);
                registration.group = project.group;
                registration.nature = project.nature;
            }
        }
    }

    int participantCount = 0;
    Registration tempReg;
    FILE *tempFile = fopen("registrations.txt", "r");
    if (tempFile != NULL) {
        while (fscanf(tempFile, "%s %s %d %d %d %s %d", tempReg.athleteID, tempReg.projectName, &tempReg.group, &tempReg.nature, &tempReg.athleteNumber, tempReg.departmentID, &tempReg.approval) != EOF) {
            if (strcmp(tempReg.projectName, registration.projectName) == 0 && tempReg.group == registration.group && tempReg.nature == registration.nature && tempReg.approval == 1) {
                participantCount++;
            }
        }
    } else {
        printf("无法打开文件\n");
    }

    if (participantCount >= project.maxParticipants) {
        printf("报名人数已满，无法报名\n");
        fclose(file);
        fclose(projectFile);
        return;
    }

    int maxAthleteNumber = 0;
    tempFile = fopen("registrations.txt", "r");
    if (tempFile != NULL) {
        while (fscanf(tempFile, "%s %s %d %d %d %s %d", tempReg.athleteID, tempReg.projectName, &tempReg.group, &tempReg.nature, &tempReg.athleteNumber, tempReg.departmentID, &tempReg.approval) != EOF) {
            while(strcmp(tempReg.projectName, registration.projectName) == 0 && tempReg.group == registration.group && tempReg.nature == registration.nature){
                if (tempReg.athleteNumber > maxAthleteNumber) {
                    maxAthleteNumber = tempReg.athleteNumber;
                }
            }
        }
        fclose(tempFile);
    } else {
        printf("无法打开文件\n");
    }

    registration.athleteNumber = maxAthleteNumber + 1;

    strcpy(registration.departmentID, department);
    registration.approval = 0;
    fprintf(file, "%s %s %d %d %d %s %d\n", athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
    fclose(file);
    fclose(projectFile);
    printf("报名成功\n");
    printf("-------------------------\n");
}

void modifyRegistration(){
    deleteRegistration();
    addRegistration();
}

void queryAthleteSchedule(){
    printf("-------------------------\n");
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }

    printf("运动员 %s 的赛程安排:\n", athleteID);
    int found = 0;
    while (fscanf(file, "%s %d %d %s %s %s", schedule.projectName, &schedule.group, &schedule.nature, schedule.date, schedule.time, schedule.venue) != EOF) {
        Registration registration;
        FILE *regFile = fopen("registrations.txt", "r");
        if (regFile == NULL) {
            printf("无法打开报名文件\n");
            fclose(file);
            return;
        }

        while (fscanf(regFile, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
            if (strcmp(registration.athleteID, athleteID) == 0 && strcmp(registration.projectName, schedule.projectName) == 0 && registration.group == schedule.group && registration.nature == schedule.nature) {
                found = 1;
                printf("项目名称: %s\t", schedule.projectName);
                printf("日期: %s\t", schedule.date);
                printf("时间: %s\t", schedule.time);
                printf("场地: %s\n", schedule.venue);
                break;
            }
        }
        fclose(regFile);
    }

    fclose(file);
    if (!found) {
        printf("未找到该运动员的赛程\n");
    }
    printf("-------------------------\n");
}

void deleteRegistration() {
    printf("-------------------------\n");
    char projectName[50];
    Registration registration;
    FILE *file = fopen("registrations.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("无法打开报名文件\n");
        return;
    }
    rewind(file);

    printf("运动员 %s 的报名:\n", athleteID);
    int exist = 0;
    while (fscanf(file, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if (strcmp(registration.athleteID, athleteID) == 0 && registration.approval == 0){
            exist = 1;
            printf("项目名称: %s\t 项目组别: ", registration.projectName);
            chPrintGroup(registration.group);printf("\t");
            printf("项目性质: ");
            chPrintNature(registration.nature);printf("\t");
            printf("项目人数: %d\t 学院ID: %s\t 审核状态: %d\n", registration.athleteNumber, registration.departmentID, registration.approval);
        }
    }
    if(!exist){
        printf("暂无可删除的报名\n");
    }
    rewind(file);

    if(exist){
    printf("请输入要删除的项目名称: ");
    scanf("%s", projectName);

    int judge,found = 0;
    while (fscanf(file, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if ((strcmp(registration.athleteID, athleteID) == 0 && strcmp(registration.projectName, projectName) == 0) && registration.approval == 0) {
            printf("其项目组别为:"); chPrintGroup(registration.group);
            printf("项目性质为:"); chPrintNature(registration.nature);
            printf("项目人数为: %d, 学院ID为: %s\n", registration.athleteNumber, registration.departmentID);
            printf("确认删除? (0: 否, 1: 是): ");
            scanf("%d", &judge);
            if(!judge){
                found--;
                fprintf(tempFile, "%s %s %s %s %d %s %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
            } else {
                found = 100;
            }
        } else {
            fprintf(tempFile, "%s %s %d %d %d %s %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("registrations.txt");
    rename("temp.txt", "registrations.txt");

    if (!found) {
        printf("未找到符合报名\n");
    } else if (found < 0){
        printf("未执行操作\n");
    } else {
        printf("报名删除成功\n");
    }
    } else {
        printf("暂无可删除项目\n");
    }
    printf("-------------------------\n");
}

void displayAllDepartments() {
    printf("-------------------------\n");
    Department department;
    FILE *file = fopen("departments.txt", "r");
    if (file == NULL) {
        printf("无法打开学院文件\n");
        return;
    }

    printf("学院列表:\n");
    while (fscanf(file, "%s %s", department.departmentID, department.departmentName) != EOF) {
        printf("学院ID: %s, 学院名称: %s\n", department.departmentID, department.departmentName);
    }

    fclose(file);
    printf("-------------------------\n");
}

void registUser() {
    printf("-------------------------\n");
    User user;
    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("无法打开用户文件\n");
        return;
    }

    printf("请输入学号（或职工号）: ");
    scanf("%s", user.username);
    strcpy(user.password, "none");

    int choice;
    printf("请选择你的身份:\n");
    printf("1.男性老年教职工\n");
    printf("2.女性老年教职工\n");
    printf("3.男性中年教职工\n");
    printf("4.女性中年教职工\n");
    printf("5.男性青年教职工\n");
    printf("6.女性青年教职工\n");
    printf("7.男性研究生\n");
    printf("8.女性研究生\n");
    printf("9.男性本科生\n");
    printf("10.女性本科生\n");
    scanf("%d", &user.group);
    printf("请选择你的学院:\n");
    printf("1.通信与信息工程学院\n");
    printf("2.电子与光学工程学院\n");
    printf("3.计算机学院\n");
    printf("4.理学院\n");
    scanf("%d", &user.department);

    User existingUser;
    FILE *existingFile = fopen("users.txt", "r");
    if (existingFile != NULL) {
        while (fscanf(existingFile, "%s %s %d %d %d", existingUser.username, existingUser.password, &existingUser.role, &existingUser.group, &existingUser.department) != EOF) {
            if (strcmp(existingUser.username, user.username) == 0) {
                printf("用户已存在\n");
                fclose(existingFile);
                fclose(file);
                registUser();
            }
        }
        fclose(existingFile);
    }

    fprintf(file, "%s %s 1 %d %d\n", user.username, user.password, user.group, user.department);
    fclose(file);
    printf("注册成功！请登录\n");
    printf("-------------------------\n");
}

void countpaticipant(char *projectName, int group, int nature){
    int participantCount = 0;
    int maxParticipants;
    Registration registration;
    Project project;
    FILE *regFile = fopen("registrations.txt", "r");
    FILE *projectFile = fopen("projects.txt", "r");
    if (regFile == NULL || projectFile == NULL) {
        printf("无法打开报名文件\n");
        return;
    }
    while(fscanf(regFile, "%s %s %d %d %d %s %d", registration.athleteID, registration.projectName, &registration.group, &registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF){
        if (strcmp(registration.projectName, projectName) == 0 && registration.group == group && registration.nature == nature && registration.approval == 1){
            participantCount++;
        }
    }
    while(fscanf(projectFile, "%s %d %d %d %d %s %s", project.projectName, &project.group, &project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee) != EOF){
        if (strcmp(project.projectName, projectName) == 0 && project.group == group && project.nature == nature){
            maxParticipants = project.maxParticipants;
            break;
        }
    }
    fclose(regFile);
    fclose(projectFile);
    printf("已报名人数: %d", participantCount);
    if (participantCount >= maxParticipants){
        printf("(人数已满!)");
    }
    else{
        printf("(还可报名!)");
    }
}

void chPrintGroup(int number){
    switch (number){
        case 1:
            printf("教工老年男子组");
            break;
        case 2:
            printf("教工老年女子组");
            break;
        case 3:
            printf("教工中年男子组");
            break;
        case 4:
            printf("教工中年女子组");
            break;
        case 5:
            printf("教工青年男子组");
            break;
        case 6:
            printf("教工青年女子组");
            break;
        case 7:
            printf("研究生男子组  ");
            break;
        case 8:
            printf("研究生女子组  ");
            break;
        case 9:
            printf("本科生男子组  ");
            break;
        case 10:
            printf("本科生女子组  ");
            break;
        case 11:
            printf("男女混合组    ");
            break;
        default:
            printf("未知");
            break;
        }
}

void chPrintNature(int number){
    switch (number){
    case 1:
        printf("个人赛");
        break;
    case 2:
        printf("团体赛");
        break;
    default:
        printf("未知");
        break;
    }
}