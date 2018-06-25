#include "tester.h"
ofstream Pyout;
void CPy_Datalog_Merge(CString path)
{
	Pyout.open(path);
	Pyout	<< "import sys,os, re, time"					<<endl 
			<< "try:"										<<endl
			<< "\tif len(sys.argv) < 3:"					<<endl
			<< "\t\traise Exception(\"Too few file imported\")"<<endl
			<< "\toutput_file = sys.argv[1]"				<<endl
			<< "\tinput_files = sys.argv[2:]"				<<endl
			<< "\tfor i in range(len(input_files)):"		<<endl
			<< "\t\tf = open(input_files[i])"				<<endl
            << "\t\tcontent = f.read()				    "	<<endl
            << "\t\tf.close()						    "	<<endl
            << "\t\tbuffer = ''.join(content)		    "	<<endl
            << "\t\tcontent = buffer.split('\\n')       "	<<endl
            << "\t\tif i == 0:						    "	<<endl
			<< "\t\t\tif os.path.exists(output_file) :  "	<<endl
            << "\t\t\t\tos.remove(output_file)		    "	<<endl
            << "\t\telse:							    "	<<endl
            << "\t\t\tcontent = content[2:]			    "	<<endl
            << "\t\tbuffer = '\\n'.join(content)	    "	<<endl
            << "\t\tfile = open(output_file,'a')	    "	<<endl
            << "\t\tfile.write(buffer)				    "	<<endl
            << "\t\tf.close()						    "	<<endl
			<< "\tfor i in range(len(input_files)) :    "	<<endl
			<< "\t\tif os.path.exists(input_files[i]) :	"	<<endl
			<< "\t\t\tos.remove(input_files[i])			"	<<endl
			<< "except Exception as err :				"	<<endl
			<< "\tprint('Have a Exception:' + str(err))	"	<<endl
			<< endl;
	Pyout.close();
	CreateMyProcess(CString2Char(("attrib +h " + path)));
}