#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>

int MAX_MAZE_WIDTH = 100;
int MAX_MAZE_LENGTH = 100;
int MAX_CRISTALS_AMOUNT = 100;
char CRISTAL_SIGN = '*';
char BLOCK_SIGN = '#';


using namespace std;

// NA POZNIEJ DO ARBITRA
//void readInputArguments(int &xLen, int &yLen, int &numOfMirrors) {
//    cin >> yLen;
//    cin >> xLen;
//    cin >> numOfMirrors;
//    cin.ignore();
//}
//
//void parseMaze(const int &xLen, const int &yLen, vector<vector<char>> &maze) {
//    for (int i = 0; i < yLen; ++i) {
//        maze[i] = vector<char>(xLen);
//        string input;
//        getline(cin, input);
//        for (int j = 0; j < xLen; ++j) {
//            maze[i][j] = input[j];
//        }
//    }
//}
void parseInputDataFromFile (ifstream inputFile){

}
int main(int argc, char *argv[]) {

    int mazeLength;
    int mazeWidth;
    int amountOfMirrors;
    vector<string> originalMaze(1);

    string line;
    ifstream inputText;
    inputText.open("C:/Home/Studia/Semestr_6/7_AdPTO/Project/labirynt.txt", ios_base::in);

    if(inputText.is_open()){
        getline(inputText, line);
        int spacePosition = line.find_first_of(" ");
        mazeLength = atoi(line.substr(0, spacePosition).c_str());
        int lengthOfSecondString = line.length() - (spacePosition + 1);
        mazeWidth = atoi(line.substr(spacePosition + 1, lengthOfSecondString).c_str());
        getline(inputText, line);
        amountOfMirrors = atoi(line.c_str());

        for(int i = 0; i< mazeLength; ++i){
            string vectorLine;
            getline(inputText, vectorLine);
            originalMaze[i] = vectorLine;
        }
    }
    inputText.close();


    cout << mazeWidth << endl;
    cout << mazeLength << endl;
    cout << amountOfMirrors << endl;



    system("pause");
    return 0;
}