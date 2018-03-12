import math

class EceTools:
    def __init__(self):
        pass

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


