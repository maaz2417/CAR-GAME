#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// ----------- Car Base & Types (Inheritance + Polymorphism) ------------
class Car {
protected:
    string model;
    double price;
public:
    Car(string model, double price) : model(model), price(price) {}
    virtual void show() const {
        cout << "Car: " << model << ", Price: $" << price << endl;
    }
    virtual double getPrice() const { return price; }
    virtual string getType() const = 0;
    virtual ~Car() {}
};

class Sedan : public Car {
public:
    Sedan(string model, double price) : Car(model, price) {}
    string getType() const override { return "Sedan"; }
};

class SUV : public Car {
public:
    SUV(string model, double price) : Car(model, price) {}
    string getType() const override { return "SUV"; }
};

// ----------- Player Class (Static, Composition, Overloading) ------------
class Player {
private:
    string name;
    double money;
    int reputation;
    int chapter;
    vector<Car*> inventory;

public:
    static int playerCount;

    Player(string name) : name(name), money(5000), reputation(50), chapter(1) {
        playerCount++;
    }

    ~Player() {
        for (auto car : inventory) delete car;
    }

    void showStatus() const {
        cout << "\n[" << name << "] Money: $" << money << " | Reputation: " << reputation << " | Chapter: " << chapter << endl;
    }

    void buyCar(Car* car) {
        if (money >= car->getPrice()) {
            inventory.push_back(car);
            money -= car->getPrice();
            reputation += 5;
            cout << "You bought a " << car->getType() << ": " << car->getPrice() << "\n";
        } else {
            cout << "Not enough funds.\n";
            delete car;
        }
    }

    void showInventory() const {
        cout << "\nInventory:\n";
        if (inventory.empty()) cout << "Empty.\n";
        for (auto car : inventory) car->show();
    }

    void changeMoney(int amount) { money += amount; }
    void changeReputation(int amount) { reputation += amount; }
    int getChapter() const { return chapter; }
    void nextChapter() { chapter++; }

    void saveToFile() const {
        ofstream file("savegame.txt");
        file << name << " " << money << " " << reputation << " " << chapter << " " << inventory.size() << "\n";
        for (auto car : inventory) {
            file << car->getType() << " " << car->getPrice() << " Auto" << (int)car->getPrice() << "\n";
        }
        file.close();
    }

    bool loadFromFile() {
        ifstream file("savegame.txt");
        int invSize;
        string type, model;
        double price;
        if (!(file >> name >> money >> reputation >> chapter >> invSize)) return false;
        for (int i = 0; i < invSize; ++i) {
            file >> type >> price >> model;
            if (type == "Sedan") inventory.push_back(new Sedan(model, price));
            else if (type == "SUV") inventory.push_back(new SUV(model, price));
        }
        return true;
    }

    // Operator overloading
    bool operator>(const Player& other) const {
        return reputation > other.reputation;
    }
};

int Player::playerCount = 0;

// ----------- Chapter Functions ------------
void chapter1(Player& player) {
    int choice;
    cout << "\n[CHAPTER 1: The Dealership Begins]";
    cout << "\nYou inherit a run-down garage from your uncle.";
    cout << "\nAn investor offers $10,000 for 60% ownership.";
    cout << "\n1. Accept the investment (gain money, lose reputation)";
    cout << "\n2. Reject and build alone (gain reputation)\nChoice: ";
    cin >> choice;

    if (choice == 1) {
        player.changeMoney(10000);
        player.changeReputation(-20);
    } else {
        player.changeReputation(15);
    }

    player.nextChapter();
    player.saveToFile();
}

void chapter2(Player& player) {
    int choice;
    cout << "\n[CHAPTER 2: First Customer]";
    cout << "\nA customer wants a fast car. You can:";
    cout << "\n1. Sell a damaged car (gain $4000, lose 25 rep)";
    cout << "\n2. Warn them and lose the sale (gain 10 rep)\nChoice: ";
    cin >> choice;

    if (choice == 1) {
        player.changeMoney(4000);
        player.changeReputation(-25);
    } else {
        player.changeReputation(10);
    }

    player.nextChapter();
    player.saveToFile();
}

void chapter3(Player& player) {
    int choice;
    cout << "\n[CHAPTER 3: Inventory Upgrade]";
    cout << "\nYou find a good Sedan for $3000. Buy it?";
    cout << "\n1. Yes\n2. No\nChoice: ";
    cin >> choice;
    if (choice == 1) {
        player.buyCar(new Sedan("StarterRide", 3000));
    }

    cout << "\nLater, a shady dealer offers a mystery SUV for $2000.";
    cout << "\n1. Buy it (chance of profit or loss)";
    cout << "\n2. Refuse\nChoice: ";
    cin >> choice;

    if (choice == 1) {
        int luck = rand() % 2;
        if (luck) {
            cout << "It was a steal! Worth $5000.\n";
            player.buyCar(new SUV("LuckySUV", 2000));
            player.changeReputation(5);
        } else {
            cout << "It broke down. Total loss.\n";
            player.changeMoney(-2000);
            player.changeReputation(-10);
        }
    }

    player.nextChapter();
    player.saveToFile();
}

void playChapters(Player& player) {
    int current = player.getChapter();

    if (current == 1) chapter1(player);
    if (current == 2) chapter2(player);
    if (current == 3) chapter3(player);
    // You can keep adding more chapters here.
}

// ----------- Main Menu ------------
int main() {
    Player player("Unknown");

    cout << "Load saved game? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' && player.loadFromFile()) {
        cout << "\nSave loaded!\n";
    } else {
        string name;
        cout << "Enter your name: ";
        cin >> name;
        player = Player(name);
        player.saveToFile();
    }

    player.showStatus();
    player.showInventory();

    cout << "\nContinue your story? (y/n): ";
    cin >> choice;

    if (choice == 'y') {
        playChapters(player);
        player.showStatus();
        player.showInventory();
    } else {
        cout << "dont come again :(\n";
        cout<<"my name is bushra asad "<<endl;
    }

    return 0;
}
