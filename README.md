# Train Trips
This program will check the route and price of train tickets between stations. The representation of rail connections between stations is stored in distances1.txt. 

## Introduction
This program will check the route and price of train tickets between stations. The representation of rail connections between stations is stored in distances1.txt and will need to load and specified as a command line argument. The program should load this file and use it to make a graph representation of the train network. It should then prompt the user to enter the names of the start and end stations, and then use some graph algorithm to decide the shortest route between the stations, and display it to the user. It should repeatedly do this until the user wants to quit the program.
- This is a university project finished on Dec 2018
- Author: [Yichun Zhang](https://github.com/missyQWQ)

## Tasks
1. If the program is run without the correct number of command line parameters, it will exit with the error message "Invalid command line arguments. Usage: train <distances.txt>". The filename is a relative or absolute pathname which can be understood by fopen. If the file cannot be opened, perror is used to print "Cannot open file." together with the operating system specific error message and exit with exit code 1.
2. The program will load data from the file into a graph data-structure, where the stations are vertexes, the connections are directed edges, and distances are edge weights. Then this data-structure is used to calculate the journeys required below. The program will not have to access the file again beyond this point. Note that connections do not have to by symmetric, ie, there may be pairs of stations where people can travel from A to B but not B to A. The graph data will be a weakly connected directed graph.
3. Once the graph has been created, the program will prompt the user with the message "Start station: " and read in a string. The user can quit the program by entering nothing for the start station; the program should then exit with exit code 0. Otherwise, if that string is not the name of a station then the program should print the error message "No such station." and prompt for the start station again. All user input should be matched case-sensitively.
4. After the start station, the program will prompt the user with the message "End station: " and read in a string. If that string is not the name of a station then the program should print the error message "No such station." and prompt for the start station again. If the start and end station are the same, the program should print "No journey, same start and end station." and prompt for the start station again.
5. If both station names are valid, the program will calculate the shortest journey from the start to the end station. The program will print out the journey in the following format, where S and E are the start and end station names; C1, C2, C3, ... are all the intermediate station names that the journey will go through; XXX is the total distance of the journey; and YYY is the total cost of the journey. If there are no intermediate stations, then the output display "direct" instead of "via" (and obviously not print any intermediate stations). Once this has been printed out then it should prompt for the start station again.
```
From S
via
C1
C2
C3
...
To E
Distance XXX km
Cost YYY RMB
```

6. If there is no possible journey between the stations then the program will print "No possible journey." and prompt for the start station again.
7. The cost of the journey is calculated as the following and if the result is not a whole number then it will be rounded up to the next nearest integer.
```
(the total distance multiplied by 1.2) + (the number of intermediate stations multiplied by 25)
```

8. If the program needs to exit because it cannot allocate memory, it will print the error message "Unable to allocate memory." and exit with exit code 3. And exit with exit code 4 for any other reasons.
9. Free any memory which is no longer needed and this program doesn't have any memory leaks.

## Visualisation of Distance File
![distance-table](/distance-table.png)
- Lines are separated by a single newline character '\n'.
- On each line, there are multiple cells. Each cell is separated by a comma character ','.
- Each cell may or may not have a value. Cells without values have nothing between commas or between the start/end of the line and the comma.
- Each value can contain any number of any printable ASCII characters, excluding the comma ',' and newline '\n' characters.
- Blank lines (lines with no characters) at the end of the file should be ignored.
- The first line of values in the cells are organised like a table. The first row are the names of stations. The first column are the name of stations. The top-left cell should be empty. The rest of the values of the cells represents the distance in kilometers from the station of that row to the station of that column (ie, from the station of the left to the station on the top). If the cell is empty, that represents no direct connection between those two stations.
- Files which do not match this format are invalid. In particular, if a distance cell contains something other than a positive non-zero integer or empty then it is invalid.
- In the case of invalid files, the program should print the error message "Invalid distances file." and exit with exit code 2.

## Example Input/Output
Running the program and just pressing return:
```
yichun $ ./train distances1.txt
Start station:
yichun $
```
Running the program:
```
yichun $ ./train distances1.txt
Start station: Ningbo
End station: Suzhou
From Ningbo
via
Hangzhou
Shanghai
To Suzhou
Distance 425 km
Cost 560 RMB
Start station: Ningbo
End station: Ningbo
No journey, same start and end station.
Start station: Glasgow
No such station.
Start station: Nanjing
End station: Glasgow
No such station.
Start station: 341ed admom1 q!!!!
No such station.
Start station: Wenzhou
End station: Fuzhou
From Wenzhou
direct
To Fuzhou
Distance 325 km
Cost 390 RMB
Start station:
yichun $
```
Running the program with an invalid file:
```
yichun $ ./train invalid1.txt
Invalid distances file.
yichun $
```
Running the program with a wrong filename:
```
yichun $ ./train missing.txt
Cannot open file.<OS-specific perror message here>
yichun $
```
