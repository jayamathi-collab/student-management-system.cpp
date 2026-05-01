#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <limits>
using namespace std;

class Student {
private:
    string name, id, dept;
    int year;
    double cgpa;

public:
    Student() {}

    Student(string n, string i, string d, int y, double c)
        : name(n), id(i), dept(d), year(y), cgpa(c) {}

    string getId() const { return id; }
    string getDept() const { return dept; }
    double getCGPA() const { return cgpa; }

    void update(string n, string d, int y, double c) {
        name = n;
        dept = d;
        year = y;
        cgpa = c;
    }

    void display() const {
        cout << "\n-------------------------\n";
        cout << "Name : " << name << "\n";
        cout << "ID   : " << id << "\n";
        cout << "Dept : " << dept << "\n";
        cout << "Year : " << year << "\n";
        cout << "CGPA : " << cgpa << "\n";
    }

    string serialize() const {
        return name + "|" + id + "|" + dept + "|" +
               to_string(year) + "|" + to_string(cgpa);
    }

    static Student deserialize(string line) {
        stringstream ss(line);
        string name, id, dept, yearStr, cgpaStr;

        getline(ss, name, '|');
        getline(ss, id, '|');
        getline(ss, dept, '|');
        getline(ss, yearStr, '|');
        getline(ss, cgpaStr, '|');

        return Student(name, id, dept, stoi(yearStr), stod(cgpaStr));
    }
};

class StudentManager {
private:
    vector<Student> students;
    const string filename = "students.txt";

public:
    void load() {
        ifstream file(filename);
        if (!file) return;

        string line;
        while (getline(file, line)) {
            students.push_back(Student::deserialize(line));
        }
    }

    void save() {
        ofstream file(filename);
        for (auto &s : students) {
            file << s.serialize() << "\n";
        }
    }

    bool idExists(string id) {
        return any_of(students.begin(), students.end(),
                      [&](Student &s) { return s.getId() == id; });
    }

    void addStudent() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string name, id, dept;
        int year;
        double cgpa;

        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter ID: ";
        cin >> id;

        if (idExists(id)) {
            cout << "ID already exists!\n";
            return;
        }

        cout << "Enter Department: ";
        cin >> dept;

        cout << "Enter Year: ";
        while (!(cin >> year) || year <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid year. Enter again: ";
        }

        cout << "Enter CGPA: ";
        while (!(cin >> cgpa) || cgpa < 0 || cgpa > 10) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid CGPA (0-10). Enter again: ";
        }

        students.emplace_back(name, id, dept, year, cgpa);
        save();
        cout << "Student added successfully!\n";
    }

    void displayAll() {
        if (students.empty()) {
            cout << "No records found.\n";
            return;
        }

        for (auto &s : students) s.display();
    }

    void updateStudent() {
        string id;
        cout << "Enter ID to update: ";
        cin >> id;

        for (auto &s : students) {
            if (s.getId() == id) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                string name, dept;
                int year;
                double cgpa;

                cout << "Enter new Name: ";
                getline(cin, name);

                cout << "Enter new Department: ";
                cin >> dept;

                cout << "Enter new Year: ";
                while (!(cin >> year) || year <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid year. Enter again: ";
                }

                cout << "Enter new CGPA: ";
                while (!(cin >> cgpa) || cgpa < 0 || cgpa > 10) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid CGPA. Enter again: ";
                }

                s.update(name, dept, year, cgpa);
                save();
                cout << "Updated successfully!\n";
                return;
            }
        }
        cout << "Student not found\n";
    }

    void deleteStudent() {
        string id;
        cout << "Enter ID to delete: ";
        cin >> id;

        char confirm;
        cout << "Are you sure? (y/n): ";
        cin >> confirm;

        if (confirm != 'y' && confirm != 'Y') {
            cout << "Deletion cancelled\n";
            return;
        }

        auto it = remove_if(students.begin(), students.end(),
                            [&](Student &s) { return s.getId() == id; });

        if (it != students.end()) {
            students.erase(it, students.end());
            save();
            cout << "Deleted successfully\n";
        } else {
            cout << "Student not found\n";
        }
    }

    void searchByID() {
        string id;
        cout << "Enter ID: ";
        cin >> id;

        for (auto &s : students) {
            if (s.getId() == id) {
                s.display();
                return;
            }
        }
        cout << "Not found\n";
    }

    void filterCGPA() {
        double min;
        cout << "Enter minimum CGPA: ";
        cin >> min;

        for (auto &s : students) {
            if (s.getCGPA() >= min)
                s.display();
        }
    }

    void sortByCGPA() {
        sort(students.begin(), students.end(),
             [](Student &a, Student &b) {
                 return a.getCGPA() > b.getCGPA();
             });
        cout << "Sorted successfully\n";
    }
};

bool login() {
    string user, pass;
    int attempts = 3;

    while (attempts--) {
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> pass;

        if (user == "admin" && pass == "1234")
            return true;

        cout << "Invalid credentials. Attempts left: "
             << attempts << "\n";
    }
    return false;
}

int main() {
    if (!login()) {
        cout << "Access Denied\n";
        return 0;
    }

    StudentManager sm;
    sm.load();

    int choice;
    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All\n";
        cout << "3. Update Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Search by ID\n";
        cout << "6. Filter by CGPA\n";
        cout << "7. Sort by CGPA\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: sm.addStudent(); break;
            case 2: sm.displayAll(); break;
            case 3: sm.updateStudent(); break;
            case 4: sm.deleteStudent(); break;
            case 5: sm.searchByID(); break;
            case 6: sm.filterCGPA(); break;
            case 7: sm.sortByCGPA(); break;
            case 8: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice\n";
        }

    } while (choice != 8);

    return 0;
}