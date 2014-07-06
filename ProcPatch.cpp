// ============================================================================
// ProcPatch
// ============================================================================

#include <conio.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;


typedef unsigned __int8		byte;
typedef unsigned __int16	word;
typedef unsigned __int32	dword;
typedef unsigned __int64	qword;


namespace Proc
{
	const dword	Offset			= 0x16db2f;	
	const dword	Max				= 6;
	const byte	Expect[Max]		= { 0x00, 0x3E, 0x00, 0x25, 0x00, 0x73 };	
	const byte	Patched[Max]	= { 0x00, 0x25, 0x00, 0x73, 0x00, 0x00 };	
	const byte	Init[Max]		= { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };	
	const char*	Exe				= "UnrealEd.exe";	
}



// ============================================================================
// ProcPatch
// ============================================================================
struct ProcPatch
{
public:
	ProcPatch( int argc, char** argv )
	{
		ExePath = argv[0];
		FileName = Proc::Exe;
		FileOffset = Proc::Offset;
		memcpy( FileBuffer, Proc::Init, Proc::Max );
		FileStream.exceptions( ifstream::eofbit | ifstream::failbit | ifstream::badbit );

		if( argc == 2 )
		{
			sscanf( argv[1], "%lx", &FileOffset );
			//FileOffset = atol(argv[1]);
		}
	}

	~ProcPatch()
	{
		if( FileStream.is_open() )
			FileStream.close();
	}

public:
	void Patch()
	{
		// open file
		FileStream.open( FileName.c_str(), ios::in | ios::out | ios::binary );
		FileStream.seekg( FileOffset, ios_base::beg );

		// feedback
		cout << "Offset: 0x" << hex << FileOffset << endl;

		// load hex
		cout << hex;
		for( int i=0; i<Proc::Max; ++i )
		{
			FileStream >> FileBuffer[i];
			cout << (dword)FileBuffer[i] << " ";
		}
		cout << endl;

		if( memcmp( FileBuffer, Proc::Expect, Proc::Max ) == 0 )
		{
			// unpatched: patch
			FileStream.seekp( FileOffset, ios_base::beg );
			FileStream.write( reinterpret_cast<const char*>(&Proc::Patched[0]), Proc::Max );

			// dump
			cout << hex;
			for( int i=0; i<Proc::Max; ++i )
			{
				cout << (dword)Proc::Patched[i] << " ";
			}
			cout << endl;
		}
		else if( memcmp( FileBuffer, Proc::Patched, Proc::Max ) == 0 )
		{
			// already patched: throw
			throw exception("UnrealEd.exe is already patched.");
		}
		else
		{
			// unknown: throw
			throw exception("Unknown data, can't patch.");
		}
	}

private:
	string		ExePath;
	string		FileName;
	fstream		FileStream;
	byte		FileBuffer[Proc::Max];
	long		FileOffset;
};


// ============================================================================
// pause
// ============================================================================
inline void pause()
{
	cout << endl;
	cout << "Press any key to continue..." << endl;
	getch();
}

// ============================================================================
// main
// ============================================================================
int main( int argc, char** argv )
{
	try
	{
		cout << "===============================================================================" << endl;
		cout << "  ProcPatch:    Fixes UnrealEd v3369 procedural sound playback and properties." << endl;
		cout << "  Usage:        Put in UnrealEd.exe folder and run." << endl;
		cout << "  Expert:       File offset can be specified, ie ProcPatch.exe 16db2f" << endl;
		cout << "" << endl;
		cout << "  Copyright 2005 Roman Dzieciol, neai@o2.pl" << endl;
		cout << "===============================================================================" << endl;

		ProcPatch p( argc, argv );
		p.Patch();
		cout << "Patched." << endl;

		pause();
		return 0;
	}
	catch( ios_base::failure& )		{ cout << "Error - [File Access]" << endl; }
	catch( exception& e )			{ cout << "Error - " << e.what() << endl; }
	catch( ... )					{ cout << "Error - [Unhandled exception]" << endl; }

	pause();
	return 1;
}

// ============================================================================