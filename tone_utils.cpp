#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include "tone_utils.hpp"

const std::vector<std::string> NOTES = {"A","A#","B","C","C#","D","D#","E","F","F#","G","G#"};

std::pair<std::string, double> getClosestPitch(double pitch, double concertPitch) {
    int i = nearbyint(log2(pitch / concertPitch) * 12);
    std::string octaveNum = std::to_string(4 + (int)floor((i + 9) / 12.0));
    int index = (i % 12 + 12) % 12;
    std::string closestNote = NOTES[index] + octaveNum;
    double closestPitch = concertPitch * pow(2, i / 12.0);
    return std::make_pair(closestNote, closestPitch); 
}
