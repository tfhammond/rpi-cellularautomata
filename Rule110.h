#include <iostream>
#include <bitset>

#include "led-matrix.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

#include <vector>
#include <iostream>

class Rule110
{
private:
    std::vector<int> patternVector;
    Canvas *canvas;

    // Function to generate a new pattern based on the given pattern
    std::vector<int> generateNewPattern() const;

public:
    // Constructor to initialize the pattern vector
    Rule110(int numberOfColumns, Canvas);

    // Function to print the elements of the pattern vector
    void printPattern(int,int);

    // Function to simulate the Rule110 for a given number of iterations
    void simulate(int iterations);
};

// Constructor to initialize the pattern vector
Rule110::Rule110(int numberOfColumns, Canvas *canvas_)
{
    patternVector = std::vector<int>(numberOfColumns, 0);
    patternVector[numberOfColumns / 2] = 1;
    canvas = canvas_;
}

// Function to generate a new pattern based on the given pattern
std::vector<int> Rule110::generateNewPattern() const
{
    std::vector<int> newPatternVector(patternVector);

    for (size_t i = 1; i < patternVector.size() - 1; ++i)
    {
        int value = 100 * patternVector[i - 1] + 10 * patternVector[i] + patternVector[i + 1];

        // Apply the rules to update the center cell
        newPatternVector[i] = (value == 100 || value == 1 || value == 10 || value == 11) ? 1 : 0;
    }

    return newPatternVector;
}

// Function to print the elements of the pattern vector
void Rule110::printPattern(int col, int row)
{
    for (int value : patternVector)
    {
        if (value == 1){
            canvas->SetPixel(col,row,255,255,255);
            usleep(1 * 1000);
        }
        else{
            canvas->SetPixel(col,row,0,0,0);
            usleep(1 * 1000);
        }
        std::cout << value;

        col++;

    }

    
    std::cout << std::endl;
}

// Function to simulate the Rule110 for a given number of iterations
void Rule110::simulate(int iterations)
{
    // Print the initial pattern
    printPattern(0,0);

    // Generate and print new patterns for the specified number of iterations
    for (int i = 0; i < iterations; ++i)
    {
        patternVector = generateNewPattern();
        printPattern(0,i);
    }
}
