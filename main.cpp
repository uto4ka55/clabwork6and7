#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct LossInfo {
    string date;
    int personnelLoss;
    int tankLoss;
    int aircraftLoss;
    LossInfo* next;
    LossInfo* prev;
};

LossInfo* createLoss(string date, int personnelLoss, int tankLoss, int aircraftLoss) {
    LossInfo* newLoss = new LossInfo;
    newLoss->date = date;
    newLoss->personnelLoss = personnelLoss;
    newLoss->tankLoss = tankLoss;
    newLoss->aircraftLoss = aircraftLoss;
    newLoss->next = nullptr;
    newLoss->prev = nullptr;
    return newLoss;
}

void addLoss(LossInfo*& head, LossInfo*& tail, string date, int personnelLoss, int tankLoss, int aircraftLoss) {
    LossInfo* newLoss = createLoss(date, personnelLoss, tankLoss, aircraftLoss);

    if (head == nullptr) {
        head = newLoss;
        tail = newLoss;
    } else if (date < head->date) {
        newLoss->next = head;
        head->prev = newLoss;
        head = newLoss;
    } else {
        LossInfo* current = head;
        while (current->next != nullptr && current->next->date < date) {
            current = current->next;
        }
        newLoss->next = current->next;
        if (current->next != nullptr) {
            current->next->prev = newLoss;
        }
        current->next = newLoss;
        newLoss->prev = current;
        if (newLoss->next == nullptr) {
            tail = newLoss;
        }
    }
}

void updateLoss(LossInfo* head, string date, int personnelLoss, int tankLoss, int aircraftLoss) {
    LossInfo* current = head;
    while (current != nullptr) {
        if (current->date == date) {
            current->personnelLoss = personnelLoss;
            current->tankLoss = tankLoss;
            current->aircraftLoss = aircraftLoss;
            return;
        }
        current = current->next;
    }
    cout << "No loss info for date " << date << " found in the list." << endl;
}

void printLosses(LossInfo* head) {
    LossInfo* current = head;
    while (current != nullptr) {
        cout << "Date: " << current->date << ", Personnel Loss: " << current->personnelLoss
             << ", Tank Loss: " << current->tankLoss << ", Aircraft Loss: " << current->aircraftLoss << endl;
        current = current->next;
    }
}

void printLossesByDate(LossInfo* head, string date) {
    LossInfo* current = head;
    while (current != nullptr) {
        if (current->date == date) {
            cout << "Date: " << current->date << ", Personnel Loss: " << current->personnelLoss
                 << ", Tank Loss: " << current->tankLoss << ", Aircraft Loss: " << current->aircraftLoss << endl;
        }
        current = current->next;
    }
}

int totalPersonnelLoss(LossInfo* head) {
    int total = 0;
    LossInfo* current = head;
    while (current != nullptr) {
        total += current->personnelLoss;
        current = current->next;
    }
    return total;
}

string dateWithGreatestPersonnelLoss(LossInfo* head) {
    int maxLoss = 0;
    string date;
    LossInfo* current = head;
    while (current != nullptr) {
        if (current->personnelLoss > maxLoss) {
            maxLoss = current->personnelLoss;
            date = current->date;
        }
        current = current->next;
    }
    return date;
}

void removeNoAircraftDays(LossInfo*& head, LossInfo*& tail) {
    LossInfo* current = head;
    while (current != nullptr) {
        if (current->aircraftLoss == 0) {
            if (current == head) {
                head = head->next;
                if (head != nullptr)
                    head->prev = nullptr;
            } else if (current == tail) {
                tail = tail->prev;
                tail->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
        }
        current = current->next;
    }
}

void freeMemory(LossInfo* head) {
    while (head != nullptr) {
        LossInfo* temp = head;
        head = head->next;
        delete temp;
    }
}

void writeLossesToFile(LossInfo* head, const string& filename) {
    ofstream file(filename); // This will clear the file before writing
    LossInfo* current = head;
    while (current != nullptr) {
        file << current->date << " " << current->personnelLoss << " "
             << current->tankLoss << " " << current->aircraftLoss << "\n";
        current = current->next;
    }
    file.close();
}

void readLossesFromFile(LossInfo*& head, LossInfo*& tail, const string& filename) {
    ifstream file(filename);
    string date;
    int personnelLoss, tankLoss, aircraftLoss;
    while (file >> date >> personnelLoss >> tankLoss >> aircraftLoss) {
        addLoss(head, tail, date, personnelLoss, tankLoss, aircraftLoss);
    }
    file.close();
}

int main(){
    const string choice[] = {
            "[1] Print all losses",
            "[2] Create a new loss info for a specific date",
            "[3] Update loss info for a specific date",
            "[4] Print losses for a specific date",
            "[5] Calculate total personnel loss",
            "[6] Find the date with the greatest personnel loss",
            "[7] Remove days with no aircraft loss",
            "[8] Write losses to file",
            "[9] Exit"};
    int choiceNum;
    LossInfo* head = nullptr;
    LossInfo* tail = nullptr;
    readLossesFromFile(head, tail, "losses.txt");

    while (true){
        cout << endl<< "Menu:" << endl;
        for (int i = 0; i < 9; i++) {
            cout << choice[i] << endl;
        }
        cout << "Enter choice: ";
        cin >> choiceNum;
        cout << endl;

        switch (choiceNum) {
            case 1: {
                printLosses(head);
                break;
            }
            case 2: {
                string date;
                int personnelLoss, tankLoss, aircraftLoss;
                cout << "Enter date: ";
                cin >> date;
                cout << "Enter personnel loss: ";
                cin >> personnelLoss;
                cout << "Enter tank loss: ";
                cin >> tankLoss;
                cout << "Enter aircraft loss: ";
                cin >> aircraftLoss;
                addLoss(head, tail, date, personnelLoss, tankLoss, aircraftLoss);
                break;
            }
            case 3: {
                string date;
                int personnelLoss, tankLoss, aircraftLoss;
                cout << "Enter date: ";
                cin >> date;
                cout << "Enter personnel loss: ";
                cin >> personnelLoss;
                cout << "Enter tank loss: ";
                cin >> tankLoss;
                cout << "Enter aircraft loss: ";
                cin >> aircraftLoss;
                updateLoss(head, date, personnelLoss, tankLoss, aircraftLoss);
                break;
            }
            case 4: {
                string date;
                cout << "Enter date: ";
                cin >> date;
                printLossesByDate(head, date);
                break;
            }
            case 5: {
                cout << "Total personnel loss: " << totalPersonnelLoss(head) << endl;
                break;
            }
            case 6: {
                cout << "Date with greatest personnel loss: " << dateWithGreatestPersonnelLoss(head) << endl;
                break;
            }
            case 7: {
                removeNoAircraftDays(head, tail);
                cout << "Days with no aircraft loss removed" << endl;
                break;
            }
            case 8: {
                writeLossesToFile(head, "losses.txt");
                cout << "Losses written to file" << endl;
                break;
            }
            case 9: {
                freeMemory(head);
                cout << "Exiting program" << endl;
                return 0;
            }
            default: {
                cout << "Invalid choice" << endl;
                break;
            }
        }
    }
}