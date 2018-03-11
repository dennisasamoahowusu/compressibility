
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
        print(appearances)
        return bytecount

    def stdev(self):
        pass

