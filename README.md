# About 
TimeManagmentProgram(TTM) helps you to track the time spent when sitting in front of your computer. This program is extremely simple and can't do much.
## How does it store data?
It creates a file in the location folder according to the template: "month-day-year.txt" (where the month, day and year is corresponding to the current date). When you run the program it looks for a corresponding to the current date file, and if there is a file it pasres the file then rewrite it every time you switch your current working program. Actually, TTM is ultra low-consumption as it cycles with some delay(in milliseconds) to avoid overloading your computer.

## How to set things up?
Download the project and compile the files in [MS Visual Studio](https://visualstudio.microsoft.com). If you use different IDE I can't enshure you that it'll compile properly because of using VS workload dlls. At the end you should get a `.exe` executable file. 

**But if you are good with the compilation:**
 - Choose a folder where you want the program to store and create files with the data
 - Create a shortcut to the `.exe` file 
 - Press Win + R and write `shell:startup`
 - Move the shortcut to the startup folder and run the shortcut or restart your pc.
## Backstory
I wrote this profram to better manage my time. I just started to feel that I waste my time, so I desided to manage my time better. 
