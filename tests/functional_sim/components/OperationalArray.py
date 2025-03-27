import numpy as np

class OperationalArray:
    def __init__(self, rows, cols, bit_depth):
        self.rows = rows
        self.cols = cols
        self.bit_depth = bit_depth
        self.array = np.zeros((rows, cols*bit_depth), dtype=int)



    def load_weights(self, weights):
        assert weights.shape == self.array.shape, "Weights shape must match array shape"
        self.array = weights

    def _flatten_weights(self):
        # Flatten the weights to (rows, cols * bit_depth)
        flattened_weights = self.array.reshape(self.rows, self.cols * self.bit_depth)
        return flattened_weights

    def multiply(self,input_bits):
        # Create a mask by broadcasting input bits across columns
        mask = input_bits[:, np.newaxis]  # Convert to column vector for broadcasting

        # Apply the mask to the large matrix
        masked_matrix = self.array * mask

        return masked_matrix