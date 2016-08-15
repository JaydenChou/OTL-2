#include <iostream>
using namespace std;	
#pragma comment(lib, "oci.lib")
#define OTL_ORA10G
#include "otlv4.h"

otl_connect db; // connect object
		
void insert()	// insert rows into table
{ 
	otl_stream o
	(50, // buffer size
	 "insert into test_tab values(:f1<int>,:f2<char[31]>)", // SQL statement
	  db // connect object
	 );
	//otl_stream o;
	//o.open(50, // buffer size
	// "insert into test_tab values(:f1<int>,:f2<char[31]>)", // SQL statement
	//  db // connect object
	// );

	char tmp[32];
	for (int i=1; i<=100; ++i)
	{
	  sprintf(tmp, "Name%d", i);
	  o << i << tmp;
	}
// o.close();
}
	
void select()
{ 
	otl_stream i(50, // buffer size
	              "select * from test_tab where f1>=:f<int> and f1<=:f*2",
	                 // SELECT statement
	              db // connect object
	             ); 
	   // create select stream
	 
	 float f1;
	 char f2[31];
	
	 i<<4; // assigning :f = 4
	   // SELECT automatically executes when all input variables are
	   // assigned. First portion of output rows is fetched to the buffer
	
	 while(!i.eof()){ // while not end-of-data
	  i >> f1 >> f2;
	  cout << "f1=" << f1 << ", f2=" << f2 << endl;
	 }
	
}
	
int main()
{
	otl_connect::otl_initialize();			// initialize OCI environment
	try
	{
	  db.rlogon("system/hisensoft@APTS107"); // connect to Oracle

	  // direct_exec的两种使用方式
	 //otl_cursor::direct_exec(db, "drop table test_tab", otl_exception::disabled); // drop table
	  db.direct_exec("drop table test_tab", otl_exception::disabled);

	  otl_cursor::direct_exec(db, "create table test_tab(f1 number, f2 varchar2(30))");  // create table
	
	  insert(); // insert records into table
	  select(); // select records from table
	}
	catch(otl_exception& e) // intercept OTL exceptions
	{
		cerr << "ERROR: code[" <<e.code<<"] ";
		cerr << e.msg << endl; // print out error message
		cerr << e.stm_text << endl; // print out SQL that caused the error
		cerr << e.var_info << endl; // print out the variable that caused the error
	 }
	 db.logoff(); // disconnect from Oracle
	
	 return 0;
}
