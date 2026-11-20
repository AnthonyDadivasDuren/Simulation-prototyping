#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

float calculateDensityAdjustment(int row, int column, int gridWidth,
    const std::vector<float>& fog);
void printFog(int gridWidth, int gridHeight, const std::vector<float>& fog,
    int playerIndex);

int main()
{
    const int gridHeight = 16;
    const int gridWidth = 48;
    const int cellCount = gridWidth * gridHeight;

    int playerRow = 8;
    int playerColumn = 1;

    std::vector<float> fog(cellCount, 1.0f);
    std::vector<int> holdTick(cellCount, 0);

    printFog(gridWidth, gridHeight, fog, playerRow * gridWidth + playerColumn);

    while ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) == 0)
    {
        const int previousRow = playerRow;
        const int previousColumn = playerColumn;

        if ((GetAsyncKeyState('D') & 0x8000) != 0)
        {
            if (playerColumn < gridWidth - 2)
            {
                ++playerColumn;
            }
        }
        else if ((GetAsyncKeyState('A') & 0x8000) != 0)
        {
            if (playerColumn > 1)
            {
                --playerColumn;
            }
        }

        if ((GetAsyncKeyState('W') & 0x8000) != 0 && playerRow > 1)
        {
            --playerRow;
        }

        if ((GetAsyncKeyState('S') & 0x8000) != 0 && playerRow < gridHeight - 2)
        {
            ++playerRow;
        }

        std::vector<float> nextFog = fog;

        for (int row = 1; row < gridHeight - 1; ++row)
        {
            for (int column = 1; column < gridWidth - 1; ++column)
            {
                const int index = row * gridWidth + column;

                if (holdTick[index] > 0)
                {
                    --holdTick[index];
                    continue;
                }

                const float adjustment =
                    calculateDensityAdjustment(row, column, gridWidth, fog);
                nextFog[index] += adjustment;

                const float missingFog = 1.0f - nextFog[index];
                nextFog[index] += missingFog * 0.05f;
            }
        }

        fog = nextFog;

        const int playerIndex = playerRow * gridWidth + playerColumn;
        if (playerRow != previousRow || playerColumn != previousColumn)
        {
            fog[playerIndex] = 0.0f;
            holdTick[playerIndex] = 2;
        }

        std::system("cls");
        printFog(gridWidth, gridHeight, fog, playerIndex);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

float calculateDensityAdjustment(int row, int column, int gridWidth,
    const std::vector<float>& fog)
{
    const int centerIndex = row * gridWidth + column;
    const int cellAbove = (row - 1) * gridWidth + column;
    const int cellBelow = (row + 1) * gridWidth + column;
    const int cellRight = row * gridWidth + column + 1;
    const int cellLeft = row * gridWidth + column - 1;

    const float neighbourAverage =
        (fog[cellAbove] + fog[cellBelow] + fog[cellRight] + fog[cellLeft]) / 4.0f;

    return (neighbourAverage - fog[centerIndex]) / 4.0f;
}

void printFog(int gridWidth, int gridHeight, const std::vector<float>& fog,
    int playerIndex)
{
    std::string frame;
    frame.reserve(gridHeight * (gridWidth + 3));

    for (int row = 0; row < gridHeight; ++row)
    {
        frame += '|';

        for (int column = 0; column < gridWidth; ++column)
        {
            const int index = row * gridWidth + column;

            if (index == playerIndex)
            {
                frame += '@';
                continue;
            }

            const float density = fog[index];

            if (density >= 0.99f)
            {
                frame += '#';
            }
            else if (density >= 0.85f)
            {
                frame += '*';
            }
            else if (density >= 0.75f)
            {
                frame += '+';
            }
            else if (density >= 0.5f)
            {
                frame += ':';
            }
            else if (density >= 0.25f)
            {
                frame += '.';
            }
            else
            {
                frame += ' ';
            }
        }

        frame += "|\n";
    }

    std::cout << frame << std::flush;
}