#include <iostream>
#include <vector>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "TemplateArgumentsIssues"

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
                CrystalPosition crystalPosition;
                if (i == 1 && j == 0){
                    crystalPosition = {
                            i, j, true,
                    };
                } else {
                    crystalPosition = {
                            i, j, false,
                    };
                }
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

    for (int i = 0; i < mirrorPositions.size(); i++) {
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

bool checkNextSign(MirrorPosition mirrorPosition, vector<vector<char>> &maze){
    if(mirrorPosition.direction == NORTH){
        return !(maze[mirrorPosition.yposition-1][mirrorPosition.xposition] == BLOCK_SIGN);
    }

    if(mirrorPosition.direction == SOUTH){
        return !(maze[mirrorPosition.yposition+1][mirrorPosition.xposition] == BLOCK_SIGN);
    }

    if(mirrorPosition.direction == EAST){
        return !(maze[mirrorPosition.yposition][mirrorPosition.xposition+1] == BLOCK_SIGN);
    }

    if(mirrorPosition.direction == WEST){
        return !(maze[mirrorPosition.yposition][mirrorPosition.xposition-1] == BLOCK_SIGN);
    }
}

bool checkIfAccessible(MirrorPosition startPosition, CellPosition finishPosition, vector<vector<char>> &maze) {
    if ((startPosition.yposition != finishPosition.yposition) && (startPosition.xposition != finishPosition.xposition)) {
        return false;
    }
    if ((startPosition.yposition == finishPosition.yposition) && (startPosition.xposition == finishPosition.xposition)) {
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
        if (direction == NORTH){
            return false;
        }

        if(direction == SOUTH){
            return false;
        }

        if (direction == EAST) {
            if(startPosition.xposition > finishPosition.xposition){
                return false;
            }
            for (int i = startPosition.xposition + 1; i < finishPosition.xposition; i++) {
                if (maze[startPosition.yposition][i] == BLOCK_SIGN) {
                    result = false;
                }
            }
        }

        if (direction == WEST) {
            if(startPosition.xposition < finishPosition.xposition){
                return false;
            }
            for (int i = startPosition.xposition - 1; i > finishPosition.xposition; i--) {
                if (maze[startPosition.yposition][i] == BLOCK_SIGN) {
                    result = false;
                }
            }
        }
    }

    if (commonCoordinate == SAME_X) {
        if(direction == EAST){
            return false;
        }

        if(direction == WEST){
            return false;
        }

        if (direction == SOUTH) {
            if(startPosition.yposition > finishPosition.yposition){
                return false;
            }
            for (int i = startPosition.yposition + 1; i < finishPosition.yposition; i++) {
                if (maze[i][startPosition.xposition] == BLOCK_SIGN) {
                    result = false;
                }
            }
        }

        if (direction == NORTH) {
            if(startPosition.yposition < finishPosition.yposition){
                return false;
            }
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

    if (mirror == LEFT_MIRROR) {
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
    potentialResults = vector<vector<vector<MirrorPosition>>>(amountOfMirrors + 1);

    for (int i = 0; i < mazeLength; i++) {
        originalMaze[i] = vector<char>(mazeWidth);
        newMaze[i] = vector<char>(mazeWidth);
    }

    for (int i = 0; i < (amountOfMirrors + 1); i++) {
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
    MirrorPosition startPosition = {1, 0, originalMaze[1][0], EAST};
    vector<MirrorPosition> vec1 = vector<MirrorPosition>();
    vec1.push_back(startPosition);
    vector<vector<MirrorPosition>> vec2 = vector<vector<MirrorPosition>>();
    vec2.push_back(vec1);
    potentialResults[0] = vec2;

        for (int i = 1; i < (amountOfMirrors + 1) && gotResult == false; i++) { // i oznacza ilosc luster w tworzonych rozwiazaniach
            int amountOfPreviousSizeResults = potentialResults[i - 1].size();
            for(int j = 0; j < amountOfPreviousSizeResults && gotResult == false; j++) { // j sluzy jedynie do przejscia po wszystkich rozwiazaniach o rozmiarze mniejszym o 1 od aktualnie tworzonych
                vector<MirrorPosition> result = potentialResults[i - 1][j];
                MirrorPosition lastElementOfResult = result[i-1];
                for(int k = 0; k < positionsForMirrors.size() && gotResult == false; k++) { //k sluzy jedynie do przejscia po wszystkich mozliwych miejscach dla luster i zobaczenia czy sa one dostepne z poziomu ostatniego lustra przetawarzanego rozwiazania
                    if(checkIfAccessible(lastElementOfResult, positionsForMirrors[k], originalMaze)){
                        vector<MirrorPosition> newResult1 = vector<MirrorPosition>(i+1); //i+1 poniewaz w rozwiazaniu bedzie tez punkt startowy
                        vector<MirrorPosition> newResult2 = vector<MirrorPosition>(i+1);
                        for(int l = 0; l < i; l++){
                            newResult1[l] = result[l];
                            newResult2[l] = result[l];
                        }
                        MirrorPosition lastPosition1 = {positionsForMirrors[k].yposition, positionsForMirrors[k].xposition, LEFT_MIRROR, getDirectionWhenMirror(lastElementOfResult.direction, LEFT_MIRROR)};
                        MirrorPosition lastPosition2 = {positionsForMirrors[k].yposition, positionsForMirrors[k].xposition, RIGHT_MIRROR, getDirectionWhenMirror(lastElementOfResult.direction, RIGHT_MIRROR)};

                        newResult1[i] = lastPosition1;
                        newMaze = getCopyOfMaze(originalMaze, mazeLength, mazeWidth);
                        newMaze = insertMirrorsToTheMaze(newMaze, mazeLength, mazeWidth, newResult1);
                        if (checkIfPathIsCorrect(newMaze, positionsOfCrystals)){
                            gotResult = true;
                        }

                        newResult2[i] = lastPosition2;
                        if(gotResult == false){
                            newMaze = getCopyOfMaze(originalMaze, mazeLength, mazeWidth);
                            newMaze = insertMirrorsToTheMaze(newMaze, mazeLength, mazeWidth, newResult2);
                            if (checkIfPathIsCorrect(newMaze, positionsOfCrystals)){
                                gotResult = true;
                            }
                        }

                        if(checkNextSign(lastPosition1, originalMaze)){
                            potentialResults[i].push_back(newResult1);
                        }
                        if(checkNextSign(lastPosition2, originalMaze)){
                            potentialResults[i].push_back(newResult2);
                        }
                    }
                }
            }
        }

    if(gotResult){
        cout << mazeLength << " " << mazeWidth << endl;
        cout << amountOfMirrors << endl;
        printMaze(mazeLength, mazeWidth, newMaze);
    }
    else {
        printMaze(mazeLength, mazeWidth, originalMaze);
    }
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