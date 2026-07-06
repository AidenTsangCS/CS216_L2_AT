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

/*OUTPUT
aidentsang@Aidens-MacBook-Pro CS216_L2_AT % "/Users/aidentsang/Pierce college Labs C++/CS216_L2_AT/build/main"

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 2

Are you sure you want to quit? (y/n): n

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 1

Enter the name of the first creature: Raquayza
Enter the name of the second creature: Digimon

--- Creatures Before Battle ---
Name                    Strength      Health
Raquayza                     155         145
Digimon                       91          73

Raquayza attacks first!

--- Battle Log ---
Round Attacker                  Damage  Defender             Defender Health
1     Raquayza                      94  Digimon                            0

--- Creatures After Battle ---
Name                    Strength      Health
Raquayza                     155         145
Digimon                       91           0

Raquayza defeated Digimon in 1 rounds.

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 1

Enter the name of the first creature: 1324
Enter the name of the second creature: /fdsa

--- Creatures Before Battle ---
Name                    Strength      Health
1324                         176         143
/fdsa                        141          95

1324 attacks first!

--- Battle Log ---
Round Attacker                  Damage  Defender             Defender Health
1     1324                          50  /fdsa                             45
2     /fdsa                        105  1324                              38
3     1324                          99  /fdsa                              0

--- Creatures After Battle ---
Name                    Strength      Health
1324                         176          38
/fdsa                        141           0

1324 defeated /fdsa in 3 rounds.

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 3
Invalid choice. Please enter 1 or 2: `
Invalid choice. Please enter 1 or 2: 0
Invalid choice. Please enter 1 or 2: d
Invalid choice. Please enter 1 or 2: fsa
Invalid choice. Please enter 1 or 2: 2

Are you sure you want to quit? (y/n): 3
Invalid input. Please enter 'y' or 'n'.

Are you sure you want to quit? (y/n): 4
Invalid input. Please enter 'y' or 'n'.

Are you sure you want to quit? (y/n): 2
Invalid input. Please enter 'y' or 'n'.

Are you sure you want to quit? (y/n): n

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 1

Enter the name of the first creature: Aiden
Enter the name of the second creature: Daniel

--- Creatures Before Battle ---
Name                    Strength      Health
Aiden                        188         145
Daniel                       104         156

Daniel attacks first!

--- Battle Log ---
Round Attacker                  Damage  Defender             Defender Health
1     Daniel                        80  Aiden                             65
2     Aiden                        163  Daniel                             0

--- Creatures After Battle ---
Name                    Strength      Health
Aiden                        188          65
Daniel                       104           0

Aiden defeated Daniel in 2 rounds.

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 2

Are you sure you want to quit? (y/n): y
Goodbye!
aidentsang@Aidens-MacBook-Pro CS216_L2_AT % */
