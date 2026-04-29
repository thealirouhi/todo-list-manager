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

    int getId() const { return id; }
    string getTitle() const { return title; }
    bool isDone() const { return done; }

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
        cout << "List '" << name << "' created.\n";
    }

    void addTask(int id, const string& title) {
        if (findTask(id) != nullptr) {
            cout << "Error: task " << id << " already exists.\n";
            return;
        }

        tasks.emplace_back(id, title);
        cout << "Task " << id << " added.\n";
    }

    void listTasks() {
        if (tasks.empty()) {
            cout << "No tasks.\n";
            return;
        }

        for (const auto& task : tasks) {
            cout << task.getId() << " | "
                 << task.getTitle() << " | "
                 << (task.isDone() ? "done" : "pending");

            auto deps = task.getDependencies();
            if (!deps.empty()) {
                cout << " | deps: ";
                for (int d : deps) cout << d << " ";
            }

            cout << "\n";
        }
    }

    void markDone(int id) {
        Task* task = findTask(id);
        if (!task) {
            cout << "Task not found.\n";
            return;
        }
    
        // check dependencies
        for (int depId : task->getDependencies()) {
            Task* depTask = findTask(depId);
    
            if (!depTask) {
                cout << "Dependency " << depId << " not found.\n";
                return;
            }
    
            if (!depTask->isDone()) {
                cout << "Cannot complete task " << id
                     << ". Dependency " << depId << " is not done.\n";
                return;
            }
        }
    
        task->markDone();
        cout << "Task " << id << " marked done.\n";
    }

    void addDependency(int taskId, int depId) {
        Task* task = findTask(taskId);
        if (!task) {
            cout << "Task not found.\n";
            return;
        }

        task->addDependency(depId);
        cout << "Dependency added: " << taskId << " -> " << depId << "\n";
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
            ss >> id;

            string title;
            getline(ss, title);
            if (!title.empty() && title[0] == ' ')
                title = title.substr(1);

            todo.addTask(id, title);
        }

        else if (command == "LIST") {
            todo.listTasks();
        }

        else if (command == "DONE") {
            int id;
            ss >> id;
            todo.markDone(id);
        }

        else if (command == "DEP") {
            int taskId, depId;
            ss >> taskId >> depId;
            todo.addDependency(taskId, depId);
        }
    }

    return 0;
}