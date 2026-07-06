// Aiden Tsang
// Lab #2

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <limits>

using namespace std;

// ----- global constants -----
const int NAME_COL = 20;
const int NUM_COL = 12;

const int MIN_STAT = 60;
const int MAX_STAT = 200;

// menu choices
enum MenuOption { BATTLE = 1, QUIT = 2 };

// =========================================================
// Creature class - manages one creature
// =========================================================
class Creature {
private:
    string name;
    int strength;   // maximum damage a creature can inflict
    int health;     // hit points; cannot go below 0

public:
    Creature();
    Creature(const string& creatureName, int creatureStrength, int creatureHealth);

    void setCreature(const string& creatureName, int creatureStrength, int creatureHealth);
    void setName(const string& creatureName);
    void setStrength(int creatureStrength);
    void setHealth(int creatureHealth);

    string getName() const;
    int getStrength() const;
    int getHealth() const;
    int getDamage() const;

    string toString() const;
};

Creature::Creature() {
    setCreature("", 0, 0);
}

Creature::Creature(const string& creatureName, int creatureStrength, int creatureHealth) {
    setCreature(creatureName, creatureStrength, creatureHealth);
}

// single place that actually stores the member variables;
// every other setter/constructor routes through here
void Creature::setCreature(const string& creatureName, int creatureStrength, int creatureHealth) {
    name = creatureName;
    strength = creatureStrength;
    // health can never be negative; floor it at 0
    health = (creatureHealth < 0) ? 0 : creatureHealth;
}

void Creature::setName(const string& creatureName) {
    setCreature(creatureName, strength, health);
}

void Creature::setStrength(int creatureStrength) {
    setCreature(name, creatureStrength, health);
}

void Creature::setHealth(int creatureHealth) {
    setCreature(name, strength, creatureHealth);
}

string Creature::getName() const {
    return name;
}

int Creature::getStrength() const {
    return strength;
}

int Creature::getHealth() const {
    return health;
}

// damage inflicted is a random number up to the creature's strength
int Creature::getDamage() const {
    return (rand() % strength) + 1;
}

string Creature::toString() const {
    stringstream ss;
    ss << left << setw(NAME_COL) << name
       << right << setw(NUM_COL) << strength
       << setw(NUM_COL) << health;
    return ss.str();
}

// =========================================================
// free helper functions
// =========================================================

// prints the Name/Strength/Health table for both creatures
void printCreatureTable(const Creature& a, const Creature& b, const string& title) {
    cout << "\n" << title << "\n";
    cout << left << setw(NAME_COL) << "Name"
         << right << setw(NUM_COL) << "Strength"
         << setw(NUM_COL) << "Health" << "\n";
    cout << a.toString() << "\n";
    cout << b.toString() << "\n";
}

void printRoundHeader() {
    cout << left << setw(6) << "Round"
         << setw(NAME_COL) << "Attacker"
         << right << setw(NUM_COL) << "Damage"
         << left << setw(NAME_COL + 2) << "  Defender"
         << right << setw(NUM_COL + 4) << "Defender Health" << "\n";
}

void printRoundRow(int round, const Creature& attacker, int damage, const Creature& defender) {
    cout << left << setw(6) << round
         << setw(NAME_COL) << attacker.getName()
         << right << setw(NUM_COL) << damage
         << left << setw(NAME_COL + 2) << ("  " + defender.getName())
         << right << setw(NUM_COL + 4) << defender.getHealth() << "\n";
}

// one creature attacks another; health cannot drop below 0.
// same function is reused for both directions by swapping the arguments.
int attackRound(const Creature& attacker, Creature& defender) {
    int damage = attacker.getDamage();
    int newHealth = defender.getHealth() - damage;
    if (newHealth < 0) {
        newHealth = 0;
    }
    defender.setHealth(newHealth);
    return damage;
}

void runBattle() {
    string nameA, nameB;
    cout << "\nEnter the name of the first creature: ";
    getline(cin, nameA);
    cout << "Enter the name of the second creature: ";
    getline(cin, nameB);

    int strengthA = (rand() % (MAX_STAT - MIN_STAT + 1)) + MIN_STAT;
    int healthA   = (rand() % (MAX_STAT - MIN_STAT + 1)) + MIN_STAT;
    int strengthB = (rand() % (MAX_STAT - MIN_STAT + 1)) + MIN_STAT;
    int healthB   = (rand() % (MAX_STAT - MIN_STAT + 1)) + MIN_STAT;

    Creature creatureA(nameA, strengthA, healthA);
    Creature creatureB(nameB, strengthB, healthB);

    printCreatureTable(creatureA, creatureB, "--- Creatures Before Battle ---");

    // randomly select who attacks first
    bool aAttacksFirst = (rand() % 2 == 0);

    cout << "\n" << (aAttacksFirst ? creatureA.getName() : creatureB.getName())
         << " attacks first!\n";

    cout << "\n--- Battle Log ---\n";
    printRoundHeader();

    int round = 0;
    bool attackerIsA = aAttacksFirst;

    while (creatureA.getHealth() > 0 && creatureB.getHealth() > 0) {
        round++;
        int damage;
        if (attackerIsA) {
            damage = attackRound(creatureA, creatureB);
            printRoundRow(round, creatureA, damage, creatureB);
        } else {
            damage = attackRound(creatureB, creatureA);
            printRoundRow(round, creatureB, damage, creatureA);
        }
        attackerIsA = !attackerIsA;
    }

    printCreatureTable(creatureA, creatureB, "--- Creatures After Battle ---");

    const Creature& winner = (creatureA.getHealth() == 0) ? creatureB : creatureA;
    const Creature& loser  = (creatureA.getHealth() == 0) ? creatureA : creatureB;

    cout << "\n" << winner.getName() << " defeated " << loser.getName()
         << " in " << round << " rounds.\n";
}

void printMenu() {
    cout << "\n===== Creature Battle Menu =====\n";
    cout << BATTLE << ". Battle\n";
    cout << QUIT << ". Quit\n";
    cout << "Enter your choice: ";
}

// reads the menu choice, reprompting in place (no menu reprint / message
// spam) until a valid integer choice (BATTLE or QUIT) is entered
int readMenuChoice() {
    int choice;
    while (true) {
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter " << BATTLE << " or " << QUIT << ": ";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice != BATTLE && choice != QUIT) {
            cout << "Invalid choice. Please enter " << BATTLE << " or " << QUIT << ": ";
            continue;
        }

        return choice;
    }
}

// asks the user to confirm quitting; returns true only on 'y'
bool confirmQuit() {
    char answer;
    do {
        cout << "\nAre you sure you want to quit? (y/n): ";
        cin >> answer;
        cin.ignore();
        answer = static_cast<char>(tolower(answer));

        if (answer != 'y' && answer != 'n') {
            cout << "Invalid input. Please enter 'y' or 'n'.\n";
        }
    } while (answer != 'y' && answer != 'n');

    return answer == 'y';
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int choice = 0;
    bool running = true;

    while (running) {
        printMenu();
        choice = readMenuChoice();

        switch (choice) {
            case BATTLE:
                runBattle();
                break;
            case QUIT:
                if (confirmQuit()) {
                    cout << "Goodbye!\n";
                    running = false;
                }
                break;
        }
    }

    return 0;
}
