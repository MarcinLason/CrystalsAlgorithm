#include <iostream>
#include <vector>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "TemplateArgumentsIssues"
int MAX_MAZE_WIDTH = 100;
int MAX_MAZE_LENGTH = 100;
int MAX_CRISTALS_AMOUNT = 100;
char CRISTAL_SIGN = '*';
char BLOCK_SIGN = '#';
char LEFT_MIRROR = '/';
char RIGHT_MIRROR = '\\';

int NORTH = 1;
int WEST = 2;
int EAST = 3;
int SOUTH = 4;


using namespace std;
// STRUCTS:

struct CellPosition {
    int yposition;
    int xposition;
};

struct CrystalPosition {
    int yposition;
    int xposition;
    bool visited;
};

//DECLARATIONS:

void printMaze(const int &length, const int &width, vector<vector<char>> &maze);

void printPositionsForMirrors(vector<CellPosition> positionsVector);

void printPositionsOfCrystals(vector<CrystalPosition> positionsVector);

bool checkNeighboringPositions(int i, int j, vector<vector<char>> &maze, const int &length, const int &width);

//MAIN LOGIC METHODS

void readInputArguments(int &length, int &width, int &numOfMirrors) {
    cin >> length;
    cin >> width;
    cin >> numOfMirrors;
    cin.ignore();
}

void parseMaze(const int &length, const int &width, vector<vector<char>> &maze) {
    for (int i = 0; i < length; ++i) {
        string input;
        getline(cin, input);
        for (int j = 0; j < width; ++j) {
            maze[i][j] = input[j];
        }
    }
}

vector<CellPosition> setPositionsForMirrors(vector<vector<char>> &maze, const int &length, const int &width) {
    vector<CellPosition> positionVector;
    positionVector = vector<CellPosition>();


    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j] != CRISTAL_SIGN && maze[i][j] != BLOCK_SIGN &&
                checkNeighboringPositions(i, j, maze, length, width)) {

                CellPosition currentPosition = {
                        i,
                        j,
                };
                positionVector.push_back(currentPosition);
            }
        }
    }
    positionVector.shrink_to_fit();
    return positionVector;
}

vector<CrystalPosition> setPositionsOfCrystals(vector<vector<char>> &maze, const int &length, const int width) {
    vector<CrystalPosition> positionVector;
    positionVector = vector<CrystalPosition>();

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j] == CRISTAL_SIGN) {
                CrystalPosition crystalPosition = {
                        i,
                        j,
                        false,
                };
                positionVector.push_back(crystalPosition);
            }
        }
    }
    positionVector.shrink_to_fit();
    return positionVector;
}

// HELPER METHODS:

bool checkNeighboringPositions(int i, int j, vector<vector<char>> &maze, const int &length, const int &width) {
    if (length < 3 && width < 3) return false;
    if (i == 0 || i == (length - 1) || j == 0 || j == (width - 1)) return false;
    if (maze[i - 1][j] == BLOCK_SIGN && maze[i + 1][j] == BLOCK_SIGN) return false;
    if (maze[i][j - 1] == BLOCK_SIGN && maze[i][j + 1] == BLOCK_SIGN) return false;

    return true;
}


int main(int argc, char *argv[]) {
    int direction = EAST;
    int mazeLength;
    int mazeWidth;
    int amountOfMirrors;

    vector<vector<char>> originalMaze;
    vector<CellPosition> positionsForMirrors = vector<CellPosition>();
    vector<CrystalPosition> positionsOfCrystals = vector<CrystalPosition>();

    //Wczytanie podstawowych danych wejściowych
    readInputArguments(mazeLength, mazeWidth, amountOfMirrors);

    //Przygotowanie wektora przetrzymującego labirynt
    originalMaze = vector<vector<char>>(mazeLength);
    for (int i = 0; i < mazeLength; i++) {
        originalMaze[i] = vector<char>(mazeWidth);
    }

    //Wczytanie labiryntu a także pozycji miejsc na lustra i kryształów
    parseMaze(mazeLength, mazeWidth, originalMaze);
    positionsForMirrors = setPositionsForMirrors(originalMaze, mazeLength, mazeWidth);
    positionsOfCrystals = setPositionsOfCrystals(originalMaze, mazeLength, mazeWidth);

    printPositionsForMirrors(positionsForMirrors);
    printPositionsOfCrystals(positionsOfCrystals);
    return 0;
}


void printMaze(const int &length, const int &width, vector<vector<char>> &maze) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < (width - 1); j++) {
            cout << maze[i][j];
        }
        cout << maze[i][width - 1] << endl;
    }
}

void printPositionsForMirrors(vector<CellPosition> positionsVector) {
    cout << "Pozycje potencjalnych pol na lustra" << endl;
    for (int i = 0; i < positionsVector.size(); i++) {
        CellPosition c = positionsVector.at(i);
        cout << "Y: " << c.yposition << " X: " << c.xposition << endl;
    }
}

void printPositionsOfCrystals(vector<CrystalPosition> positionsVector) {
    cout << "Pozycje pol z krysztalami" << endl;
    for (int i = 0; i < positionsVector.size(); i++) {
        CrystalPosition c = positionsVector.at(i);
        cout << "Y: " << c.yposition << " X: " << c.xposition << endl;
    }
}


#pragma clang diagnostic pop