#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
//#include <unistd.h>

using namespace std;

void printBoard(char** player_board, char** player_moves) {
    // prints game board after each move
    // also prints how many of the opponent's ships the player has sunk

    system("clear");

    cout << "Legend:" << endl;
    cout << "O.....................untouched" << endl;
    cout << "X..........................miss" << endl;
    cout << "#...........................hit" << endl;
    cout << "@................untouched boat" << endl << endl;

    // print player's moves first
    cout << "Your moves:" << endl;
    cout << ".";
    for(int i = 0; i < 10; i++) {
        cout << " " << i+1;
    }
    cout << endl;

    char rows[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};

    for (int j = 0; j < 10; j++) {
        cout << rows[j];
        for (int k = 0; k < 10; k++) {
            cout << " " << player_moves[j][k];
        }
        cout << endl;
    }
    cout << endl << "----------------------" << endl << "Your board:" << endl;

    // print player's board
    cout << ".";
    for(int i = 0; i < 10; i++) {
        cout << " " << i+1;
    }
    cout << endl;

    for (int j = 0; j < 10; j++) {
        cout << rows[j];
        for (int k = 0; k < 10; k++) {
            cout << " " << player_board[j][k];
        }
        cout << endl;
    }
    cout << endl; 
}

int rowLetterToNumber(char letter) {
    if (letter == 'A') return 0;
    if (letter == 'B') return 1;
    if (letter == 'C') return 2;
    if (letter == 'D') return 3;
    if (letter == 'E') return 4;
    if (letter == 'F') return 5;
    if (letter == 'G') return 6;
    if (letter == 'H') return 7;
    if (letter == 'I') return 8;
    if (letter == 'J') return 9;
    return -1;
}

bool checkBoardforShipPlacement(int size, int row, int column, bool horizontal, char** board) {
    if (horizontal) {
        for (int i = column; i < column + size; i++) {
            if (board[row][i] != 'O') {
                return false;
            }
        }
    }
    else {
        for (int i = row; i < row + size; i++) {
            if (board[i][column] != 'O') {
                return false;
            }
        }
    }
    return true;
}

/*  * Checks if user selected a vldi location for their ship and then places the ship
    * Returns true if placement successful. Otherwise, returns false */
bool checkShipPlacement(int size_idx, string location, bool horizontal, char** board, map<string, int>* ship_coords) {
    // check for invalid coordinate entry
    if (location.length() > 3 || location.length() < 2) {
        return false;
    }

    int column;
    int row = rowLetterToNumber(location[0]);
    vector<int> sizes = {5, 4, 3, 3, 2};
    int size = sizes[size_idx];
    
    // invalid row entry
    if (row == -1) {
        cout << "bad row entry" << endl;
        return false;
    }

    // check for valid column entry
    if (location.length() == 2) {
        column = location[1] - '0';
        column -= 1;
    }
    else {
        if (location[1] == '1' && location[2] == '0') {
            column = 9;
        }
        else {
            return false;
        }
    }

    // check horizontal placement works with ship
    if (horizontal) {
        if ((column + size - 1) > 9) {
            cout << "The ship extends out of bounds! Please try again." << endl;
            return false;
        }
    }

    // check vertical placement works with ship
    else {
        if ((row + size - 1) > 9) {
            cout << "The ship extends out of bounds! Please try again." << endl;
            return false;
        }
    }

    bool valid_placement = checkBoardforShipPlacement(size, row, column, horizontal, board);
    if (!valid_placement) {
        cout << "The ship cannot go on top of another ship! Please try again." << endl;
        return false;
    }

    // place ship on board
    if (horizontal) {
        for (int i = column; i < column + size; i++) {
            board[row][i] = '@';
            ship_coords->insert(std::pair<string, int>(location, size_idx));
        }
    }
    else {
        for (int i = row; i < row + size; i++) {
            board[i][column] = '@';
            ship_coords->insert(std::pair<string, int>(location, size_idx));
        }
    }
    return true;
}

void placeShips(char** board, char** moves, map<string, int>* ship_coords) {
    int boat_sizes[5] = {5, 4, 3, 3, 2};
    bool placement_valid = false;
    string coordinate = "K11";
    string left_or_top = "neither";

    string orientation_s = "diagonal";

    for (int i = 0; i < 5; i++) {
        placement_valid = false;
        orientation_s = "diagonal";
        
        // User selects orientation for boat
        while (orientation_s != "horizontal" && orientation_s != "vertical") {
            cout << "Please select 'horizontal' or 'vertical' for your size " << boat_sizes[i] << " ship: ";
            cin >> orientation_s;

            // ensures user picks horizontal or vertical
            if (orientation_s == "horizontal" || orientation_s == "vertical") {
                if (orientation_s == "horizontal") {
                    left_or_top = "left";
                }
                else {
                    left_or_top = "top";
                }

                // wait for valid coordinate entry
                while(!placement_valid) {
                    cout << "Please enter a coordinate (e.g. 'A1') for the " << left_or_top << " end of your size " << boat_sizes[i] << " ship, or type 'back' to reselect your ship's orientation: ";
                    cin >> coordinate;
                    if (coordinate != "back") {
                        placement_valid = checkShipPlacement(i, coordinate, orientation_s == "horizontal", board, ship_coords);
                    }
                    else {
                        orientation_s = "diagonal"; // reset orientation in case user backed out of picking coordinate
                        break;
                    }
                }                
            }
        }
        // reprint board to show placement of new ship
        printBoard(board, moves);
    }
}

void cpuPlaceShips(char** board, map<string, int>* ship_coords) {
    int ships_placed = 0;
    int ship_sizes[5] = {5, 4, 3, 3, 2};
    int coords[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int horizontal;
    int curr_size;
    int column;
    int row;
    vector<char> row_letters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};

    while(ships_placed < 5) {
        horizontal = rand() % 2;
        curr_size = ship_sizes[ships_placed];

        // randomly choose coordinates for ship so it will be within board's bounds
        if (horizontal == 1) {
            row = coords[rand() % 10];
            column = coords[rand() % (10 - curr_size + 1)];
        }
        else {
            row = coords[rand() % (10 - curr_size + 1)];
            column = coords[rand() % 10];
        }

        // ensure ship's location does not interfere with existing ships
        bool placement_valid = checkBoardforShipPlacement(curr_size, row, column, horizontal == 1, board);
        if (placement_valid) {
            // place ship on board
            if (horizontal == 1) {
                for (int i = column; i < column + curr_size; i++) {
                    board[row][i] = '@';
                    // Add ship coordinate to map storing all ship coordinates
                    ship_coords->insert(std::pair<string, int>(row_letters[row] + std::to_string(i+1), ships_placed));
                }
            }
            else {
                for (int i = row; i < row + curr_size; i++) {
                    board[i][column] = '@';
                    // Add ship coordinate to map storing all ship coordinates
                    ship_coords->insert(std::pair<string, int>(row_letters[i] + std::to_string(column+1), ships_placed));
                }
            }
            ships_placed += 1;
            cout << "cpu ship placed successfully" << endl;
        }
    }
}

/*  * Makes player's move
    * returns -1 if coordinate already used, 0 if miss, 1 if hit */
int makeMove(char** player_moves, char** opp_board, int row, int column) {
    // Player already hit this coordinate
    if (opp_board[row][column] == 'X' || opp_board[row][column] == '#') {
        return -1;
    }

    // Miss
    if (opp_board[row][column] == 'O') {
        opp_board[row][column] = 'X';
        player_moves[row][column] = 'X';
        return 0;
    }

    // Hit
    else {
        opp_board[row][column] = '#';
        player_moves[row][column] = '#';
        return 1;
    }
}

string getMoveCoord(char** player_moves, char** cpu_board, char** player_board) {
    string location;
    int row = -1;
    int column = -1;

    while (row == -1 || column == -1) {
        row = -1;
        column = -1;
        cout << "Enter the coordinate where you would like to make your move (e.g. 'A1'): ";
        cin >> location;
        if (1 < location.length() && location.length() < 4) {
            row = rowLetterToNumber(location[0]);
            if (row != -1)  {
                // check for valid column entry
                if (location.length() == 2) {
                    column = location[1] - '0';
                    column -= 1;
                }
                else if (location[1] == '1' && location[2] == '0') {
                    column = 9;
                }
                else {
                    column = -1;
                }
                if (column != -1) {
                    // column tracks outcome of move, sends back to top of loop if -1
                    column = makeMove(player_moves, cpu_board, row, column);
                }
            }
        }
        if (row == -1 || column == -1) {
            cout << "Invalid coordinate entry! Please try again." << endl;
        }
    }
    if (column == 0) {
        printBoard(player_board, player_moves);
        cout << location << "... Miss!" << endl;
        return "miss";
    }
    else {
        printBoard(player_board, player_moves);
        cout << location << "... Hit!";
        return location;
    }
}

string cpuMakeMove(char** player_moves, char** player_board) {
    bool move_valid = false;
    int row = -1;
    int column = -1;
    int r_val = -1;
    vector<char> rows = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    while (!move_valid) {
        row = rand() % 10;
        column = rand() % 10;
        if (player_board[row][column] == 'O' || player_board[row][column] == '@') {
            move_valid = true;
        }
    }
    string location = rows[row] + to_string(column);
    if (player_board[row][column] == 'O') {
        player_board[row][column] = 'X';
        printBoard(player_board, player_moves);
        cout << "CPU moved at " << location << "... Miss!";
        return "miss";
    }
    else {
        player_board[row][column] = '#';
        printBoard(player_board, player_moves);
        cout << "CPU moved at " << location << "... Hit!";
    }
    return location;
}

bool checkWinner(vector<int> score) {
    vector<int> victory = {5, 4, 3, 3, 2};
    // checks if computer or player won the game
    for (int i = 0; i < 5; i++) {
        if (score[i] != victory[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    cout << "------ Welcome to Battleship! ------" << endl;

    // Tracks number of hits on each ship
    // Player wins when their score array is {5, 4, 3, 3, 2}
    vector<int> player_score = {0, 0, 0, 0, 0};
    vector<int> cpu_score = {0, 0, 0, 0, 0};
    vector<int> victory = {5, 4, 3, 3, 2};

    char** player_board = new char*[10];
    char** cpu_board = new char*[10];
    char** player_moves = new char*[10];

    for (int i = 0; i < 10; i++) {
        player_board[i] = new char[10];
        cpu_board[i] = new char[10];
        player_moves[i] = new char[10];

        for (int j = 0; j < 10; j++) {
            player_board[i][j] = 'O';
            cpu_board[i][j] = 'O';
            player_moves[i][j] = 'O';
        }
    }
    map<string, int>* p_ship_coords = new map<string, int>();
    map<string, int>* cpu_ship_coords = new map<string, int>();

    printBoard(player_board, player_moves);
    placeShips(player_board, player_moves, p_ship_coords);
    cpuPlaceShips(cpu_board, cpu_ship_coords);

    bool game_over = false;
    string move_result;

    printBoard(cpu_board, player_board);
    cout << "player board on top, cpu board on bottom" << endl;

    // game loop
    while(!game_over) {
        move_result = getMoveCoord(player_moves, cpu_board, player_board);
        // Update player score
        if (move_result != "miss") {
            int ship = p_ship_coords->at(move_result);
            player_score[ship] += 1;
            if (player_score[ship] == victory[ship]) {
                cout << " Sunk the opponent's size " << victory[ship] << "ship!";
                game_over = checkWinner(player_score);
            }
        }
        cout << endl;
        if (game_over) {
            cout << "Congratulations! You won Battleship!" << endl;
            cout << "Press 'Enter' to exit game. Thanks for playing!" << endl;
            cin >> move_result;
            break;
        }

        // Computer move
        cout << "The computer is making its move";
        for (int i = 0; i < 3; i++) {
            cout << ".";
            // sleep some amount of time
        }
        cout << endl;
        move_result = cpuMakeMove(player_moves, player_board);
        // Update player score
        if (move_result != "miss") {
            int ship = cpu_ship_coords->at(move_result);
            cpu_score[ship] += 1;
            if (cpu_score[ship] == victory[ship]) {
                cout << " The CPU sunk your size " << victory[ship] << "ship!";
                game_over = checkWinner(cpu_score);
            }
        }
        cout << endl;
    
        if (game_over) {
            cout << "The computer won! Better luck next time." << endl;
            cout << "Press 'Enter' to exit game. Thanks for playing!" << endl;
            cin >> move_result;
        }
    }

    // delete boards
    for (int i = 0; i < 10; i++) {
        delete[] player_board[i];
        delete[] cpu_board[i];
        delete[] player_moves[i];
    }
    delete[] player_board;
    delete[] cpu_board;
    delete[] player_moves;
    delete p_ship_coords;
    delete cpu_ship_coords;

    return 0;
}