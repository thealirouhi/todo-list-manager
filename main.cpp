#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Task {
private:
    int id;
    string title;
    bool done;
    vector<int> dependencies;

public:
    Task(int taskId, const string& taskTitle)
        : id(taskId), title(taskTitle), done(false) {}

    int getId() const {
        return id;
    }

    string getTitle() const {
        return title;
    }

    bool isDone() const {
        return done;
    }

    void markDone() {
        done = true;
    }

    void addDependency(int depId) {
        dependencies.push_back(depId);
    }

    vector<int> getDependencies() const {
        return dependencies;
    }
};

class TodoList {
private:
    string name;
    vector<Task> tasks;

    Task* findTask(int id) {
        for (auto& task : tasks) {
            if (task.getId() == id) {
                return &task;
            }
        }
        return nullptr;
    }

public:
    void createList(const string& listName) {
        name = listName;
        tasks.clear();
        cout << "List '" << name << "' created." << endl;
    }

    void addTask(int id, const string& title) {
        if (findTask(id) != nullptr) {
            cout << "Error: task " << id << " already exists." << endl;
            return;
        }

        tasks.push_back(Task(id, title));
        cout << "Task " << id << " added." << endl;
    }
};

int main() {
    TodoList todo;
    string line;

    while (getline(cin, line)) {
        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "EXIT") {
            break;
        }
        else if (command == "NEW_LIST") {
            string listName;
            ss >> listName;
            todo.createList(listName);
        }
        else if (command == "ADD") {
            int id;
            string title;
            ss >> id >> title;
            todo.addTask(id, title);
        }
    }

    return 0;
}