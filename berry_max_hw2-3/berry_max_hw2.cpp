/*
Name: Maxwell Berry
Date: 10 June, 2021 11:59 PM
Description: This project is meant to print a colored map of an island given a user's inputs. If the user chooses to include -S in their command, 
             they will be given an extra set of prompts
Usage: To use the program, the user must enter <exe> <-S> (optional) into the command line. This will then prompt the user for a 
       wide variety of information regarding the island. The specifications for the first few arguments will be layed out, but for the
       number of particles, I'd recommend a high value (atleast a couple hundred) with a modest maxLife (10+). I also recommend the waterline 
       be relatively low (50-80). If you choose the '-S' option, then, after the island is printed, you will get an option to drop more particles.
       Follow the prompt, and the usage should be self-explanatory.
*/

#include <iostream>
#include <fstream>
#include <time.h>
#include <cstring>
#include "termcolor.hpp"

using namespace std;

// Instantiation of all the necessary functions
int** makeParticleMap(int**array, int width, int height, int winX, int xMax, 
                                     int winY, int yMax, int numParticles, int maxLife);
bool moveExists(int** map, int width, int height, int particleX, int particleY);
int findMax(int** map, int width, int height);
void normalizeMap(int** map, int width, int height, int maxVal);
void printMap(int** array, int width, int height, int winX, int xMax, 
                               int winY, int yMax, int particles, int maxLife, int waterline);
void colorizer(char** isPolished, int height, int width);
void specialInterface(int**array, int width, int height, int particles, int maxLife, int waterline);
int main(int argc, char** argv) {
        
        int width = -1; // Width of the total array.
        int height = -1; // Height of the total array.
        int winX = 0;  // X Coordinate used to create drop-window.
        int xMax = -1; // Width of drop-window from the x coord.
        int winY = 0; // y Coordinate used to create drop-window. 
        int yMax = -1; // Height of drop-window from the y coord.
        int particles; // Particles increase each array value by 1.
        int maxLife; // How many times the particles can travel
        int waterline;
        bool prompt = false;
        
        srand((unsigned int)time(NULL)); // Seeds the random function. Allows it to be different everyt time the program runs.

        if (argc > 1) {     // Checks if the user enters a second argument or not.  
                
                if (strcmp(argv[1], "-S") != 0) {

                        cout << "Error // Invalid Character." << endl; 
                        return -1;
                }
        }        
        
        cout << "Welcome to Maxwell Berry’s CSE240 island generator! If you'd like, enter the '-S' command for a special interface.\n" << endl;
        
        while (prompt == false) { // Loops until the user enters the correct width value.
                cout << "Enter grid width: ";
                        cin >> width;

                if (width <= 0) {
                        
                        cout << "Error // Grid width must be greater than zero." << endl;
                } else if (width > 0) {
                        
                        prompt = true;
                } 
        }

        prompt = false;
        while (prompt == false) { // Loops until the user enters the correct height value.
                cout << "Enter grid height: ";
                        cin >> height;

                if (height <= 0) {
                        
                        cout << "Error // Grid height must be positive or greater than zero." << endl;
                } else if (height > 0) {
                        
                        prompt = true;
                }
        }

        while (xMax <= winX && yMax <= winY) { // Loop to check that the values meet the specifications.
                prompt = false;
                while (prompt == false) { // Loops until the user enters the correct winX value.
                        cout << "Enter drop-window x-coordinate (0 -<width>): ";
                                cin >> winX;

                        if (winX <= 0 || winX > width) {
                                
                                cout << "Error // The maximum x value must be positive and less than the width." << endl;
                        } else if (winX >= 0 && winX < width) {
                                
                                prompt = true;
                        } 
                }

                prompt = false;
                while (prompt == false) { // Loops until the user enters the correct winY value.
                        cout << "Enter drop-window y-coordinate (0 -<height>): ";
                                cin >> winY;

                        if (winY <= 0 || winY > height) {
                                
                                cout << "Error // The minimum y value must positive and less than the height." << endl;
                        } else if (winY > 0) {
                                
                                prompt = true;
                        } 
                }

                prompt = false;
                while (prompt == false) { // Loops until the user enters the correct xMax value.
                        cout << "Enter drop-window width(>= 1): ";
                                cin >> xMax;

                        if (xMax < 1 || xMax < winX) {
                                
                                cout << "Error // The value must be greater than 1 and greater than the drop-window x coordinate." << endl;
                        } else if (xMax > 0) {
                                
                                prompt = true;
                        } 
                }


                prompt = false;
                while (prompt == false) { // Loops until the user enters the correct yMax value.
                        cout << "Enter drop-window height(>= 1): ";
                                cin >> yMax;

                        if (yMax < 1 || yMax < winY) {
                                cout << "Error // The maximum y value must greater than 1 and greater than the drop-window y coordinate." << endl;

                        } else if (yMax > 0) {
                                
                                prompt = true;
                        } 
                }

                if (xMax <= winX) {
                        
                        cout << "Error // The minimum X value must be less than the maximum." << endl;   
                }

                if (yMax <= winY) {
                        
                        cout << "Error // The minimum y value must be less than the maximum." << endl;   
                }

        }
        
        prompt = false;
        while (prompt == false) { // Loops until the user enters the correct particle value.
                cout << "Enter number of particles to drop: ";
                        cin >> particles;

                if (particles <= 0) {
                        
                        cout << "Error // particles must be positive or greater than zero." << endl;
                } else if (particles > 0) {
                        
                        prompt = true;
                } 
        }
        
        prompt = false;
        while (prompt == false) { // Loops until the user enters the correct maxLife value.
                cout << "Enter maxLife of particles: ";
                        cin >> maxLife;

                if (maxLife <= 0) {
                        
                        cout << "Error // maxLife must be positive or greater than zero." << endl;
                } else if (maxLife > 0) {                       
                        
                        prompt = true;
                } 
        }

        prompt = false;
        while (prompt == false) { // Loops until the user enters the correct waterline value.
                cout << "Enter value for waterline (40-200): ";
                        cin >> waterline;

                if (waterline < 40 || waterline > 200) {
                        
                        cout << "Error // waterline must be between 40 and 200." << endl;
                } else if (waterline >= 40 || waterline <= 200) {
                        
                        prompt = true;
                } 
        } 

        int** array = new int*[height]; // Instantiates the main array used in the program.
        for (int i = 0; i < height; i++) {
                
                array[i] = new int[width];
        } 

        for (int i = 0; i < height; i++) { // Sets the inital array values to 0.
                
                for(int j = 0; j < width; j++) {
                        
                        array[i][j] = 0;
                }
        }

        printMap(array, width, height, winX, xMax, 
                           winY, yMax, particles, maxLife, waterline); //Calling the printMap function.
        
        char yOrN;
        if (argc > 1) { // Cheks if the user entered more than one argument for the command line.    
                
                if (strcmp(argv[1], "-S") == 0) { // Prompts the user if they used the special interface.

                        while (prompt == true) {
                                cout << "Would you like to drop more particles? (y/n): ";
                                        cin >> yOrN;

                                if (yOrN == 'y') {
                                        
                                        specialInterface(array, width, height, particles, maxLife, waterline);
                                } else if (yOrN == 'n') {
                                        prompt = false;

                                } else if (yOrN != 'y' || yOrN != 'n') {
                                        
                                        cout << "Invalid Response (must be 'y' or 'n'). Please try again." << endl;
                                }
                        }
                }
        }
        return 0;
}

int** makeParticleMap(int** array, int width, int height, int winX, int xMax, 
                                       int winY, int yMax, int numParticles, int maxLife) { // This function creates the particle map
                                                                                           //   based on the user's inputs.
        int particleX;
        int particleY;
        for (int i = 0; i < numParticles; i++) { 
                
                particleX = rand() % (xMax - winX + 1) + winX; // Selects a random value in the array for the drop particle to be placed.
                        particleX--;
                particleY = rand() % (yMax - winY + 1) + winY; // Same thing for the Y.
                        particleY--;

                array[particleY][particleX]++; // Increases the value of that element as it passes through the loop.

                for (int i = 0; i < maxLife; i++) {
                        
                        if (moveExists(array, width, height, particleX, particleY) == true) { // Checks if there is an available move.
                                
                                int moves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, 
                                                        {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}; // This array is the coordinates each drop particle can move 
                                                                                                  // (up, down, left, right, etc.).
                                bool promptLoop = true;
                                while (promptLoop == true) {
                                        
                                        int randMovesIndex = rand() % 8; // 8 Number of possible moves
                                        int moveX = particleX + moves[randMovesIndex][0];
                                        int moveY = particleY + moves[randMovesIndex][1]; // Picks a random position to move. 
                                        if (moveY >= 0 && moveX >= 0  && moveY < height && moveX < width) {
                                                
                                                if (array[moveY][moveX] <= array[particleY][particleX]) {
                                                        
                                                        particleX = moveX;
                                                        particleY = moveY;
                                                        array[particleY][particleX]++;
                                                        
                                                        promptLoop = false;
                                                }
                                        }
                                }
                        } 
                }
        }

        return array;
}

bool moveExists(int** map, int width, int height, int particleX, int particleY) { // A boolean function that checks the availability for a particle to move.
        
        int moves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, 
                                {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}; // Same array as previously used.
        int moveX;
        int moveY;
        for (int i = 0; i < 8; i++) {
                
                moveX = particleX + moves[i][0];
                moveY = particleY + moves[i][1];
                if (moveY >= 0 && moveX >= 0  && moveY < height && moveX < width) {
                        
                        if (map[moveY][moveX] <= map[particleY][particleX]) {
                                
                                return true; // Returns true when a move is available.
                        }
                } else {

                        return false; // Returns false otherwise.
                }
        }
        return false;
}

int findMax(int** map, int width, int height) { // Finds the maximum value within the array.
        
        int maxValue = 0;
        for (int i = 0; i < height; i++) {
               
                for (int j = 0; j < width; j++) {
                        
                        if (map[i][j] > maxValue) {
                                
                                maxValue = map[i][j];
                        }
                }
        }
        return maxValue;
}

void normalizeMap(int** map, int width, int height, int maxVal) { // Performs a normalization operation on all the values on the map.
        
        for(int i = 0; i < height; i++) {
                
                for(int j = 0; j < width; j++) {
                        
                        map[i][j] = (map[i][j] / (double) maxVal) * 255; // Floating point division by the max value, multiplied by 255.
                }
        }
}

void printMap(int**array, int width, int height, int winX, int xMax, 
                                int winY, int yMax, int particles, int maxLife, int waterline) { // This is the function that prints the map onto the text file by 
                                                                                                // calling other functions, looping through the array, and assinging 
                                                                                               // each value their respected character.

        ofstream myfile ("island.txt"); // The text file with the island printout.
        int** isOriginal = makeParticleMap(array, width, height, winX, xMax, winY, yMax, particles, maxLife);
        int max = findMax(isOriginal, width, height);
        normalizeMap(isOriginal, width, height, max);

        // Initializing this array.
        char** isPolished = new char*[height];
        for(int i = 0; i < height; i++) {
                
                isPolished[i] = new char[width];
        } 

        int landzone = (255 - waterline); // Definition of landzone.
        for (int i = 0; i < height; i++) { // These loops go through the values of the array and assigns them their appropriate characters.
                
                for (int j = 0; j < width; j++) {
                        
                        if (isOriginal[i][j] < (waterline * 0.5)) {
                                
                                isPolished[i][j] = '#';
                        } else if (isOriginal[i][j] > (waterline * 0.5) && 
                                                            isOriginal[i][j] <= waterline) {
                                
                                isPolished[i][j] = '~';
                        } else {
                                if (isOriginal[i][j] < (waterline + (0.15 * landzone))) {
                                       
                                        isPolished[i][j] = '.';
                                } else if (isOriginal[i][j] >= (waterline + (0.15 * landzone)) && 
                                                                        isOriginal[i][j] < (waterline + (0.40 * landzone))) {
                                        
                                        isPolished[i][j] = '-';
                                } else if (isOriginal[i][j] >= (waterline + (0.40 * landzone)) && 
                                                                        isOriginal[i][j] < (waterline + (0.80 * landzone))) {
                                        
                                        isPolished[i][j] = '*';
                                } else {
                                        
                                        isPolished[i][j] = '^';
                                }
                        }
                }
        }

        // Prints the final array onto the text file.
        for(int i = 0; i < height; i++) {
                for(int j = 0; j < width; j++) {
                        
                        myfile << isPolished[i][j];
                }
                myfile << endl;
        }
        cout << endl;
        colorizer(isPolished, height, width);
        myfile.close();
}

void colorizer(char** isPolished, int height, int width) { // Optional function used to print the island in color.
        
        for (int i = 0; i < height; i++) { // Loop specifying that each character must be assigned a certain color.
                for (int j = 0; j < width; j++) {
                        
                        if (isPolished[i][j] == '#') { 
                                
                                cout << termcolor::blue << "#";
                        } else if (isPolished[i][j] == '~') {
                                
                                cout << termcolor::cyan << "~";
                        } else if (isPolished[i][j] == '.') {
                                
                                cout << termcolor::yellow << ".";
                        } else if (isPolished[i][j] == '-') {
                                
                                cout << termcolor::bright_green << "-";
                        } else if (isPolished[i][j] == '*') {
                                
                                cout << termcolor::green << "*";   
                        } else if (isPolished[i][j] == '^') {
                                
                                cout << termcolor::grey << "^";  
                        }                  
                }
                cout << endl;
        }
        cout << endl;
        cout << termcolor::reset;
}

void specialInterface(int**array, int width, int height, int particles, int maxLife, int waterline) { // This is an extra function used to prompt the user after the island 
                                                                                                     // has been printed. If the user would like to create a new drop-window
                                                                                                    // and drop more particles, they can with this function.
        bool prompt = false;
        int winX = 0; 
        int xMax = -1; 
        int winY = 0;
        int yMax = -1;

        while (xMax <= winX && yMax <= winY) { // Repeat of the loops from the main function
                
                prompt = false;
                while (prompt == false) { // Loops until the user enters the correct winX value.
                        cout << "Enter drop-window x-coordinate (0 -<width>): ";
                                cin >> winX;

                        if (winX <= 0 || winX > width) {
                                
                                cout << "Error // The maximum x value must be positive and less than the width." << endl;
                        } else if (winX >= 0 && winX < width) {
                                
                                prompt = true;
                        } 
                }

                prompt = false;
                while (prompt == false) { // Loops until the user enters the correct winY value.
                        cout << "Enter drop-window y-coordinate (0 -<height>): ";
                                cin >> winY;

                        if (winY <= 0 || winY > height) {
                                
                                cout << "Error // The minimum y value must positive and less than the height." << endl;
                        } else if (winY > 0) {
                                
                                prompt = true;
                        } 
                }

                prompt = false;
                while (prompt == false) { // Loops until the user enters the correct xMax value.
                        cout << "Enter drop-window width(>= 1): ";
                                cin >> xMax;

                        if (xMax < 1 || xMax < winX) {
                                
                                cout << "Error // The value must be greater than 1 and greater than the drop-window x coordinate." << endl;
                        } else if (xMax > 0) {
                                
                                prompt = true;
                        } 
                }


                prompt = false;
                while (prompt == false) { // Loops until the user enters the correct yMax value.
                        cout << "Enter drop-window height(>= 1): ";
                                cin >> yMax;

                        if (yMax < 1 || yMax < winY) {
                                cout << "Error // The maximum y value must greater than 1 and greater than the drop-window y coordinate." << endl;

                        } else if (yMax > 0) {
                                
                                prompt = true;
                        } 
                }

                if (xMax <= winX) {
                        
                        cout << "Error // The minimum X value must be less than the maximum." << endl;   
                }

                if (yMax <= winY) {
                        
                        cout << "Error // The minimum y value must be less than the maximum." << endl;   
                }

        }

        prompt = false;
        while (prompt == false) {
                cout << "Enter number of particles to drop: ";
                        cin >> particles;

                if (particles <= 0) {
                        
                        cout << "Error // particles must be positive or greater than zero." << endl;
                } else if (particles > 0) {
                        
                        prompt = true;
                } 
        }
        
        prompt = false;
        while (prompt == false) {
                cout << "Enter maxLife of particles: ";
                        cin >> maxLife;

                if (maxLife <= 0) {
                        
                        cout << "Error // maxLife must be positive or greater than zero." << endl;
                } else if (maxLife > 0) {                       
                        
                        prompt = true;
                } 
        }

       printMap(array, width, height, winX, xMax, winY, yMax, particles, maxLife, waterline); // Simply recalls the printMap function to add the new particles.
}