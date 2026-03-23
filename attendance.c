#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
struct Student
{
    char regNo[20];
    char name[40];
    int totalClasses;
    int presentCount;
};

void addStudent();
void markAttendance();
void showReport();
void shortageList();
void classAverage();
void deleteStudent();   
int studentExists(char reg[]);
void toUpperCase(char str[]);

int main()
{
    setbuf(stdout,NULL);

    int choice;

    while(1)
    {

        printf("\n------ Attendance Manager ------\n");
        printf("1. Add Student\n");
        printf("2. Mark Attendance\n");
        printf("3. Show Attendance Report\n");
        printf("4. Students Below 75%%\n");
        printf("5. Class Average Attendance\n");
        printf("6. Delete Student\n");   
        printf("7. Exit\n");
        printf("Enter choice: ");

        scanf("%d",&choice);

        switch(choice)
        {
            case 1: addStudent(); break;
            case 2: markAttendance(); break;
            case 3: showReport(); break;
            case 4: shortageList(); break;
            case 5: classAverage(); break;
            case 6: deleteStudent(); break;  
            case 7: return 0;
            default: printf("Invalid choice\n");
        }
    }
}

void toUpperCase(char str[])
{
    for(int i=0; str[i]!='\0'; i++)
        str[i] = toupper(str[i]);
}

int studentExists(char reg[])
{
    FILE *fp;
    struct Student s;

    fp = fopen("attendance.txt","r");

    if(fp == NULL)
        return 0;

    while(fscanf(fp,"%s %s %d %d",
        s.regNo,s.name,&s.totalClasses,&s.presentCount) == 4)
    {
        if(strcmp(s.regNo,reg) == 0)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void addStudent()
{
    FILE *fp;
    struct Student s;

    printf("Enter Register Number: ");
    scanf("%s",s.regNo);
    toUpperCase(s.regNo);

    if(studentExists(s.regNo))
    {
        printf("Student already exists\n");
        return;
    }

    printf("Enter Name: ");
    scanf("%s",s.name);

    s.totalClasses = 0;
    s.presentCount = 0;

    fp = fopen("attendance.txt","a");

    if(fp == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    fprintf(fp,"%s %s %d %d\n",
            s.regNo,s.name,s.totalClasses,s.presentCount);

    fclose(fp);

    printf("Student added successfully\n");
}

void markAttendance()
{
    FILE *fp,*temp;
    struct Student s;
    char reg[20];
    char status;
    int found = 0;

    printf("Enter Register Number: ");
    scanf("%s",reg);
    toUpperCase(reg);

    printf("Enter Attendance (P/A): ");
    scanf(" %c",&status);
    status = toupper(status);

    fp = fopen("attendance.txt","r");
    if(fp == NULL)
    {
        printf("No records found\n");
        return;
    }

    temp = fopen("temp.txt","w");

    while(fscanf(fp,"%s %s %d %d",
        s.regNo,s.name,&s.totalClasses,&s.presentCount) == 4)
    {
        if(strcmp(s.regNo,reg) == 0)
        {
            found = 1;
            s.totalClasses++;

            if(status == 'P')
                s.presentCount++;
        }

        fprintf(temp,"%s %s %d %d\n",
                s.regNo,s.name,s.totalClasses,s.presentCount);
    }

    fclose(fp);
    fclose(temp);

    remove("attendance.txt");
    rename("temp.txt","attendance.txt");

    if(found)
        printf("Attendance updated\n");
    else
        printf("Student not found\n");
}

void deleteStudent()
{
    FILE *fp, *temp;
    struct Student s;
    char reg[20];
    int found = 0;

    printf("Enter Register Number to delete: ");
    scanf("%s", reg);
    toUpperCase(reg);

    fp = fopen("attendance.txt","r");

    if(fp == NULL)
    {
        printf("No records found\n");
        return;
    }

    temp = fopen("temp.txt","w");

    while(fscanf(fp,"%s %s %d %d",
        s.regNo,s.name,&s.totalClasses,&s.presentCount) == 4)
    {
        if(strcmp(s.regNo,reg) == 0)
        {
            found = 1;
            continue;
        }

        fprintf(temp,"%s %s %d %d\n",
                s.regNo,s.name,s.totalClasses,s.presentCount);
    }

    fclose(fp);
    fclose(temp);

    remove("attendance.txt");
    rename("temp.txt","attendance.txt");

    if(found)
        printf("Student deleted successfully\n");
    else
        printf("Student not found\n");
}

void showReport()
{
    FILE *fp;
    struct Student s;
    float percent;

    fp = fopen("attendance.txt","r");

    if(fp == NULL)
    {
        printf("No records found\n");
        return;
    }

    printf("\nRegNo\tName\tAttendance\n");

    while(fscanf(fp,"%s %s %d %d",
        s.regNo,s.name,&s.totalClasses,&s.presentCount) == 4)
    {
        if(s.totalClasses == 0)
            percent = 0;
        else
            percent = (s.presentCount*100.0)/s.totalClasses;

        printf("%s\t%s\t%.2f%%\n",
               s.regNo,s.name,percent);
    }

    fclose(fp);
}

void shortageList()
{
    FILE *fp;
    struct Student s;
    float percent;

    fp = fopen("attendance.txt","r");

    if(fp == NULL)
    {
        printf("No records found\n");
        return;
    }

    printf("\nStudents Below 75%% Attendance\n");

    while(fscanf(fp,"%s %s %d %d",
        s.regNo,s.name,&s.totalClasses,&s.presentCount) == 4)
    {
        if(s.totalClasses == 0)
            percent = 0;
        else
            percent = (s.presentCount*100.0)/s.totalClasses;

        if(percent < 75)
            printf("%s %s %.2f%%\n",s.regNo,s.name,percent);
    }

    fclose(fp);
}

void classAverage()
{
    FILE *fp;
    struct Student s;
    float percent,total=0;
    int count=0;

    fp = fopen("attendance.txt","r");

    if(fp == NULL)
    {
        printf("No records found\n");
        return;
    }

    while(fscanf(fp,"%s %s %d %d",
        s.regNo,s.name,&s.totalClasses,&s.presentCount) == 4)
    {
        if(s.totalClasses>0)
        {
            percent=(s.presentCount*100.0)/s.totalClasses;
            total+=percent;
            count++;
        }
    }

    fclose(fp);

    if(count==0)
        printf("No attendance data available\n");
    else
        printf("Class Average Attendance: %.2f%%\n",total/count);
}
