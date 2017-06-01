#include <iostream>
#include <cstdlib>
#include <vector>

int MAX_MAZE_WIDTH = 100;
int MAX_MAZE_LENGTH = 100;
int MAX_CRISTALS_AMOUNT = 100;
char CRISTAL_SIGN = '*';
char BLOCK_SIGN = '#';


using namespace std;

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

void printMaze(const int &length, const int &width, vector<vector<char>> &maze) {
    for(int i = 0; i < length; i++){
        for(int j = 0; j < (width-1); j++){
            cout << maze[i][j];
        }
        cout << maze[i][width-1] << endl;
    }
}


int main(int argc, char *argv[]) {

    int mazeLength;
    int mazeWidth;
    int amountOfMirrors;
    vector<vector<char>> originalMaze;

    readInputArguments(mazeLength, mazeWidth, amountOfMirrors);
    originalMaze = vector<vector<char>>(mazeLength);
    for(int i = 0; i < mazeLength; i++){
        originalMaze[i] = vector<char>(mazeWidth);
    }
    parseMaze(mazeLength, mazeWidth, originalMaze);

    cout << mazeWidth << endl;
    cout << mazeLength << endl;
    cout << amountOfMirrors << endl;

    cout << "Testowe wypisane labiryntu\n\n" << endl;
    printMaze(mazeLength, mazeWidth, originalMaze);

    system("pause");
    return 0;
}