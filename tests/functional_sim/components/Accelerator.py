import numpy as np
from components.ColumnAccumulator import ColumnAccumulator
from components.WordAccumulator import WordAccumulator
from components.OperationalArray import OperationalArray
from components.RowAccumulator import RowAccumulator

class Accelerator:
    def __init__(self, rows, cols, bit_depth,bit_serial):
        self.array = OperationalArray(rows, cols, bit_depth)
        self.column_accumulator = ColumnAccumulator(cols,bit_depth)
        self.word_accumulator = WordAccumulator(cols, bit_depth)
        self.row_accumulator = RowAccumulator(bit_depth)
        self.bit_depth = bit_depth
        self.bit_serial = bit_serial



    
    def load_weights(self, weights):
        self.array.load_weights(weights)

    def reset_accumulators(self):
        """
        Reset accumulators between different multiplications if needed.
        """
        self.column_accumulator.reset()
        self.word_accumulator.reset()
        self.row_accumulator.reset()

    def vector_matrix_multiply_bitserial(self, input_vector):
        """
        Perform a bit-serial multiplication of input_vector (length R) with
        the loaded matrix (shape R x ? in 8-bit sense).
        
        Returns final row accumulator output (numpy array).
        """
        # We assume 'input_vector' is 8-bit values. We'll extract each bit in [0..bit_depth-1].
        # The matrix is loaded as shape (rows, cols*bit_depth).
        # The final output size is 'cols' if each group of 8 columns = 1 output dimension.

        # Reset accumulators
        self.reset_accumulators()

        # For each bit_index from 0..bit_depth-1, build an array of shape (rows,) that has
        # the bit_index-th bit of input_vector in the first len(input_vector) rows (the rest 0).
        total_rows = self.array.rows
        for bit_index in range(self.bit_depth):
            # Build the input bit for each row
            input_bits = np.zeros(total_rows, dtype=np.uint8)
            for i, val in enumerate(input_vector):
                # Extract the bit_index-th bit
                # (LSB is bit_index = 0, so shift right & mask)
                bit_val = (val >> bit_index) & 1
                input_bits[i] = bit_val

            # Multiply on the array (bitwise mask)
            masked_matrix = self.array.multiply(input_bits)
            # Combine columns into word-sized integers
            combined_values = self.word_accumulator.process_masked_matrix(masked_matrix)
            # Accumulate the combined values shifted by the bit index
            self.row_accumulator.accumulate_shifted_values(combined_values, bit_index)

        # Final result
        return self.row_accumulator.get_result()

    # CURRENTLY NOT USED !
    def simulate(self, input_vector, cycles):
        states = []
        total_bits = cycles * self.bit_serial
        #current_sums,column_sums,final_outputs=0
        for cycle in range(cycles):
            for serial_bit in range(self.bit_serial):
                bit_index = (cycle * self.bit_serial + serial_bit) % self.bit_depth

                #bit_index = cycle % self.bit_depth
                bit_multiplications = self.array.process_input(input_vector, bit_index) 
                
                column_sums = self.column_accumulator.accumulate(bit_multiplications)
                
                current_sums = self.word_accumulator.accumulate(column_sums)
                print(current_sums)
                final_outputs = self.row_accumulator.accumulate(current_sums, bit_index)
                
                 # Only register the state at the end of the bit_serial process
                if serial_bit == self.bit_serial - 1:
                    state = {
                        'cycle': cycle,
                        'bit_index':bit_index,
                        'input_bits': input_vector,
                        'bit_multiplications': bit_multiplications,
                        'column_sums': column_sums,
                        'current_sums': current_sums,
                        'partial_sums': final_outputs,
                        'final_outputs': [bin(output) for output in final_outputs]
                    }
                    states.append(state)
                    #self.print_state(state)

                # Reset accumulators after processing the final bit
                if bit_index == self.bit_depth-1:
                    #self.column_accumulator.reset()
                    self.word_accumulator.reset()
                    self.row_accumulator.reset()

        return states

    @staticmethod
    def print_state(state):
        print(f"Cycle: {state['cycle']}")
        print("Input Bits and Weights:")
        for i, input_bit in enumerate(state['input_bits']):
            print(f"{input_bit:2} ")
        print("Bit Multiplications:")
        for row in state['bit_multiplications']:
            print(" ".join(map(str, row)))
        print("Column Sums:")
        print(" ".join(map(str, state['column_sums'])))
        print("Current Sums:")
        print(" ".join(map(str, state['current_sums'])))
        print("Partial Sums:")
        print(" ".join(map(str, state['partial_sums'])))
        print("Final Outputs (Binary):")
        for output in state['final_outputs']:
            print(output)
        print("\n")