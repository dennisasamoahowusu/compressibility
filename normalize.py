import numpy as np
from sklearn.preprocessing import MinMaxScaler
from optparse import OptionParser


def normalize(input_file, output_file):
    with open(input_file) as f:
        data = f.read().splitlines()
        data.sort()

        data_values = []
        for i in data:
            data_values.append(float(i.split()[1]))

        data_values = np.asarray(data_values)
        data_values = data_values.reshape(-1, 1)

        scaler = MinMaxScaler()
        scaler.fit(data_values)
        data_values = scaler.transform(data_values)
        output_file = open(output_file, "a+")
        for i in data_values:
            output_file.write("%f\n" % i)
        output_file.close()


if __name__ == "__main__":
    usage = """
    This script produces normalizes numbers in input_file to [0,1] and writes in output_file

    Usage: python normalize.py <input_filename><output_filename>

        """
    parser = OptionParser(usage=usage)
    (options, args) = parser.parse_args()
    if len(args) != 2:
        print(args)
        print("Invalid number of args")
        print("Usage: python normalize.py <input_filename><output_filename> ")
        exit(1)
    normalize(args[0], args[1])
