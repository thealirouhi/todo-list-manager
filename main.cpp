#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class Task {
private:
    int id;
    string title;
    bool done;
    vector<int> deps;

public:
    Task(int i, string t) : id(i), title(t), done(false) {}

    int getId() { return id; }
    string getTitle() { return title; }
    bool isDone() { return done; }
    vector<int>& getDeps() { return deps; }

    void addDep(int d) { deps.push_back(d); }
    void markDone() { done = true; }
};

class TodoList {
private:
    vector<Task> tasks;
    string name;

    Task* findTask(int id) {
        for (auto &t : tasks)
            if (t.getId() == id) return &t;
        return nullptr;
    }

    bool isDependedOn(int id) {
        for (auto &t : tasks)
            for (int d : t.getDeps())
                if (d == id) return true;
        return false;
    }

public:
    void newList(string n) {
        name = n;
        tasks.clear();
        cout << "List '" << name << "' created.\n";
    }

    void addTask(int id, string title) {
        if (findTask(id)) {
            cout << "Error: task " << id << " already exists.\n";
            return;
        }
        tasks.push_back(Task(id, title));
        cout << "Task " << id << " added.\n";
    }

    void addTaskDep(int id, string title, vector<int> deps) {
        if (findTask(id)) {
            cout << "Error: task " << id << " already exists.\n";
            return;
        }

        for (int d : deps) {
            if (!findTask(d)) {
                cout << "Error: dependency " << d << " not found.\n";
                return;
            }
        }

        Task t(id, title);
        for (int d : deps) t.addDep(d);
        tasks.push_back(t);

        cout << "Task " << id << " added with " << deps.size() << " deps.\n";
    }

    void doneTask(int id) {
        Task* t = findTask(id);

        if (!t) {
            cout << "Error: task " << id << " not found.\n";
            return;
        }

        for (int d : t->getDeps()) {
            Task* dt = findTask(d);
            if (!dt || !dt->isDone()) {
                cout << "Cannot complete task " << id
                     << ": prerequisites not met.\n";
                return;
            }
        }

        t->markDone();
        cout << "Task " << id << " marked as done.\n";
    }

    void status(int id) {
        Task* t = findTask(id);

        if (!t) {
            cout << "Error: task " << id << " not found.\n";
            return;
        }

        cout << "Task " << id << ": " << t->getTitle() << "\n";
        cout << "Status: " << (t->isDone() ? "done" : "pending") << "\n";

        auto deps = t->getDeps();
        if (deps.empty()) return;

        sort(deps.begin(), deps.end());

        cout << "Dependencies: ";

        for (int i = 0; i < deps.size(); i++) {
            Task* dt = findTask(deps[i]);
            if (!dt) continue;

            cout << deps[i] << " (" << dt->getTitle() << ")";
            if (i != deps.size() - 1) cout << ", ";
        }

        cout << "\n";
    }

    void printAll() {
        vector<Task*> v;
        for (auto &t : tasks) v.push_back(&t);

        sort(v.begin(), v.end(),
             [](Task* a, Task* b) { return a->getId() < b->getId(); });

        for (auto t : v) {
            cout << (t->isDone() ? "[done] " : "[pending] ");
            cout << t->getId() << ": " << t->getTitle() << "\n";
        }
    }

    void pending() {
        vector<Task*> v;
        for (auto &t : tasks) v.push_back(&t);

        sort(v.begin(), v.end(),
             [](Task* a, Task* b) { return a->getId() < b->getId(); });

        bool first = true;

        for (auto t : v) {
            if (!t->isDone()) {
                if (first) {
                    cout << "Pending tasks: ";
                    first = false;
                } else {
                    cout << ", ";
                }
                cout << t->getId() << " (" << t->getTitle() << ")";
            }
        }

        if (!first) cout << "\n";
    }

    void ready() {
        vector<Task*> v;
        for (auto &t : tasks) v.push_back(&t);

        sort(v.begin(), v.end(),
             [](Task* a, Task* b) { return a->getId() < b->getId(); });

        bool first = true;

        for (auto t : v) {
            if (t->isDone()) continue;

            bool ok = true;
            for (int d : t->getDeps()) {
                Task* dt = findTask(d);
                if (!dt || !dt->isDone()) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                if (first) {
                    cout << "Ready tasks: ";
                    first = false;
                } else {
                    cout << " ";
                }
                cout << t->getId();
            }
        }

        if (!first) cout << "\n";
    }

    void removeTask(int id) {
        if (!findTask(id)) {
            cout << "Error: task " << id << " not found.\n";
            return;
        }

        if (isDependedOn(id)) {
            cout << "Cannot remove task " << id
                 << ": other tasks depend on it.\n";
            return;
        }

        // cleanup dependencies
        for (auto &t : tasks) {
            auto &d = t.getDeps();
            d.erase(remove(d.begin(), d.end(), id), d.end());
        }

        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].getId() == id) {
                tasks.erase(tasks.begin() + i);
                break;
            }
        }

        cout << "Task " << id << " removed.\n";
    }
};

int main() {
    TodoList list;
    string line;

    while (getline(cin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "NEW_LIST") {
            string name;
            ss >> name;
            list.newList(name);
        }

        else if (cmd == "ADD") {
            int id;
            string title;
            ss >> id >> title;
            list.addTask(id, title);
        }

        else if (cmd == "ADD_DEP") {
            int id;
            string title;
            ss >> id >> title;

            vector<int> deps;
            int x;
            while (ss >> x) deps.push_back(x);

            list.addTaskDep(id, title, deps);
        }

        else if (cmd == "DONE") {
            int id;
            ss >> id;
            list.doneTask(id);
        }

        else if (cmd == "STATUS") {
            int id;
            ss >> id;
            list.status(id);
        }

        else if (cmd == "PRINT_ALL") {
            list.printAll();
        }

        else if (cmd == "PENDING") {
            list.pending();
        }

        else if (cmd == "READY") {
            list.ready();
        }

        else if (cmd == "REMOVE") {
            int id;
            ss >> id;
            list.removeTask(id);
        }

        else if (cmd == "EXIT") {
            break;
        }
    }

    return 0;
}