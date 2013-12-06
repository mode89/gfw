import multiprocessing
import os
import subprocess
import sys
import _winreg

MASTER_DIR          = os.getcwd()
MASTER_BUILD_DIR    = MASTER_DIR + "\\build-all"

def main() :
    # Provide path to Ninja

    os.environ["PATH"] += ( ";" + MASTER_DIR + "\\ninja\\bin" )

    # Create the build directory

    if ( not os.path.exists( MASTER_BUILD_DIR ) ) :
        os.mkdir( MASTER_BUILD_DIR )

    try :
        # MinGW build

        try :
            if ( "MINGW_HOME" not in os.environ ) :
                raise Exception( "Error: Failed to find environment variable MINGW_HOME." )

            os.chdir( MASTER_BUILD_DIR )

            if ( not os.path.exists( "mingw" ) ) :
                os.mkdir( "mingw" )
            os.chdir( "mingw" )

            os.environ["PATH"] += ( ";" + os.environ["MINGW_HOME"] + "\\bin" )
            os.environ["CC"]    = "gcc"
            os.environ["CXX"]   = "g++"

            os.system( "cmake -G Ninja -DCMAKE_BUILD_TYPE=Release " + MASTER_DIR )
            os.system( "ninja -j " + str( multiprocessing.cpu_count() ) )
        except Exception as e :
            raise Exception( "Error: Failed to build with MinGW" )

        # Visual Studio build

        try :
            # Find and run vcvarsall.bat

            try :
                key = _winreg.OpenKey( _winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\11.0" )
                shellFolder = _winreg.QueryValueEx( key, "ShellFolder" )[0]
                key.Close()

                vcvarsall = shellFolder + "VC\\vcvarsall.bat"
                if ( not os.path.exists( vcvarsall ) ) :
                    raise Exception( "Error: Not exists vcvarsall.bat" )
            except : raise

            # Build

            os.chdir( MASTER_BUILD_DIR )

            if ( not os.path.exists( "vs" ) ) :
                os.mkdir( "vs" )
            os.chdir( "vs" )

            makeBat = open( "make.bat", "w" )
            print >> makeBat, "@echo off"
            print >> makeBat, "call " + "\"" + vcvarsall + "\""
            print >> makeBat, "set CC=cl"
            print >> makeBat, "set CXX=cl"
            print >> makeBat, "cmake -G Ninja -DCMAKE_BUILD_TYPE=Release " + MASTER_DIR
            print >> makeBat, "ninja -j " + str( multiprocessing.cpu_count() )
            makeBat.close()
            subprocess.check_call( "make.bat" )

        except : raise

    except Exception as e :
        print e

    PressEnter()

def PressEnter() :
    raw_input( "Press ENTER to continue..." )
    return

if __name__ == "__main__" :
    main()
