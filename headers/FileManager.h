#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>

class FileManager {

std::string _filename;

public:

    FileManager(std::string filename) : _filename(filename) {};

    void newLine(char newLine[]);
    void shareLine();
};

inline void FileManager::newLine(char newLine[]) 
{
    std::fstream fs;
    int index = 0;
    std::string line;

    while(newLine[index] != '\n')
    {
        line += newLine[index];
        index++;
        
    }

    fs.open("log.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    fs << line << std::endl;
    fs.close();

}

inline void FileManager::shareLine() 
{
    std::cout << "shareLine" << std::endl;
}

#endif