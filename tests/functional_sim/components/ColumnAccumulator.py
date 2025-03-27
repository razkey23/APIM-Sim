import numpy as np 

class ColumnAccumulator:
    def __init__(self, cols,bit_width):
        self.bit_width = bit_width
        self.cols = cols
    
    def reset(self):
        pass

    def accumulate(self, bit_multiplications):
        return np.sum(bit_multiplications, axis=0)