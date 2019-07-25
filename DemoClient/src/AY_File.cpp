#include "pch.h"
#undef UNICODE
/*
=========== Demo Configurations ================
Server IP				192.168.2.149	
Client 1				192.168.2.145	74-D4-35-3C-4A-B2		74-D4-35-3C-4A-B2-74-D4-35-3C-4A-B2		
Client 2				192.168.2.146	74-D4-35-3C-4A-B3		74-D4-35-3C-4A-B3-74-D4-35-3C-4A-B3
Device 1				192.168.2.147
Device 1 Ghost			192.168.2.148
*/
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include <AY_Printf.h>
#include <AY_Functions.h>
#include <AY_ClientSocket.h>
#include <AY_Client.h>
#include <AY_ClientPrjtBased.h>
#include <AY_Crypt.h>
#include <AY_Memory.h>
#include <AY_Command.h>
#include <AY_File.h>

/* File Handling with C++ using ifstream & ofstream class object*/
/* To write the Content in File*/
/* Then to read the content of file*/
#include <iostream>
/* fstream header file for ifstream, ofstream,
  fstream classes */
#include <fstream> 

#include <string>
using namespace std;

int AYFILE_ClearIpList(void) {
	// Creation of ofstream class object 
	ofstream fout;

	string line;

	// by default ios::out mode, automatically deletes 
	// the content of file. To append the content, open in ios:app 
	// fout.open("sample.txt", ios::app) 
	fout.open(".\file\AddIP.bat");
	// Execute a loop If file successfully opened 
	while (fout) {
		// Read a Line from standard input 
		getline(cin, line);

		// Press -1 to exit 
		if (line == "-1")
			break;

		// Write line in file 
		fout << line << endl;
	}

	// Close the File 
	fout.close();

	// Creation of ifstream class object to read the file 
	ifstream fin;

	// by default open mode = ios::in mode 
	fin.open(".\file\AddIP.bat");

	// Execute a loop until EOF (End of File) 
	while (fin) {

		// Read a Line from File 
		getline(fin, line);

		// Print line in Console 
		cout << line << endl;
	}

	// Close the file 
	fin.close();

	return 1;
}
