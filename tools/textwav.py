# -*- coding: utf-8 -*-

import sys
import numpy as np
import scipy.io.wavfile

def main():
    try:
        if len(sys.argv) != 5:
            raise ValueError("Invalid arguement count");
            
        if sys.argv[1] == "towave":
            toWave(sys.argv[2], sys.argv[3], float(sys.argv[4]))
        elif sys.argv[1] == "totextwave":
            toTextWave(sys.argv[2], sys.argv[3], float(sys.argv[4]))
        else:
            raise ValueError("Invalid first argument");
    except Exception as ex:
        printUsage()
        print(ex)
        
def toWave(inputFilePath, outputFilePath, gain):
    with open(inputFilePath, "r") as inputFile:
        lines = [line.rstrip('\n') for line in inputFile]
        size = int(lines[0]);
        Fs = int(lines[1]);
        
        data = np.zeros((size,), dtype=np.int16);
        
        for i in range(size):
            data[i] = int(float(lines[i + 2]) * gain)
            
        scipy.io.wavfile.write(outputFilePath, Fs, data);
        
    
def toTextWave(inputFilePath, outputFilePath, gain):
    Fs, data = scipy.io.wavfile.read(inputFilePath)
        
    if data.shape != (data.size,):
        raise ValueError("Many channel wave are not supported")

    data = data * gain;
    
    with open(outputFilePath, "w") as outputFile:
        outputFile.write(str(data.size) + "\n")
        outputFile.write(str(Fs) + "\n")
        for i in range(data.size):
            outputFile.write(str(data[i]) + "\n")
            
def printUsage():
    print("Convert a wave file to a text wave file:")
    print("\tpython textwav.py totextwave input_file_path output_file_path")
    print("Convert a text wave file to a wave file:")
    print("\tpython textwav.py towave input_file_path output_file_path\n\n")
    
if __name__ == "__main__":
    main()