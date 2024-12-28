#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Colony Class
class Colony {
public:
    string name;
    string species;
    int workers;
    int warriors;
    int antKills;
    vector<string> colonyKills;
    int ticksAlive;
    bool alive;
    string killedBy;

    Colony(const string& colonyName, const string& colonySpecies) {
        name = colonyName;
        species = colonySpecies;
        workers = 0;
        warriors = 0;
        antKills = 0;
        ticksAlive = 0;
        alive = true;
        killedBy = "";
    }

    void addResources(int workerCount, int warriorCount) {
        if (!alive) return;
        workers += workerCount;
        warriors += warriorCount;
    }

    void incrementAntKills(int kills) {
        if (!alive) return;
        antKills += kills;
    }

    void addColonyKill(const string& colonyName) {
        if (!alive) return;
        colonyKills.push_back(colonyName);
    }

    void incrementTicksAlive() {
        if (alive) ticksAlive++;
    }

    void kill(const string& attackerName) {
        alive = false;
        killedBy = attackerName;
    }

    void showSummary() const {
        cout << "Species: " << species << endl;
        cout << "Workers: " << workers << endl;
        cout << "Warriors: " << warriors << endl;
        cout << "Ant Kills: " << antKills << endl;
        cout << "Colony Kills: " << colonyKills.size();
        if (!colonyKills.empty()) {
            cout << " (";
            for (size_t i = 0; i < colonyKills.size(); ++i) {
                cout << colonyKills[i];
                if (i < colonyKills.size() - 1) cout << ", ";
            }
            cout << ")";
        }
        cout << endl;
        cout << "Ticks Alive: " << ticksAlive << endl;
        cout << "Status: " << (alive ? "Alive" : "Killed by " + killedBy) << endl;
    }
};

// Singleton: Meadow Class
class Meadow {
private:
    static Meadow* instance;
    Colony** colonies;
    int colonyCount;
    static const int MAX_COLONIES = 10;

public:
    Meadow() {
        colonies = new Colony*[MAX_COLONIES];
        colonyCount = 0;
    }

    ~Meadow() {
        for (int i = 0; i < colonyCount; ++i) {
            delete colonies[i];
        }
        delete[] colonies;
    }

    static Meadow* getInstance() {
        if (!instance) {
            instance = new Meadow();
        }
        return instance;
    }

    void spawnColony(const string& name, const string& species) {
        if (colonyCount < MAX_COLONIES) {
            colonies[colonyCount] = new Colony(name, species);
            colonyCount++;
            cout << "Colony " << name << " of species " << species << " created." << endl;
        } else {
            cout << "Cannot create more colonies. Maximum limit reached." << endl;
        }
    }

    void giveResources(int colonyIndex, int workers, int warriors) {
        if (colonyIndex < 0 || colonyIndex >= colonyCount) {
            cout << "Invalid colony index." << endl;
            return;
        }
        colonies[colonyIndex]->addResources(workers, warriors);
        cout << "Resources added to colony " << colonies[colonyIndex]->name << "." << endl;
    }

    void simulateTick() {
        for (int i = 0; i < colonyCount; ++i) {
            colonies[i]->incrementTicksAlive();
        }
        cout << "One tick has passed." << endl;
    }

    void attack(int attackerIndex, int defenderIndex) {
        if (attackerIndex < 0 || attackerIndex >= colonyCount || defenderIndex < 0 || defenderIndex >= colonyCount) {
            cout << "Invalid colony index." << endl;
            return;
        }

        Colony* attacker = colonies[attackerIndex];
        Colony* defender = colonies[defenderIndex];

        if (!attacker->alive || !defender->alive) {
            cout << "Both colonies must be alive to attack." << endl;
            return;
        }

        if (attacker->warriors > defender->warriors) {
            attacker->incrementAntKills(defender->workers + defender->warriors);
            attacker->addColonyKill(defender->name);
            defender->kill(attacker->name);
            cout << attacker->name << " defeated " << defender->name << "!" << endl;
        } else {
            cout << defender->name << " defended successfully!" << endl;
        }
    }

    void showSummary(int colonyIndex) {
        if (colonyIndex < 0 || colonyIndex >= colonyCount) {
            cout << "Invalid colony index." << endl;
            return;
        }
        colonies[colonyIndex]->showSummary();
    }

    void listColonies() {
        cout << "Current Colonies:" << endl;
        for (int i = 0; i < colonyCount; ++i) {
            cout << i + 1 << ". " << colonies[i]->name << " (" << colonies[i]->species << ")" << endl;
        }
    }
};

Meadow* Meadow::instance = nullptr;

// Command-Line Interface
void showMenu() {
    cout << endl << "--- Ant Farm Simulation ---" << endl;
    cout << "1. Spawn Colony" << endl;
    cout << "2. Give Resources" << endl;
    cout << "3. Simulate Tick" << endl;
    cout << "4. Attack Colony" << endl;
    cout << "5. Show Summary" << endl;
    cout << "6. List Colonies" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice: ";
}

void handleUserInput() {
    Meadow* meadow = Meadow::getInstance();
    bool running = true;

    while (running) {
        showMenu();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            string name, species;
            cout << "Enter colony name: ";
            cin >> name;
            cout << "Enter species: ";
            cin >> species;
            meadow->spawnColony(name, species);
            break;
        }
        case 2: {
            int index, workers, warriors;
            meadow->listColonies();
            cout << "Enter colony index to give resources: ";
            cin >> index;
            cout << "Enter number of workers: ";
            cin >> workers;
            cout << "Enter number of warriors: ";
            cin >> warriors;
            meadow->giveResources(index - 1, workers, warriors);
            break;
        }
        case 3:
            meadow->simulateTick();
            break;
        case 4: {
            int attacker, defender;
            meadow->listColonies();
            cout << "Enter attacker colony index: ";
            cin >> attacker;
            cout << "Enter defender colony index: ";
            cin >> defender;
            meadow->attack(attacker - 1, defender - 1);
            break;
        }
        case 5: {
            int index;
            meadow->listColonies();
            cout << "Enter colony index to show summary: ";
            cin >> index;
            meadow->showSummary(index - 1);
            break;
        }
        case 6:
            meadow->listColonies();
            break;
        case 7:
            running = false;
            cout << "Exiting simulation. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

int main() {
    cout << "Welcome to the Ant Farm Simulation!" << endl;
    handleUserInput();
    return 0;
}
