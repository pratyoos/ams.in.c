#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 20 // Maximum number of students that can be added

// Structure to store student data
typedef struct
{
    char name[30];
    int present;
    int absent;
} Student;


// name: saveDataToFile
// working: Saves the student data to a file by overwriting the existing data
//desc: This function takes the array of students and the number of students as input and saves the data to a file
void saveDataToFile(Student students[], int numStudents)
{
    FILE *file = fopen("attendance.bin", "wb");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    fwrite(students, sizeof(Student), numStudents, file);
    fclose(file);
}

// name: loadDataFromFile
// working: Loads the student data from a file
// desc: This function takes the array of students variable as input and loads the data from a file. It returns the number of students loaded from the file.
int loadDataFromFile(Student students[])
{
    FILE *file = fopen("attendance.bin", "rb");
    if (file == NULL)
    {
        return 0;
    }
    int numStudents = fread(students, sizeof(Student), MAX_STUDENTS, file);
    fclose(file);
    return numStudents;
}

// name: checkAttendance
// working: Displays the attendance of all the students
// desc: This function takes the array of students and the number of students as input and displays the attendance of all the students
void checkAttendance(Student students[], int numStudents)
{
    printf("Name\t\tPresent\t\tAbsent\n");
    for (int i = 0; i < numStudents; i++)
    {
        printf("%s\t\t%d\t\t%d\n", students[i].name, students[i].present, students[i].absent);
    }
}

// name: addStudent
// working: Adds a student to the attendance register
// desc: This function takes the array of students and the number of students as input and adds a student to the attendance register by taking the name as input from the user
void addStudent(Student students[], int *numStudents)
{
    int numToAdd;
    if (*numStudents == MAX_STUDENTS)
    {
        printf("Maximum number of students reached.\n");
        return;
    }

    printf("How many students you want to add: ");
    scanf("%d", &numToAdd);
    getchar(); // Consume the newline character left by scanf

    if (*numStudents + numToAdd > MAX_STUDENTS)
    {
        printf("Exceeding maximum number of students. Only %d students can be added.\n", MAX_STUDENTS - *numStudents);
        return;
    }

    for (int i = 0; i < numToAdd; i++)
    {
        printf("\nEnter student name to add in attendance register: ");
        fgets(students[*numStudents].name, sizeof(students[*numStudents].name), stdin); // Read the name using fgets
        strtok(students[*numStudents].name, "\n"); // Remove the trailing newline from fgets
        students[*numStudents].present = 0;
        students[*numStudents].absent = 0;
        (*numStudents)++;
    }
    saveDataToFile(students, *numStudents);
}

// name: findStudentIndex
// working: Finds the index of a student in the array of students
// desc: This function takes the array of students, the number of students and the name of the student as input and returns the index of the student in the array. If the student is not found, it returns -1.
int findStudentIndex(Student students[], int numStudents, const char *name)
{
    for (int i = 0; i < numStudents; i++)
    {
        if (strcmp(name, students[i].name) == 0)
        {
            return i;
        }
    }
    return -1; // Return -1 if the student is not found
}

// name: removeStudent
// working: Removes a student from the attendance register
// desc: This function takes the array of students and the number of students as input and removes a student from the attendance register by taking the name as input from the user
void removeStudent(Student students[], int *numStudents)
{
    char studentName[30];
    printf("Enter student name to remove: ");
    fgets(studentName, sizeof(studentName), stdin);
    strtok(studentName, "\n"); // Remove the trailing newline from fgets

    int index = findStudentIndex(students, *numStudents, studentName);
    if (index == -1)
    {
        printf("Student not found.\n");
        return;
    }

    for (int i = index; i < *numStudents - 1; i++)
    {
        strcpy(students[i].name, students[i + 1].name);
        students[i].present = students[i + 1].present;
        students[i].absent = students[i + 1].absent;
    }
    (*numStudents)--;
    saveDataToFile(students, *numStudents);
    printf("\n%s student is removed.\n", studentName);
}

// name: takeAttendance
// working: Takes the attendance of all the students
// desc: This function takes the array of students and the number of students as input and takes the attendance of all the students by taking the input from the user
void takeAttendance(Student students[], int numStudents)
{
    char ch;
    printf("\nEnter Y for present and N for absent\n");
    for (int i = 0; i < numStudents; i++)
    {
        printf("%d. %s is present: ", i + 1, students[i].name);
        scanf(" %c", &ch);
        getchar(); // Consume the newline character left by scanf

        if (ch == 'Y' || ch == 'y')
        {
            students[i].present += 1;
        }
        else if (ch == 'N' || ch == 'n')
        {
            students[i].absent += 1;
        }
        else
        {
            printf("Invalid character. Try again.\n");
            i--; // Retry the current student's attendance
        }
    }
    saveDataToFile(students, numStudents);
}

// main function
int main()
{
    Student students[MAX_STUDENTS]; // Array of students
    int numStudents = loadDataFromFile(students); //load the students from file

    int choose;
    do
    {
        // Display the menu
        printf("\n\n");
		printf("================= Main Menu ====================\n");
        printf("1. Add student\n");
        printf("2. Take attendance\n");
        printf("3. Check attendance\n");
        printf("4. Remove student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choose);
        getchar(); // Consume the newline character left by scanf

        // Perform the action based on the user's choice
        switch (choose)
        {
        case 1:
            addStudent(students, &numStudents);
            break;
        case 2:
            takeAttendance(students, numStudents);
            break;
        case 3:
            checkAttendance(students, numStudents);
            break;
        case 4:
            removeStudent(students, &numStudents);
            break;
        case 5:
            saveDataToFile(students, numStudents); // Save data before exiting
            printf("Exiting...\n");
            exit(0);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choose != 5);

    return 0;
}