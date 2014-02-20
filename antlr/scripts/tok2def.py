import re
import sys

if ( len( sys.argv ) != 3 ) :
    raise Exception( "Wrong number of arguments" )

inputFileName = sys.argv[1];
inputFile = open( inputFileName, "r" )

outputFileName = sys.argv[2];
outputFile = open( outputFileName, "w" )

outputFile.write( "#define TOKENS \\\n" )
for line in inputFile :
    outputFile.write( re.sub( r"(.+)=(\d+)", r"T(\1, \2) \\", line ) )
outputFile.write( "\n" )

outputFile.close()
inputFile.close()
