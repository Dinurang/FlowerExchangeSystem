#ifndef GUI_H
#define GUI_H

#include <string>

// Open the output CSV in a text viewer or editor
void openOutputCSV();
// Open a file dialog to select a CSV file and return the selected file path
std::string get_file_path_from_gui();

#endif