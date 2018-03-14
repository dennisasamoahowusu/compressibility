import math
from optparse import OptionParser
import os
import shutil


class EceTools:

    @staticmethod
    def ecetool(ece_type, source_dir):
        files = [os.path.join(source_dir, f) for f in os.listdir(source_dir)
                 if os.path.isfile(os.path.join(source_dir, f))]
        output_filename = ece_type + "_ece_output"
        if os.path.exists(output_filename):
            os.remove(output_filename)
        output_file = open(output_filename, "a+")
        for f in files:
            with open(f, "rb") as binary_file:
                data = bytearray(binary_file.read())
                if ece_type == "bc":
                    ece_output = EceTools.bytecounting(data)
                elif ece_type == "bs":
                    ece_output = EceTools.byte_stdev(data)
                else:
                    raise ValueError("bad ece_type")
                output_file.write("%s %d \n" % (f, ece_output))
        output_file.close()

    @staticmethod
    def bytecounting(_input: bytearray):
        appearances = [0] * 256
        bytecount = 0
        # if len(_input) < 256:
        #     raise ValueError("Input length too small for byte counting")
        threshold = len(_input)/256
        for byte in _input:
            slotnum = int(byte)
            if appearances[slotnum] < threshold:
                appearances[slotnum] += 1
                if appearances[slotnum] >= threshold:
                    bytecount += 1
        return bytecount

    @staticmethod
    def byte_stdev(_input: bytearray):
        n = len(_input)
        factor = 1.0/(n-1)

        appearances = [0] * 256
        for byte in _input:
            slotnum = int(byte)
            appearances[slotnum] += 1

        # has only frequency of bytes that occur in data
        occurrences = []
        total_frequency = 0
        for i in appearances:
            if i != 0:
                occurrences.append(i)
                total_frequency += i

        _sum = 0
        for i in occurrences:
            x = i/total_frequency
            _sum += (x - 1/n)**2

        return math.sqrt(factor * _sum)


if __name__ == "__main__":
    usage = """
    This script produces a file with ece-outputs for files in source-dir

    Usage: python ecetools.py <ece-type><source-dir> <output-dir> 

        <ece-type> ece type: bc (for bytecounting), bs (for byte stdev) 
        <source-dir> source dir
        """
    parser = OptionParser(usage=usage)
    (options, args) = parser.parse_args()
    if len(args) != 2:
        print(args)
        print("Invalid number of args")
        print("Usage: python ecetools.py <ece-type><source-dir> ")
        exit(1)
    EceTools.ecetool(args[0], args[1])

