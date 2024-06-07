#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Player {
public:
    string name;
    int price;
    int speed;
    int finishing;
    int defence;
};

class Team {
public:
    string name;
    int money;
    vector<Player> players;
    int wins = 0;
    int draws = 0;
    int losses = 0;
};

vector<Player> allPlayers;
vector<Team> allTeams;

void createPlayer(string name, int price, int speed, int finishing, int defence) {
    Player newPlayer;
    newPlayer.name = name;
    newPlayer.price = price;
    newPlayer.speed = speed;
    newPlayer.finishing = finishing;
    newPlayer.defence = defence;

    // Check for duplicate players
    if (find_if(allPlayers.begin(), allPlayers.end(), [&name](const Player& p) { return p.name == name; }) == allPlayers.end()) {
        allPlayers.push_back(newPlayer);
        cout << "player added to the team succesfully" << endl;
    } else {
        cout << "player already exists" << endl;
    }
}

void createTeam(string name, int money) {
    Team newTeam;
    newTeam.name = name;
    newTeam.money = money;

    // Check for duplicate teams
    if (find_if(allTeams.begin(), allTeams.end(), [&name](const Team& t) { return t.name == name; }) == allTeams.end()) {
        allTeams.push_back(newTeam);
        cout << "team added successfully" << endl;
    } else {
        cout << "team already exists" << endl;
    }
}

void buyPlayer(int playerID, int teamID) {
    // Implement the buy player logic here
}

void sellPlayer(int playerID, int teamID) {
    // Implement the sell player logic here
}

void matchTeams(int teamID1, int teamID2) {
    // Implement the match logic here
}

void showPlayers() {
    // Implement displaying all players here
}

void showTeams() {
    // Implement displaying all teams here
}

void showPlayer(int playerID) {
    // Implement displaying a specific player here
}

void showTeam(int teamID) {
    // Implement displaying a specific team here
}

void showRank() {
    // Implement displaying the rank of teams here
}

int main() {
    // Your main program logic goes here

    return 0;
}
