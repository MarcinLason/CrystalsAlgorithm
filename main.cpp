#include <iostream>
#include <vector>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "TemplateArgumentsIssues"
int MAX_MAZE_WIDTH = 100;
int MAX_MAZE_LENGTH = 100;
int MAX_CRISTALS_AMOUNT = 100;
char CRYSTAL_SIGN = '*';
char BLOCK_SIGN = '#';
char LEFT_MIRROR = '/';
char RIGHT_MIRROR = '\\';

int NORTH = 1;
int WEST = 2;
int EAST = 3;
int SOUTH = 4;

int SAME_X = 0;
int SAME_Y = 1;


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

struct MirrorPosition {
    int yposition;
    int xposition;
    char sign;
    int direction;
};

//DECLARATIONS:

void printMaze(const int &length, const int &width, vector<vector<char>> &maze);

void printPositionsForMirrors(vector<CellPosition> positionsVector);

void printPositionsOfCrystals(vector<CrystalPosition> positionsVector);

bool checkNeighboringPositions(int i, int j, vector<vector<char>> &maze, const int &length, const int &width);

bool everyCrystalVisited(vector<CrystalPosition> &crystalPositions);

int getDirectionWhenMirror(int direction, char mirror);

vector<vector<char>> getCopyOfMaze(vector<vector<char>> &maze, const int length, const int width);
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
            if (maze[i][j] != CRYSTAL_SIGN && maze[i][j] != BLOCK_SIGN &&
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
            if (maze[i][j] == CRYSTAL_SIGN) {
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

bool checkIfPathIsCorrect(vector<vector<char>> &maze, vector<CrystalPosition> &crystalPositions) {
    int direction = EAST;
    bool isPositionRight = true;
    CellPosition currentPosition = {1, 0};
    vector<CrystalPosition> positionsOfCrystals = vector<CrystalPosition>();
    for (int i = 0; i < crystalPositions.size(); i++) {
        CrystalPosition position = crystalPositions.at(i);
        positionsOfCrystals.push_back(position);
    }

    while ((!everyCrystalVisited(positionsOfCrystals)) && isPositionRight) {
        if (direction == EAST) {
            currentPosition.xposition++;
        }
        if (direction == WEST) {
            currentPosition.xposition--;
        }
        if (direction == NORTH) {
            currentPosition.yposition--;
        }
        if (direction == SOUTH) {
            currentPosition.yposition++;
        }
        //sprawdzam, czy przypadkiem nie wyjdę z labiryntu
        if (currentPosition.yposition == 1 && currentPosition.xposition == 0 && direction == WEST) {
            isPositionRight = false;
        }
        // sprawdzam, czy nie jestem na ścianie
        if (maze[currentPosition.yposition][currentPosition.xposition] == BLOCK_SIGN) {
            isPositionRight = false;
        }

        //    Jezeli jestem na krysztale to muszę go znaleźć i ustawić mu visited na true
        if (maze[currentPosition.yposition][currentPosition.xposition] == CRYSTAL_SIGN) {
            for (int i = 0; i < positionsOfCrystals.size(); i++) {
                if (positionsOfCrystals.at(i).yposition == currentPosition.yposition &&
                    positionsOfCrystals.at(i).xposition == currentPosition.xposition) {

                    positionsOfCrystals[i].visited = true;
                    cout << "Ustawiam visited na true" << endl;
                }
            }
        }
        // Jezeli jestem na lustrze to musze zmienic kierunek
        if (maze[currentPosition.yposition][currentPosition.xposition] == LEFT_MIRROR) {
            direction = getDirectionWhenMirror(direction, LEFT_MIRROR);
        }
        // Jezeli jestem na lustrze to musze zmienic kierunek
        if (maze[currentPosition.yposition][currentPosition.xposition] == RIGHT_MIRROR) {
            direction = getDirectionWhenMirror(direction, RIGHT_MIRROR);
        }
    }
    return isPositionRight;
}

vector<vector<char>> insertMirrorsToTheMaze(vector<vector<char>> &maze, const int &length, const int &width,
                                            vector<MirrorPosition> mirrorPositions) {
    vector<vector<char>> newMaze = vector<vector<char>>(length);
    for (int i = 0; i < length; i++) {
        newMaze[i] = vector<char>(width);
    }
    newMaze = maze;

    for(int i = 0; i < mirrorPositions.size(); i++){
        newMaze[mirrorPositions[i].yposition][mirrorPositions[i].xposition] = mirrorPositions[i].sign;
    }
    return newMaze;

}

// HELPER METHODS:

bool checkNeighboringPositions(int i, int j, vector<vector<char>> &maze, const int &length, const int &width) {
    if (length < 3 && width < 3) return false;
    if (i == 0 || i == (length - 1) || j == 0 || j == (width - 1)) return false;
    if (maze[i - 1][j] == BLOCK_SIGN && maze[i + 1][j] == BLOCK_SIGN) return false;
    if (maze[i][j - 1] == BLOCK_SIGN && maze[i][j + 1] == BLOCK_SIGN) return false;

    return true;
}

bool checkIfAccessible(MirrorPosition startPosition, CellPosition finishPosition, vector<vector<char>> &maze) {
    if ((startPosition.xposition != startPosition.xposition) && (startPosition.yposition != startPosition.xposition)) {
        return false;
    }
    if ((startPosition.xposition == startPosition.xposition) && (startPosition.yposition == startPosition.xposition)) {
        return false;
    }
    int direction = startPosition.direction;
    int commonCoordinate = -1;
    bool result = true;
    if (startPosition.yposition == finishPosition.yposition) {
        commonCoordinate = SAME_Y;
    }
    if (startPosition.xposition == finishPosition.xposition) {
        commonCoordinate = SAME_X;
    }
    if (commonCoordinate == -1) {
        return false;
    }

    if (commonCoordinate == SAME_Y) {
        if (direction == EAST) {
            for (int i = startPosition.xposition + 1; i < finishPosition.xposition; i++) {
                if (maze[startPosition.yposition][i] == BLOCK_SIGN) {
                    result = false;
                }
            }
        }

        if (direction == WEST) {
            for (int i = startPosition.xposition - 1; i > finishPosition.xposition; i--) {
                if (maze[startPosition.yposition][i] == BLOCK_SIGN) {
                    result = false;
                }
            }
        }
    }

    if (commonCoordinate == SAME_X) {
        if (direction == SOUTH) {
            for (int i = startPosition.yposition + 1; i < finishPosition.yposition; i++) {
                if (maze[i][startPosition.xposition] == BLOCK_SIGN) {
                    result = false;
                }
            }
        }

        if (direction == NORTH) {
            for (int i = startPosition.yposition - 1; i > finishPosition.yposition; i--) {
                if (maze[i][startPosition.xposition] == BLOCK_SIGN) {
                    result = false;
                }
            }
        }
    }


    return result;
}

bool everyCrystalVisited(vector<CrystalPosition> &crystalPositions) {
    bool result = true;
    for (int i = 0; i < crystalPositions.size(); i++) {
        if (!crystalPositions.at(i).visited) {
            result = false;
        }
    }
    return result;
}

int getDirectionWhenMirror(int direction, char mirror) {

    if (mirror == LEFT_MIRROR) { //   / takie
        cout << "Jestem na lewym lustrze" << endl; //[TO REMOVE]
        if (direction == NORTH) {
            return EAST;
        }
        if (direction == EAST) {
            return NORTH;
        }
        if (direction == WEST) {
            return SOUTH;
        }
        if (direction == SOUTH) {
            return WEST;
        }
    }
    if (mirror == RIGHT_MIRROR) {
        cout << "Jestem na prawym lustrze " << endl; // [TO REMOVE}
        if (direction == NORTH) {
            return WEST;
        }
        if (direction == WEST) {
            return NORTH;
        }
        if (direction == EAST) {
            return SOUTH;
        }
        if (direction == SOUTH) {
            return EAST;
        }
    }
}

vector<vector<char>> getCopyOfMaze(vector<vector<char>> &maze, const int length, const int width) {
    vector<vector<char>> newMaze = vector<vector<char>>(length);
    for (int i = 0; i < length; i++) {
        newMaze[i] = vector<char>(width);
    }
    newMaze = maze;
    return newMaze;
}


int main(int argc, char *argv[]) {
    int mazeLength;
    int mazeWidth;
    int amountOfMirrors;

    vector<vector<char>> originalMaze;
    vector<vector<char>> newMaze;
    vector<CellPosition> positionsForMirrors = vector<CellPosition>();
    vector<CrystalPosition> positionsOfCrystals = vector<CrystalPosition>();
    vector<vector<vector<MirrorPosition>>> potentialResults;

    //Wczytanie podstawowych danych wejściowych
    readInputArguments(mazeLength, mazeWidth, amountOfMirrors);

    //Przygotowanie wektorów przetrzymujących labirynt
    originalMaze = vector<vector<char>>(mazeLength);
    newMaze = vector<vector<char>>(mazeLength);
    potentialResults = vector<vector<vector<MirrorPosition>>>(amountOfMirrors);

    for (int i = 0; i < mazeLength; i++) {
        originalMaze[i] = vector<char>(mazeWidth);
        newMaze[i] = vector<char>(mazeWidth);
    }

    for(int i = 0; i < amountOfMirrors; i++){
        potentialResults[i] = vector<vector<MirrorPosition>>();
    }

    //Wczytanie labiryntu a także pozycji miejsc na lustra i kryształów
    parseMaze(mazeLength, mazeWidth, originalMaze);
    positionsForMirrors = setPositionsForMirrors(originalMaze, mazeLength, mazeWidth);
    positionsOfCrystals = setPositionsOfCrystals(originalMaze, mazeLength, mazeWidth);


    //Sprawdzenie czy może nie trzeba wstawiać żadnych luster do labiryntu
    bool gotResult = false;
    newMaze = getCopyOfMaze(originalMaze, mazeLength, mazeWidth);
    if (checkIfPathIsCorrect(newMaze, positionsOfCrystals)) {
        gotResult = true;
    }

    //Wstawienie punktu startowego do pierwszej pozycji rozwiązań
    MirrorPosition startPosition = {1, 0, ' ', EAST};
    vector<MirrorPosition> vec1 = vector<MirrorPosition>();
    vec1.push_back(startPosition);
    vector<vector<MirrorPosition>> vec2 = vector<vector<MirrorPosition>>();
    vec2.push_back(vec1);
    potentialResults[0] = vec2;

    while (false) {

//        for(int i = 1; i < amountOfMirrors; i++){
//            int sizeOfPreviousVector = potentialResults[i-1].size(); //ilosc rozwiazan o dlugosci i - 1
//
//            for (int j = 0; j < sizeOfPreviousVector; j++){
//                potentialResults[i-1];
//            }
//
//
//        }


    }

    printMaze(mazeLength, mazeWidth, newMaze);
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