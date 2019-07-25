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
	string filename = "file/AddIP.bat";

	// Write to File
	ofstream fout(filename.c_str(), ios::out | ios::binary);
	if (!fout.is_open()) {
		printf("error: open file for output failed!");
		abort();
	}
	fout.write((char *)"", 0);

	fout.close();
}

int AYFILE_AddIPsToFile(Ui32 *pIP, Ui16 cnt) {
	//==============================================================================
	int i=0;
	char line[128];
	string filename = "file/AddIP.bat";
	string cd = "cd..";
	char netsh[] = "netsh interface ip add address \"Local Area Connection\" ";
	// Read/Write from file
	fstream fio(filename.c_str(), ios::out | ios::in | ios::binary);
	while (fio.read(&line[0],1)) {
		i++;
	};
	//==============================================================================
	if (i < 2) {
		fio.write((char *)&cd, sizeof(cd));
		fio.write((char *)&cd, sizeof(cd));
		fio.write((char *)&cd, sizeof(cd));
		fio.write((char *)&cd, sizeof(cd));
		fio.write((char *)&cd, sizeof(cd));
	}
	
	while (cnt) {
		strcpy(line, netsh);
		AY_ConvertIPAddToStrRet((Ui08 *)*pIP, line);
		AY_ConvertIPAddToStrRet((Ui08 *)*pIP, line);
		AY_ConvertIPAddToStrRet((Ui08 *)*pIP, line);
		fio.write((char *)&line, sizeof(line));
		cnt--;
	}

	fio.close();
	//netsh interface ip add address "Local Area Connection" 192.168.2.213 255.255.255.0 192.168.2.1


	return 1;
}



int AYFILE_ClearIpListb(void) {
	string filename = ".\file\AddIP.bat";
	

	// Write to File
	ofstream fout(filename.c_str(), ios::out | ios::binary);
	if (!fout.is_open()) {
		cerr << "error: open file for output failed!" << endl;
		abort();
	}
	int i = 1234;
	double d = 12.34;
	fout.write((char *)&i, sizeof(int));
	fout.write((char *)&i, sizeof(int));
	fout.write((char *)&d, sizeof(double));
	fout.close();

	// Read from file
	ifstream fin(filename.c_str(), ios::in | ios::binary);
	if (!fin.is_open()) {
		cerr << "error: open file for input failed!" << endl;
		abort();
	}
	int i_in;
	double d_in;
	fin.read((char *)&i_in, sizeof(int));
	cout << i_in << endl;
	fin.read((char *)&d_in, sizeof(double));
	cout << d_in << endl;
	fin.close();
	
	
	
	
	//// Creation of ofstream class object 
	//ofstream fout;

	//string line;
	//const char * c;

	//// by default ios::out mode, automatically deletes 
	//// the content of file. To append the content, open in ios:app 
	//// fout.open("sample.txt", ios::app) 
	//fout.open("AddIP.txt", ios::trunc | ios::out | ios::in); //fout.open(".\file\AddIP.bat");
	//// Execute a loop If file successfully opened 
	//while (fout) {
	//	// Read a Line from standard input 
	//	getline(cin, line);

	//	// Press -1 to exit 
	//	if (line == "-1")
	//		break;

	//	// Write line in file 
	//	char * writable = new char[line.size() + 1];
	//	std::copy(line.begin(), line.end(), writable);
	//	writable[line.size()] = '\0'; // don't forget the terminating 0


	//	//c = line.c_str();
	//	printf("LINE: %s", writable);
	//	fout.write(writable,strlen(writable));
	//	//fout << line << endl;
	//}

	//// Close the File 
	//fout.close();

	//// Creation of ifstream class object to read the file 
	//ifstream fin;

	//// by default open mode = ios::in mode 
	//fin.open("AddIP.txt"); //fin.open(".\file\AddIP.bat");

	//// Execute a loop until EOF (End of File) 
	//while (fin) {

	//	// Read a Line from File 
	//	getline(fin, line);

	//	// Print line in Console 
	//	cout << line << endl;
	//}

	//// Close the file 
	//fin.close();

	return 1;
}
