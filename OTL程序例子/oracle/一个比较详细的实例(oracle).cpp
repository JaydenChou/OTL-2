#define	OTL_ORA10G		// Compile OTL 4/OCI10G
#include "otlv4.h"		// include the OTL 4 header file
#include <iostream>
using namespace std;
#pragma comment(lib, "oci.lib")

otl_connect db;			// connect object

void insert()			// insert rows into table
{ 
	otl_stream wr
		(50,	// buffer size
		"insert into test_tab values(:f1<float>,:f2<char[31]>)", // SQL statement
		db		// connect object
		);

	char tmp[32];

	for (int i=1; i<=20; ++i)
	{
		sprintf(tmp, "Name%d", i);
		wr << (float)i << tmp;
	}
}

void select()
{ 
	otl_stream rd
		(50,	 // buffer size
		"select * from test_tab where f1>=:f<int> and f1<=:f*2",	// SELECT statement
		db		 // connect object
		); 

	float f1;
	char f2[31];

	rd << 8;     // assigning :f = 8

	while (!rd.eof())	// while not end-of-data
	{ 
		rd >> f1 >> f2;
		cout << "f1 = " << f1 
			 << ", f2 = " << f2 << endl;
	}

	rd << 4; // assigning :f = 4
	while (!rd.eof())	// while not end-of-data
	{ 
		rd >> f1 >> f2;
		cout << "f1 = " << f1 
			<< ", f2 = " << f2 << endl;
	}
}

int main()
{
	otl_connect::otl_initialize();		// initialize OTL environment

	try
	{
		db.rlogon("aptscs/cs@apts224"); // connect to Oracle

		otl_cursor::direct_exec
			(db,
			"drop table test_tab",
			otl_exception::disabled			// disable OTL exceptions
			); // drop table

		otl_cursor::direct_exec
			(
			db,
			"create table test_tab(f1 number, f2 varchar2(30))"
			);  // create table

		insert(); // insert records into table
		select(); // select records from table

	}
	catch (otl_exception& e)	    // intercept OTL exceptions
	{
		cerr << "ERROR: code[" <<e.code<<"]\n";
		cerr << e.msg << endl;		// print out error message
		cerr << e.stm_text << endl; // print out SQL that caused the error
		cerr << e.var_info << endl; // print out the variable that caused the error
	}
	db.logoff();				    // disconnect from Oracle

	return 0;
}
