import numpy as np 

class WordAccumulator:
    def __init__(self, cols, bit_depth):
        self.cols = cols
        self.bit_depth = bit_depth
        self.current_sums = np.zeros(cols, dtype=float)

    def reset(self):
        self.current_sums = np.zeros(self.cols, dtype=float)
        
    
    def process_masked_matrix(self,masked_matrix):
        """
        Process a masked matrix to compute combined values from column sums.
        """
        # Sum each column
        column_sums = np.sum(masked_matrix, axis=0)
        
        # Initialize the list to store combined values
        combined_values = []
        
        # Process every 8 columns as a binary number
        num_groups = len(column_sums) // self.bit_depth
        for group in range(num_groups):
            binary_sum = 0
            # Iterate over each bit in the group
            for bit_position in range(self.bit_depth):
                # Calculate the value for this bit
                bit_value = column_sums[group * self.bit_depth + bit_position] * (2 ** (self.bit_depth-1 - bit_position))
                # Add to the binary sum
                binary_sum += bit_value
            # Append the combined value to the list
            combined_values.append(binary_sum)
            #self.combined_values.append(binary_sum)
        #self.combined_values_grouped.append(combined_values)
        return combined_values