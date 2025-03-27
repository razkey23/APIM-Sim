import numpy as np

class RowAccumulator:
    def __init__(self, bit_depth):
        self.bit_depth = bit_depth
        self.partial_sums = None
        


    def accumulate_shifted_values(self, combined_values, cycle_index):
        """
        Accumulate shifted values for the given cycle and update the result.
        """
        # Initialize result array if not already initialized
        if self.partial_sums is None:
            self.partial_sums = np.zeros(len(combined_values), dtype=np.float64)
        
        # Shift the combined values based on the cycle number
        shifted_values = [value * (2 ** cycle_index) for value in combined_values]
        
        # Add the shifted values to the result array
        self.partial_sums += np.array(shifted_values, dtype=np.int64)

    def get_result(self):
        return self.partial_sums
    
    def reset(self):
        self.partial_sums = None