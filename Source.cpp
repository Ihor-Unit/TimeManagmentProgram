#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

#include "LineObject.h"


bool cmp(const std::pair<std::string, LineObject> &a, const std::pair<std::string, LineObject>& b) {
    return a.second.time > b.second.time;
}
std::vector<std::pair<std::string, LineObject>> mapSort(const std::map<std::string, LineObject> &map) {
    std::vector<std::pair<std::string, LineObject>> vect;
    
    for (auto& it : map) {
        vect.push_back(it);
    }

    std::sort(vect.begin(), vect.end(), cmp);
    return vect;
}

std::string GetActiveWindowTitle() {
    char wnd_title[256];
    HWND hwnd = GetForegroundWindow();
    GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));
    return wnd_title;
}

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm tstruct{};
    char       buf[40];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%m-%d-%Y", &tstruct);

    return buf;
}

std::string insertSpaces(time_t sp) {
    std::string str = "";
    for (time_t i{}; i < sp; i++)
        str.append(" ");
    return str;
}

std::string toCustTime(time_t sec) {
    time_t h = 0, m = 0, s = 0;
    if (sec >= 60 * 60) {
        h = sec / 3600;
        sec -= h * 3600;
    }
    if (sec >= 60) {
        m = sec / 60;
        sec -= m * 60;
    }
    s = sec;
    return std::to_string(h) + "h " + std::to_string(m) + "m " + std::to_string(s) + "s ";
}

time_t fromCustTime(std::string time) {
    return std::stoi(time.substr(0, time.find('h'))) * 3600 +
           std::stoi(time.substr(time.find('h') + 2, time.find('m'))) * 60 +
           std::stoi(time.substr(time.find('m') + 2, time.find('s')));
}

//counts how many aligning spases in each line
std::map<std::string, time_t> spacingEachLine(const std::map<std::string, time_t> const &frommap) {
    time_t maxsize = 0;

    std::map<std::string, time_t>::const_iterator itr;

    std::map<std::string, time_t> spacingmap;

    for (itr = frommap.begin(); itr != frommap.end(); ++itr) 
        maxsize = (itr->first.size() > maxsize) ? itr->first.size() : maxsize;
    
    for (itr = frommap.begin(); itr != frommap.end(); ++itr) 
        spacingmap.insert(std::pair<std::string, time_t>(itr->first,
            maxsize - itr->first.size()));
    
    return spacingmap;
}

void writeChunk(std::string filename, const std::map<std::string, LineObject> &chunk) {
    std::ofstream outfile;
    outfile.open(filename);

    std::vector<std::pair<std::string, LineObject>>::iterator itr;
    std::vector<std::pair<std::string, LineObject>> sortmap;
    time_t maxsize = 0, t_tot = 0;

    sortmap = mapSort(chunk);

    for (itr = sortmap.begin(); itr != sortmap.end(); ++itr) {
        maxsize = (itr->first.size() > maxsize) ? itr->first.size() : maxsize;
        t_tot += itr->second.time;
    }

    for (itr = sortmap.begin(); itr != sortmap.end(); ++itr) {
        itr->second.spacesLength = maxsize - itr->first.size();
        outfile << std::to_string(itr->second.spacesLength) + "\t" << itr->first
            << insertSpaces(itr->second.spacesLength) + "\t"
            << toCustTime(itr->second.time) << "\n";
    }
    outfile << "\n" + std::to_string(maxsize - 16) << "\tTotal spent time"
        << insertSpaces(maxsize - 16) + "\t"
        << toCustTime(t_tot) << "\n";

    outfile.close();
}

void sliceRigtSpaces(std::string& str) {
    for (int i = str.size() - 1; i >= 0; i--) {
        if (str[i] == 32)
            str = str.substr(0, i);
        else
            break;
    }
}

void startUp(const std::string const &filename, std::map<std::string, LineObject> &chunk) {
    std::ifstream infile;
    infile.open(filename);

    std::map<std::string, time_t> retmap;
    std::map<std::string, time_t> rawmap;
    
    std::map<std::string, time_t> spacingmap;

    if (infile.is_open()) {//if and only if the file exists
        std::string line, leftpart, righpart, centpart;
        
        while (getline(infile, line)) {
            //std::cout << line << std::endl;
            if (line == "") continue;

            size_t lpos = line.find('\t');
            size_t rpos = line.rfind('\t');

            
            leftpart = line.substr(0, lpos);
            centpart = line.substr(lpos+1, rpos - std::stoi(leftpart)-2);
            sliceRigtSpaces(centpart);
            if (centpart  == "Total spent time") continue;
            righpart = line.substr(rpos+1, line.size()); 


            spacingmap[centpart] = std::stoi(leftpart);
                   
            rawmap[centpart] = fromCustTime(righpart);
        }

        for (auto itr = rawmap.begin(); itr != rawmap.end(); ++itr) {
            chunk[itr->first] = LineObject(itr->first.size(), 
                                           spacingmap[itr->first],
                                           rawmap[itr->first]);            
        }
        
    }
}



int main(){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    std::string today = currentDateTime();
    const std::string file = today + ".txt";
    
    std::map<std::string, LineObject> data;//in the output file this map is always sorted 
    
    startUp(file, data);

    bool once = false;
    time_t t_bef = time(0), t_after;
    for (std::string prev = (GetActiveWindowTitle() == "") ? "Desktop or bottom panel" : GetActiveWindowTitle();;) {
        std::string curr = GetActiveWindowTitle();
        if (curr == "") curr = "Desktop, bottom panel or sleep mode";
        

        if (curr.compare(prev) != 0 || !once) {//switched a programm
            t_after = time(0);
            time_t passed = (t_after - t_bef);
            

            if (data[prev] == 0) {
                data[prev].time = passed;
                data[prev].mainPartLength = prev.size();
                writeChunk(file, data);

                t_bef = time(0);
            }
            else {
                time_t tem_datTime = data[prev].time;
                data[prev].time = static_cast<size_t>(tem_datTime + passed);
                data[prev].mainPartLength = prev.size();
                writeChunk(file, data);
                
                t_bef = time(0);
            } 
            
            prev = curr;
            once = true;
        }

        Sleep(50);
    }

    return 0;
} 