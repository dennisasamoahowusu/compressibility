import math
from optparse import OptionParser
import os
import gzip
import lz4.frame
import lzma


class EceTools:

    @staticmethod
    def ecetool(ece_type, source_dir, output_filename):
        files = [os.path.join(source_dir, f) for f in sorted(os.listdir(source_dir))
                 if os.path.isfile(os.path.join(source_dir, f))]
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
                elif ece_type == "gz1":
                    ece_output = EceTools.gzip(data, 1)
                elif ece_type == "gz6":
                    ece_output = EceTools.gzip(data, 6)
                elif ece_type == "lz4":
                    ece_output = EceTools.lz4(data)
                elif ece_type == "xz":
                    ece_output = EceTools.xz(data)
                else:
                    raise ValueError("bad ece_type")
                ece_output_str = "{0:.20f}".format(ece_output)
                output_file.write("%s %s \n" % (f, ece_output_str))
                #output_file.write("%s\n" % ece_output_str)
        output_file.close()

    @staticmethod
    def xz(_input):
        # xz is the default format
        data_out = lzma.compress(_input)
        return len(data_out)

    # return size in bytes
    @staticmethod
    def lz4(_input):
        compressed = lz4.frame.compress(_input)
        return len(compressed)

    # returns size in bytes
    @staticmethod
    def gzip(_input: bytearray, level):
        filename = 'temp-gz-output.gz'
        output = gzip.open(filename, 'wb', compresslevel=level)
        try:
            output.write(_input)
        finally:
            output.close()
        size = os.stat(filename).st_size
        return size

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

        result = factor * _sum

        return math.sqrt(result)


if __name__ == "__main__":
    usage = """
    This script produces a file with ece-outputs for files in source-dir

    Usage: python ecetools.py <ece-type><source-dir> <output-dir><output-filename>

        <ece-type> ece type: bc (for bytecounting), bs (for byte stdev) , gz1, gz6, lz4, xz
        <source-dir> source dir
        <output-filename> output filename
        """
    parser = OptionParser(usage=usage)
    (options, args) = parser.parse_args()
    if len(args) != 3:
        print(args)
        print("Invalid number of args")
        print("Usage: python ecetools.py <ece-type><source-dir><output-filename> ")
        exit(1)
    EceTools.ecetool(args[0], args[1], args[2])

