// Aiden Tsang
// Lab #2

#include <iostream>
#include <iomanip> 
#include <string>
#include <sstream>
#include <cstdlib>
#include <limits>

using namespace std;


const int NAME_COL = 20;
const int NUM_COL = 12;

const int MIN_STAT = 60;
const int MAX_STAT = 200;

const string DEFAULT_NAME = "Unknown";
const int DEFAULT_STRENGTH = 0;
const int DEFAULT_HEALTH = 0;


const unsigned int RANDOM_SEED = 216;

enum MenuOption { BATTLE = 1, QUIT = 2 };


class Creature {
private:
    string name = DEFAULT_NAME;       // creature's name
    int strength = DEFAULT_STRENGTH;  // maximum damage a creature can inflict
    int health = DEFAULT_HEALTH;      // hit points; cannot go below 0

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


void printCreatureTable(const Creature& a, const Creature& b, const string& title);
void printRoundHeader();
void printRoundRow(int round, const Creature& attacker, int damage, const Creature& defender);
int attackRound(const Creature& attacker, Creature& defender);
void runBattle();
void printMenu();
int readMenuChoice();
bool confirmQuit();


int main() {
    srand(RANDOM_SEED);

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


Creature::Creature() {
    setCreature(DEFAULT_NAME, DEFAULT_STRENGTH, DEFAULT_HEALTH);
}

Creature::Creature(const string& creatureName, int creatureStrength, int creatureHealth) {
    setCreature(creatureName, creatureStrength, creatureHealth);
}

void Creature::setCreature(const string& creatureName, int creatureStrength, int creatureHealth) {
    name = creatureName;
    strength = creatureStrength;
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

    bool aAttacksFirst = (rand() % 2 == 0);

    cout << "\n" << (aAttacksFirst ? creatureA.getName() : creatureB.getName())
         << " attacks first!\n";

    cout << "\n--- Battle Log ---\n";
    printRoundHeader();

    int round = 0;
    bool attackerIsA = aAttacksFirst;

    while (creatureA.getHealth() > 0 && creatureB.getHealth() > 0) {
        round++;
        int damage = 0;
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

int readMenuChoice() {
    int choice = 0;
    bool valid = false;

    while (!valid) {
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
            cout << "Invalid choice. Please enter " << BATTLE << " or " << QUIT << ": ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == BATTLE || choice == QUIT) {
                valid = true;
            } else {
                cout << "Invalid choice. Please enter " << BATTLE << " or " << QUIT << ": ";
            }
        }
    }

    return choice;
}

bool confirmQuit() {
    char answer = ' ';

    while (answer != 'y' && answer != 'n') {
        cout << "\nAre you sure you want to quit? (y/n): ";
        cin >> answer;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Convert uppercase to lowercase without <cctype>
        if (answer >= 'A' && answer <= 'Z') {
            answer = static_cast<char>(answer - 'A' + 'a');
        }

        if (answer != 'y' && answer != 'n') {
            cout << "Invalid input. Please enter 'y' or 'n'.\n";
        }
    }

    return answer == 'y';
}

/* Output
aidentsang@Aidens-MacBook-Pro CS216_L2_AT % "/Users/aidentsang/Pierce college Labs C++/CS216_L2_AT/build/main"

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 1

Enter the name of the first creature:   Jo the elf
Enter the name of the second creature: Lee the  demon

--- Creatures Before Battle ---
Name                    Strength      Health
        Jo the elf                  186         161
Lee the  demon               131          63

Lee the  demon attacks first!

--- Battle Log ---
Round Attacker                  Damage  Defender             Defender Health
1     Lee the  demon               108          Jo the elf                       53
2       Jo the elf                   21  Lee the  demon                    42
3     Lee the  demon                95          Jo the elf                        0

--- Creatures After Battle ---
Name                    Strength      Health
        Jo the elf                  186           0
Lee the  demon               131          42

Lee the  demon defeated         Jo the elf in 3 rounds.

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 3
Invalid choice. Please enter 1 or 2: aiden
Invalid choice. Please enter 1 or 2: 1

Enter the name of the first creature: !2
Enter the name of the second creature: #fda-

--- Creatures Before Battle ---
Name                    Strength      Health
!2                           109         192
#fda-                        134         152

#fda- attacks first!

--- Battle Log ---
Round Attacker                  Damage  Defender             Defender Health
1     #fda-                         98  !2                                94
2     !2                            28  #fda-                            124
3     #fda-                         49  !2                                45
4     !2                            55  #fda-                             69
5     #fda-                        110  !2                                 0

--- Creatures After Battle ---
Name                    Strength      Health
!2                           109           0
#fda-                        134          69

#fda- defeated !2 in 5 rounds.

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 2

Are you sure you want to quit? (y/n): 1
Invalid input. Please enter 'y' or 'n'.

Are you sure you want to quit? (y/n): n

===== Creature Battle Menu =====
1. Battle
2. Quit
Enter your choice: 2

Are you sure you want to quit? (y/n): y
Goodbye!
aidentsang@Aidens-MacBook-Pro CS216_L2_AT % */
