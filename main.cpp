#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>

using namespace std;
//全局变量
char athleteID[20]; // Athlete ID or Staff ID
char refereeName[50]; // Athlete Name

// 定义结构体类型
typedef struct {
    char username[20];
    char password[20];
    int role; // 0: 管理员, 1: 运动员, 2: 裁判员
} User;

typedef struct {
    char projectName[50];
    char group[50];
    char nature[20];
    int maxParticipants;
    int minParticipants;
    char rules[200];
    char referee[50];
    char otherInfo[100];
} Project;

typedef struct {
    char departmentName[100];
    char departmentID[20];
} Department;

typedef struct {
    char athleteID[20];
    char projectName[50];
    char group[50];
    char nature[20];
    int athleteNumber;
    char departmentID[20];
    int approval; // 0: 未审核, 1: 通过, 2: 未通过
} Registration;

typedef struct {
    char projectName[50];
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

int authenticate(char *username, char *password, int *role);

// 主函数
int main() {
    login();
    return 0;
}

// 用户登录函数
void login() {
    int choice;
    printf("请选择身份:\n");
    printf("1. 管理员\n");
    printf("2. 运动员/裁判员\n");
    printf("3. 退出系统\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            loginPassword();
            break;
        case 2:
            loginWithoutPassword();
            break;
        case 3:
            exit(0);
        default:
            printf("无效的选择\n");
            login();
    }
}

void loginPassword(){
    char username[20], password[20];
    int role;
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

    // 验证用户名和密码
    if (authenticate(username, password, &role)) {
    switch (role) {
        case 0:
            adminMenu();
            break;
        default:
            printf("怎么知道密码的😠,好啊，窃取后台数据是吧。\n");
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
    printf("请输入学号（教工用职工号）（学生裁判用学号）: ");
    scanf("%s", username);
    strcpy(password, "none");

    // 验证用户名和密码
    if (authenticate(username, password, &role)) {
    switch (role) {
        case 1:
            strcpy(athleteID, username);
            athleteMenu();
            break;
        case 2:
            strcpy(refereeName, username);
            refereeMenu();
            break;
        default:
            printf("无效的用户角色\n");
            login();//其实可以删除，因为这个函数是无密码登录，所以不会出现错误
    }
}else {
        printf("用户名错误\n");
        login();
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
    while (fscanf(file, "%s %s %d", user.username, user.password, &user.role) != EOF) {
        if (strcmp(username, user.username) == 0 && strcmp(password, user.password) == 0) {
            *role = user.role;
            fclose(file);
            return 1;
        }
    }//这里是验证用户名和密码的地方，如果用户名和密码都正确，就会返回1，否则返回0

    fclose(file);
    return 0;
}

// 管理员菜单
void adminMenu() {
    printf("欢迎管理员\n");
    int choice;
    while (1) {
        printf("1. 管理比赛项目\n");
        printf("2. 管理用户信息\n");
        printf("3. 管理赛程\n");
        printf("4. 查询\n");
        printf("5. 审核报名\n");
        printf("6. 退出登录\n");
        printf("请选择: ");
        scanf("%d", &choice);
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

    printf("审核报名:\n");
    while (fscanf(file, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        printf("运动员ID: %s, 项目名称: %s, 项目组别: %s, 项目性质: %s, 项目人数: %d, 学院ID: %s, 审核状态: %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
    }
    rewind(file);

    char athleteID[20], projectName[50];

    int found = 0;
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("无法打开临时文件\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if (registration.approval == 0) {
            found = 1;
            printf("其项目组别为: %s, 项目性质为: %s, 项目人数为: %d, 学院ID为: %s\n", registration.group, registration.nature, registration.athleteNumber, registration.departmentID);
            printf("请输入审核结果 (1: 通过, 2: 未通过): ");
            scanf("%d", &registration.approval);
        }
        fprintf(tempFile, "%s %s %s %s %d %s %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
    }

    if (found) {
        FILE *regFile = fopen("registrations.txt", "a");
        if (regFile == NULL) {
            printf("无法打开报名文件\n");
            fclose(tempFile);
            return;
        }
        fprintf(regFile, "%s %s %s %s %d %s %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
        fclose(regFile);
    }

    fclose(file);
    fclose(tempFile);

    remove("registrations.txt");
    rename("temp.txt", "registrations.txt");

    if (found) {
        printf("审核完成\n");
    } else {
        printf("已全部审核完毕\n");
    }
}


// 运动员菜单
void athleteMenu() {
    printf("欢迎运动员\n");
    int choice;
    while (1) {
        printf("1. 查询比赛项目\n");
        printf("2. 报名\n");
        printf("3. 修改报名\n");
        printf("4. 删除报名\n");
        printf("5. 查询最终参赛项目及赛程安排\n");
        printf("6. 退出登录\n");
        printf("请选择: ");
        scanf("%d", &choice);
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
        printf("1. 查询自己担任裁判工作的项目\n");
        printf("2. 查询运动会的赛程安排\n");
        printf("3. 退出登录\n");
        printf("请选择: ");
        scanf("%d", &choice);
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
        printf("1. 增加比赛项目\n");
        printf("2. 修改比赛项目\n");
        printf("3. 删除比赛项目\n");
        printf("4. 返回\n");
        printf("请选择: ");
        scanf("%d", &choice);
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
    printf("请输入项目名称: ");
    scanf("%s", project.projectName);
    FILE *file = fopen("projects.txt", "a");
    if (file == NULL) {
        printf("无法打开项目文件\n");
        return;
    }
    int choice;
    printf("请输入新的组别(用数字代替): \n");
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
    scanf("%d", &choice);

    switch (choice){
    case 1:
        strcpy(project.group, "Faculty_Senior_Men's_Group");
        break;
    case 2:
        strcpy(project.group, "Faculty_Senior_Women's_Group");
        break;
    case 3:
        strcpy(project.group, "Faculty_Middle-aged_Men's_Group");
        break;
    case 4:
        strcpy(project.group, "Faculty_Middle-aged_Women's_Group");
        break;
    case 5:
        strcpy(project.group, "Faculty_Young_Men's_Group");
        break;
    case 6:
        strcpy(project.group, "Faculty_Young_Women's_Group");
        break;
    case 7:
        strcpy(project.group, "Graduate_Men's_Group");
        break;
    case 8:
        strcpy(project.group, "Graduate_Women's_Group");
        break;
    case 9:
        strcpy(project.group, "Undergraduate_Men's_Group");
        break;
    case 10:
        strcpy(project.group, "Undergraduate_Women's_Group");
        break;
    case 11:
        strcpy(project.group, "Mixed_Group");
        break;            
    default:
        printf("无效的选择\n按任意键返回\n");
        addProject();
    }
    printf("请输入新的项目性质: \n");
    printf("1.个人赛\n");
    printf("2.团体赛\n");

    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        strcpy(project.nature, "individual");
        break;
    case 2:
        strcpy(project.nature, "team");
        break;
    default:
        addProject();
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
        printf("请输入裁判员: ");
        scanf("%s", project.referee);

        rewind(userFile);
        while (fscanf(userFile, "%s %s %d", user.username, user.password, &user.role) != EOF) {
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
    
    printf("请输入其他信息: ");
    scanf("%s", project.otherInfo);
    fprintf(file, "%s %s %s %d %d %s %s %s\n", project.projectName, project.group, project.nature, project.maxParticipants, project.minParticipants, project.rules, project.referee, project.otherInfo);
    fclose(file);
    printf("项目添加成功\n");
    return;
}

// 修改比赛项目函数
void modifyProject() {
    deleteProject();
    addProject();
}

// 删除比赛项目函数
void deleteProject() {
    char projectName[50];
    Project project;
    FILE *file = fopen("projects.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("无法打开项目文件\n");
        return;
    }

    while (fscanf(file, "%s %s %s %d %d %s %s %s", project.projectName, project.group, project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee, project.otherInfo) != EOF) {
        printf("项目名称：%s\n", project.projectName);
    }
    rewind(file);

    printf("请输入要删除的项目名称: ");
    scanf("%s", projectName);

    int found = 0;
    while (fscanf(file, "%s %s %s %d %d %s %s %s", project.projectName, project.group, project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee, project.otherInfo) != EOF) {
        if (strcmp(project.projectName, projectName) != 0) {
            fprintf(tempFile, "%s %s %s %d %d %s %s %s\n", project.projectName, project.group, project.nature, project.maxParticipants, project.minParticipants, project.rules, project.referee, project.otherInfo);
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
        if (strcmp(schedule.projectName, projectName) != 0) {
            fprintf(tempScheduleFile, "%s %s %s %s\n", schedule.projectName, schedule.date, schedule.time, schedule.venue);
        }
    }

    fclose(scheduleFile);
    fclose(tempScheduleFile);

    remove("schedule.txt");
    rename("temp_schedule.txt", "schedule.txt");

    if (found) {
        printf("项目删除成功\n");
    } else {
        printf("未找到该项目\n");
    }
}

// 查询比赛项目函数
void queryByProject() {
    char projectName[50];
    Project project;
    FILE *file = fopen("projects.txt", "r");
    if (file == NULL) {
        printf("无法打开项目文件\n");
        return;
    }

    while (fscanf(file, "%s %s %s %d %d %s %s %s", project.projectName, project.group, project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee, project.otherInfo) != EOF) {
        printf("项目名称: %s\n", project.projectName);
    }
    
    rewind(file);
    printf("请输入要查询的项目名称: ");
    scanf("%s", projectName);

    int found = 0;
    while (fscanf(file, "%s %s %s %d %d %s %s %s", project.projectName, project.group, project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee, project.otherInfo) != EOF) {
        if (strcmp(project.projectName, projectName) == 0) {
            found = 1;
            printf("项目名称: %s\n", project.projectName);
            printf("组别: %s\n", project.group);
            printf("项目性质: %s\n", project.nature);
            printf("参赛人数上限: %d\n", project.maxParticipants);
            printf("参赛人数下限: %d\n", project.minParticipants);
            printf("比赛规则: %s\n", project.rules);
            printf("裁判员: %s\n", project.referee);
            printf("其他信息: %s\n", project.otherInfo);
            break;
        }
    }

    fclose(file);
        if (!found) {
            printf("未找到该项目\n");
        }
}
// 管理用户信息函数
void manageUsers() {
    int choice;
    while (1) {
        printf("1. 增加用户\n");
        printf("2. 修改用户\n");
        printf("3. 删除用户\n");
        printf("4. 返回\n");
        printf("请选择: ");
        scanf("%d", &choice);
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
    User user;
    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("无法打开用户文件\n");
        return;
    }

    printf("请输入角色 (0: 管理员, 1: 运动员, 2: 裁判员): ");
    scanf("%d", &user.role);

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
        while (fscanf(existingFile, "%s %s %d", existingUser.username, existingUser.password, &existingUser.role) != EOF) {
            if (strcmp(existingUser.username, user.username) == 0) {
                printf("用户已存在\n");
                fclose(existingFile);
                fclose(file);
                return;
            }
        }
        fclose(existingFile);
    }

    fprintf(file, "%s %s %d\n", user.username, user.password, user.role);
    fclose(file);
    printf("用户添加成功\n");
}

// 修改用户函数
void modifyUser() {
    deleteUser();
    addUser();
}

// 删除用户函数
void deleteUser() {
    char username[20];
    User user;
    FILE *file = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("无法打开用户文件\n");
        return;
    }

    while (fscanf(file, "%s %s %d", user.username, user.password, &user.role) != EOF) {
        printf("用户名：%s\n", user.username);
    }
    rewind(file);

    printf("请输入要删除的用户名: ");
    scanf("%s", username);

    int found = 0;
    while (fscanf(file, "%s %s %d", user.username, user.password, &user.role) != EOF) {
        if (strcmp(user.username, username) != 0) {
            fprintf(tempFile, "%s %s %d\n", user.username, user.password, user.role);
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
    char username[20];
    User user;
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("无法打开用户文件\n");
        return;
    }

    while (fscanf(file, "%s %s %d", user.username, user.password, &user.role) != EOF) {
        printf("用户名: %s\n", user.username);
    }
    rewind(file);

    printf("请输入要查询的用户名: ");
    scanf("%s", username);

    int found = 0;
    while (fscanf(file, "%s %s %d", user.username, user.password, &user.role) != EOF) {
        if (strcmp(user.username, username) == 0) {
            found = 1;
            printf("用户名: %s\n", user.username);
            printf("密码: %s\n", user.password);
            printf("角色: %d\n", user.role);
            break;
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该用户\n");
    }
}

void manageSchdule(){
    int choice;
    while (1) {
        printf("1. 增加赛程\n");
        printf("2. 修改赛程\n");
        printf("3. 删除赛程\n");
        printf("4. 返回\n");
        printf("请选择: ");
        scanf("%d", &choice);
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

    printf("请选择项目名称:\n");
    while (fscanf(projectFile, "%s %s %s %d %d %s %s %s", project.projectName, project.group, project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee, project.otherInfo) != EOF) {
        printf("%s\n", project.projectName);
    }
    rewind(projectFile);

    int found = 0;
    while (!found) {
        printf("请输入项目名称: ");
        scanf("%s", schedule.projectName);

        while (fscanf(projectFile, "%s %s %s %d %d %s %s %s", project.projectName, project.group, project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee, project.otherInfo) != EOF) {
            if (strcmp(schedule.projectName, project.projectName) == 0) {
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

    fprintf(file, "%s %s %s %s\n", schedule.projectName, schedule.date, schedule.time, schedule.venue);
    fclose(file);
    printf("赛程安排成功\n");
}

void modifySchedule() {
    deleteSchedule();
    addSchedule();
}

void deleteSchedule() {
    char projectName[50];
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }

    while (fscanf(file, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
        printf("项目名称：%s\n", schedule.projectName);
    }
    rewind(file);

    printf("请输入要删除的项目名称: ");
    scanf("%s", projectName);

    int found = 0;
    while (fscanf(file, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
        if (strcmp(schedule.projectName, projectName) != 0) {
            fprintf(tempFile, "%s %s %s %s\n", schedule.projectName, schedule.date, schedule.time, schedule.venue);
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
}

// 查询函数
void query() {
    // 查询的具体实现
    int choice;
    while (1) {
        printf("1. 按比赛项目查询\n");
        printf("2. 按用户查询\n");
        printf("3. 按学院查询\n");
        printf("4. 按运动员查询\n");
        printf("5. 按场地查询\n");
        printf("6. 按时间查询\n");
        printf("7. 返回\n");
        printf("请选择: ");
        scanf("%d", &choice);
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
    char departmentID[20];
    Registration registration;
    FILE *file = fopen("registrations.txt", "r");
    if (file == NULL) {
        printf("无法打开报名文件\n");
        return;
    }

    while (fscanf(file, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        printf("学院ID: %s\n", registration.departmentID);
    }
    rewind(file);

    printf("请输入要查询的学院ID: ");
    scanf("%s", departmentID);

    int found = 0;
    while (fscanf(file, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if (strcmp(registration.departmentID, departmentID) == 0) {
            found = 1;
            printf("运动员ID: %s\n", registration.athleteID);
            printf("项目名称: %s\n", registration.projectName);
            printf("审核状态: %d\n");
            printf("运动员编号: %d\n", registration.athleteNumber);
            printf("学院ID: %s\n", registration.departmentID);
            printf("审核状态: %d\n", registration.approval);
            printf("组别: %s\n", registration.group);
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该学院的运动员\n");
    }
}

void queryByAthlete() {
    char athleteID[20];
    Registration registration;
    FILE *file = fopen("registrations.txt", "r");
    if (file == NULL) {
        printf("无法打开报名文件\n");
        return;
        }

        // 用于记录已经显示过的运动员ID
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
        rewind(file);

        printf("请输入要查询的运动员ID: ");
        scanf("%s", athleteID);

    int found = 0;
    while (fscanf(file, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if (strcmp(registration.athleteID, athleteID) == 0) {
            found = 1;
            printf("运动员ID: %s\n", registration.athleteID);
            printf("项目名称: %s\n", registration.projectName);
            printf("审核状态: %d\n");
            printf("运动员编号: %d\n", registration.athleteNumber);
            printf("学院ID: %s\n", registration.departmentID);
            printf("审核状态: %d\n", registration.approval);
            printf("组别: %s\n", registration.group);
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该运动员\n");
    }
}

void queryByVenue() {
    char venue[50];
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }

     while (fscanf(file, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
        printf("场地: %s\n", schedule.venue);
    }
    rewind(file);

    printf("请输入要查询的场地: ");
    scanf("%s", venue);

    int found = 0;
    while (fscanf(file, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
        if (strcmp(schedule.venue, venue) == 0) {
            found = 1;
            printf("项目名称: %s\n", schedule.projectName);
            printf("日期: %s\n", schedule.date);
            printf("时间: %s\n", schedule.time);
            printf("场地: %s\n", schedule.venue);
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该场地的赛程\n");
    }
}

void queryByTime() {
    char date[20], time[10];
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }   

    while (fscanf(file, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
        printf("日期: %s\n", schedule.date);
        printf("时间: %s\n", schedule.time);
    }
    rewind(file);

    printf("请输入要查询的日期 (YYYY-MM-DD): ");
    scanf("%s", date);
    printf("请输入要查询的时间 (HH:MM): ");
    scanf("%s", time);

    int found = 0;
    while (fscanf(file, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
        if (strcmp(schedule.date, date) == 0 && strcmp(schedule.time, time) == 0) {
            found = 1;
            printf("项目名称: %s\n", schedule.projectName);
            printf("日期: %s\n", schedule.date);
            printf("时间: %s\n", schedule.time);
            printf("场地: %s\n", schedule.venue);
        }
    }

    fclose(file);
    if (!found) {
        printf("未找到该时间的赛程\n");
    }
}

void queryReviewProgress() {
    Registration registration;
    FILE *file = fopen("registrations.txt", "r");
    if (file == NULL) {
        printf("无法打开报名文件\n");
        return;
    }

    printf("审核进度:\n");
    while (fscanf(file, "%s %s %d %d", registration.athleteID, registration.projectName, &registration.athleteNumber) != EOF) {
        printf("运动员ID: %s, 项目名称: %s, 审核状态: %d, 运动员编号: %d\n", registration.athleteID, registration.projectName, registration.athleteNumber);
    }

    fclose(file);
}

void queryRefereeProjects(){
    
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
    printf("裁判员 %s 担任裁判工作的项目:\n", refereeName);
    while (fscanf(projectFile, "%s %s %s %d %d %s %s %s", project.projectName, project.group, project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee, project.otherInfo) != EOF) {
        if (strcmp(project.referee, refereeName) == 0) {
            printf("项目名称: %s\n", project.projectName);
            while (fscanf(scheduleFile, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
                if (strcmp(schedule.projectName, project.projectName) == 0) {
                    printf("比赛时间: %s %s\n", schedule.date, schedule.time);
                    printf("比赛地点: %s\n", schedule.venue);
                    break;
                }
            }
            rewind(scheduleFile);

            printf("参赛运动员:\n");
            while (fscanf(registrationFile, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
                if (strcmp(registration.projectName, project.projectName) == 0 && registration.approval == 1) {
                    printf("运动员ID: %s, 运动员编号: %d\n", registration.athleteID, registration.athleteNumber);
                }
            }
            rewind(registrationFile);
        }
    }

    fclose(projectFile);
    fclose(scheduleFile);
    fclose(registrationFile);
}

void querySchedule(){
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }

    printf("运动会赛程安排:\n");
    while (fscanf(file, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
        printf("项目名称: %s\n", schedule.projectName);
        printf("日期: %s\n", schedule.date);
        printf("时间: %s\n", schedule.time);
        printf("场地: %s\n", schedule.venue);
        printf("-------------------------\n");
    }

    fclose(file);
}

void addRegistration() {
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

    printf("请选择项目名称:\n");
    while (fscanf(projectFile, "%s %s %s %d %d %s %s %s", project.projectName, project.group, project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee, project.otherInfo) != EOF) {
        printf("%s\n", project.projectName);
    }
    rewind(projectFile);

    int found = 0;
    while (!found) {
        printf("请输入项目名称: ");
        scanf("%s", registration.projectName);

        while (fscanf(projectFile, "%s %s %s %d %d %s %s %s", project.projectName, project.group, project.nature, &project.maxParticipants, &project.minParticipants, project.rules, project.referee, project.otherInfo) != EOF) {
            if (strcmp(registration.projectName, project.projectName) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("无效的项目名称，请重新输入。\n");
            rewind(projectFile);
        }
    }

    int participantCount = 0;
    Registration tempReg;
    FILE *tempFile = fopen("registrations.txt", "r");
    if (tempFile != NULL) {
        while (fscanf(tempFile, "%s %s %s %s %d %s %d", tempReg.athleteID, tempReg.projectName, tempReg.group, tempReg.nature, &tempReg.athleteNumber, tempReg.departmentID, &tempReg.approval) != EOF) {
            if (strcmp(tempReg.projectName, registration.projectName) == 0) {
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

    printf("请输入新的组别(用数字代替): \n");
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
    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            strcpy(registration.group, "Faculty_Senior_Men's_Group");
            break;
        case 2:
            strcpy(registration.group, "Faculty_Senior_Women's_Group");
            break;
        case 3:
            strcpy(registration.group, "Faculty_Middle-aged_Men's_Group");
            break;
        case 4:
            strcpy(registration.group, "Faculty_Middle-aged_Women's_Group");
            break;
        case 5:
            strcpy(registration.group, "Faculty_Young_Men's_Group");
            break;
        case 6:
            strcpy(registration.group, "Faculty_Young_Women's_Group");
            break;
        case 7:
            strcpy(registration.group, "Graduate_Men's_Group");
            break;
        case 8:
            strcpy(registration.group, "Graduate_Women's_Group");
            break;
        case 9:
            strcpy(registration.group, "Undergraduate_Men's_Group");
            break;
        case 10:
            strcpy(registration.group, "Undergraduate_Women's_Group");
            break;
        case 11:
            strcpy(registration.group, "Mixed_Group");
            break;
        default:
            printf("无效的选择\n按任意键返回\n");
            fclose(file);
            fclose(projectFile);
            return;
    }

    printf("请输入项目性质: \n");
    printf("1.个人赛\n");
    printf("2.团体赛\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            strcpy(registration.nature, "individual");
            break;
        case 2:
            strcpy(registration.nature, "team");
            break;
        default:
            fclose(file);
            fclose(projectFile);
            return;
    }

    int maxAthleteNumber = 0;
    tempFile = fopen("registrations.txt", "r");
    if (tempFile != NULL) {
        while (fscanf(tempFile, "%s %s %d %s %s", tempReg.athleteID, tempReg.projectName, &tempReg.athleteNumber, tempReg.group, tempReg.nature) != EOF) {
            if (tempReg.athleteNumber > maxAthleteNumber) {
                maxAthleteNumber = tempReg.athleteNumber;
            }
        }
        fclose(tempFile);
    } else {
        printf("无法打开文件\n");
    }
    registration.athleteNumber = maxAthleteNumber + 1;

    displayAllDepartments();

    printf("请输入学院ID: ");
    scanf("%s", registration.departmentID);
    registration.approval = 0;
    fprintf(file, "%s %s %s %s %d %s %d\n", athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
    fclose(file);
    fclose(projectFile);
    printf("报名成功\n");
}

void modifyRegistration(){
    deleteRegistration();
    addRegistration();
}

void queryAthleteSchedule(){
    Schedule schedule;
    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("无法打开赛程文件\n");
        return;
    }

    printf("运动员 %s 的赛程安排:\n", athleteID);
    int found = 0;
    while (fscanf(file, "%s %s %s %s", schedule.projectName, schedule.date, schedule.time, schedule.venue) != EOF) {
        Registration registration;
        FILE *regFile = fopen("registrations.txt", "r");
        if (regFile == NULL) {
            printf("无法打开报名文件\n");
            fclose(file);
            return;
        }

        while (fscanf(regFile, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
            if (strcmp(registration.athleteID, athleteID) == 0 && strcmp(registration.projectName, schedule.projectName) == 0) {
                found = 1;
                printf("项目名称: %s\n", schedule.projectName);
                printf("日期: %s\n", schedule.date);
                printf("时间: %s\n", schedule.time);
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
}

void deleteRegistration() {
    char athleteID[20], projectName[50];
    Registration registration;
    FILE *file = fopen("registrations.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("无法打开报名文件\n");
        return;
    }

    while (fscanf(file, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        printf("运动员ID: %s, 项目名称: %s, 项目组别: %s, 项目性质: %s, 项目人数: %d, 学院ID: %s, 审核状态: %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
    }
    rewind(file);

    printf("请输入要删除的运动员ID: ");
    scanf("%s", athleteID);
    printf("请输入要删除的项目名称: ");
    scanf("%s", projectName);

    int judge,found = 0;
    while (fscanf(file, "%s %s %s %s %d %s %d", registration.athleteID, registration.projectName, registration.group, registration.nature, &registration.athleteNumber, registration.departmentID, &registration.approval) != EOF) {
        if ((strcmp(registration.athleteID, athleteID) == 0 && strcmp(registration.projectName, projectName) == 0) && registration.approval == 0) {
            printf("其项目组别为: %s, 项目性质为: %s, 项目人数为: %d, 学院ID为: %s\n", registration.group, registration.nature, registration.athleteNumber, registration.departmentID);
            printf("确认删除? (0: 否, 1: 是): ");
            scanf("%d", &judge);
            if(!judge){
                found--;
                fprintf(tempFile, "%s %s %s %s %d %s %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
            } else {
                found = 100;
            }
        } else {
            fprintf(tempFile, "%s %s %s %s %d %s %d\n", registration.athleteID, registration.projectName, registration.group, registration.nature, registration.athleteNumber, registration.departmentID, registration.approval);
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
}

void displayAllDepartments() {
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
    }