// Helper functions for music
#include <cs50.h>
#include <math.h>
#include <string.h>

#include "helpers.h"

// Notes in an octave
const string ALL_NOTES[] = {"C", "C#", "D", "D#", "E", "F",
                            "F#", "G", "G#", "A", "A#", "B"
                           };

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    return 8 * atoi(&fraction[0]) / atoi(&fraction[2]);
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    // change b to # of lower note
    if (note[1] == 'b')
    {
        note[1] = '#';
        if (note[0] == 'A')
        {
            note[0] = 'G';
        }
        else
        {
            --note[0];
        }
    }

    int octave;
    // remove the octave so note can be compared with NOTES[]
    if (note[1] == '#')
    {
        octave = atoi(&note[2]);
        note[2] = '\0';
    }
    else
    {
        octave = atoi(&note[1]);
        note[1] = '\0';
    }

    // find the note
    int i = 0;
    while (strcmp(note, ALL_NOTES[i++]) != 0)
    {
    }

    // calculate the distance to A4
    int distance = i - 10 + 12 * (octave - 4);
    // calculate and return the frequency
    return round(440 * pow(2, distance / 12.0));
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    return !strlen(s);
}