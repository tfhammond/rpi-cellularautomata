#include <iostream>
#include <bitset>

#include "led-matrix.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

class Rule110Automaton {
public:
    Rule110Automaton(int numCells, int numGenerations, Canvas *canvas) : numCells_(numCells), numGenerations_(numGenerations) {
        // Initialize the state with a single '1' in the middle
        currentState_.set(7, true);
    }

    void run() {
        canvas->Fill(0,0,0);
        // Print the initial state
        printState();

        // Generate and print the next generations
        for (int generation = 1; generation <= numGenerations_; ++generation) {
            std::bitset<8> nextState;

            // Apply Rule 110 to each cell and update the state
            for (int i = 0; i < numCells_; ++i) {
                bool left = (i > 0) ? currentState_[i - 1] : 0;
                bool center = currentState_[i];
                bool right = (i < numCells_ - 1) ? currentState_[i + 1] : 0;

                nextState[i] = applyRule110(left, center, right);
            }

            // Print the current state
            printState();

            // Update the current state for the next generation
            currentState_ = nextState;
        }
    }

private:
    std::bitset<8> currentState_;
    int numCells_;
    int numGenerations_;

    bool applyRule110(bool left, bool center, bool right) {
        return (left ^ (center || right));
    }

    void printState() const {
        for (int i = 7; i >= 0; --i) {
            std::cout << (currentState_[i] ? canvas->SetPixel(j, i, 255,255,255) : canvas->SetPixel(j, i, 0,0,0));
        }
    }
};