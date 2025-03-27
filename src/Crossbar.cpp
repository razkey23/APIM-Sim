#include "Crossbar.h"
#include <iomanip>
#include <sstream>
#include <fstream>

Crossbar::Crossbar(sc_module_name nm) : sc_module(nm) ,p_done_crossbar(0) {
    
    
    /*for(int i = 0; i < NROW_GROUPS; i++) {
        for(int j = 0; j < ROWS_PER_GROUP; j++) {
            for(int k = 0; k < NCOL_GROUPS; k++) {
                for(int l = 0; l < COLUMNS_PER_GROUP; l++) {
                    // Simple pattern: 1 if (i+j+k+l) is even, else 0
                    if (((i + j + k + l) % 2) == 0) {
                        memory[i][j][k][l] = 1;
                    } else {
                        memory[i][j][k][l] = 1;
                    }
                }
            }
        }
    }*/
    // INIT
    initArray();
    SC_THREAD(execute);
    sensitive<<p_DoA.pos();
    dont_initialize();
}

void Crossbar::initArray() {
    std::ifstream mfile("/home/razkey23/playground/acimsim/tests/functional_sim/out_matrix_bits.txt");
    if(!mfile.is_open()) {
        std::cerr << "[Crossbar] ERROR: Could not open matrix_bits.txt.\n";
        SC_REPORT_FATAL("Crossbar", "Unable to open matrix bits file");
    }
    for (int row=0; row < NROWS;row++){
        int i = row / ROWS_PER_GROUP;
        int j = row % ROWS_PER_GROUP;

        std::string line;
        if(!std::getline(mfile, line)) {
            std::cerr << "[Crossbar] ERROR: Not enough lines in matrix_bits.txt.\n";
             SC_REPORT_FATAL("Crossbar", "Matrix file mismatch");
        }
        std::stringstream ss(line);
        for(int col = 0; col < NCOLS; col++) {
            int bit_val;
            if(!(ss >> bit_val)) {
                std::cerr << "[Crossbar] ERROR: Missing bit at row " << row
                          << ", col " << col << " in matrix_bits.txt.\n";
                SC_REPORT_FATAL("Crossbar", "Matrix file mismatch");
            }
            int k = col / COLUMNS_PER_GROUP;
            int l = col % COLUMNS_PER_GROUP;
            memory[i][j][k][l] = bit_val;
        }
    }
    mfile.close(); 
}

Crossbar::~Crossbar(){
    
}

void Crossbar::execute() {
    while(true) {
        cout<<"Got Into Crossbar Execute"<<endl;
        p_done_crossbar.write(0);
        wait(50, SC_NS);
        int total_rows = NROW_GROUPS * ROWS_PER_GROUP;
        std::vector<int> vector_data(total_rows, 0);
        for (int i = 0; i < NROW_GROUPS; i++) {
            for (int j = 0; j < ROWS_PER_GROUP; j++) {
                bool bit_val = p_RSELRSR_data_in->data[i][j];
                int row_index = i * ROWS_PER_GROUP + j;
                vector_data[row_index] = bit_val ? 1 : 0;
            }
        }

        // 2) Flatten the CSR data -> indicates columns we "activate"
        // We'll use it just like a mask for columns, but let's also store it in a 2D array
        int total_cols = NCOL_GROUPS * COLUMNS_PER_GROUP;
        std::vector<int> col_mask(total_cols, 0);
        for (int k = 0; k < NCOL_GROUPS; k++) {
            for (int l = 0; l < COLUMNS_PER_GROUP; l++) {
                bool bit_val = p_CSELCSR_data_in->data[k][l];
                int col_index = k * COLUMNS_PER_GROUP + l;
                col_mask[col_index] = bit_val ? 1 : 0;
            }
        }

        // 3) Perform MVM: memory * vector_data -> result
        // result is an integer array [total_cols], accumulate sum across all rows
        std::vector<int> result(total_cols, 0);

        for (int i = 0; i < NROW_GROUPS; i++) {
            for (int j = 0; j < ROWS_PER_GROUP; j++) {
                int row_index = i * ROWS_PER_GROUP + j;
                for (int k = 0; k < NCOL_GROUPS; k++) {
                    for (int l = 0; l < COLUMNS_PER_GROUP; l++) {
                        int col_index = k * COLUMNS_PER_GROUP + l;

                        // MVM accumulation:
                        //  matrix[row_index][col_index] * vector[row_index]
                        //  but also factor in col_mask => only sum if col_mask[col_index]==1
                        int mat_val = memory[i][j][k][l];
                        int vec_val = vector_data[row_index];
                        if (col_mask[col_index] == 1) {
                            result[col_index] += (mat_val * vec_val);
                        }
                    }
                }
            }
        }
        for (int i=0; i < total_cols; i++) p_SH_data_out->data[i] = result[i];
        p_done_crossbar.write(1);
        wait();
    }
    
    
    // 4) Print the matrix for debugging
    //print_state();

   
    /*
    std::cout << "[Crossbar] MVM Output (per column) => " << std::endl;
    for (int col_index = 0; col_index < total_cols; col_index++) {
        std::cout << "result[" << col_index << "]=" << result[col_index] << "  ";
    }
    std::cout << std::endl << "[Crossbar] End of Execution\n" << std::endl;
    */
}


void Crossbar::print_state() {
    std::cout << "\n[Crossbar] Current Matrix State (MVM Matrix):\n";
    int total_rows = NROW_GROUPS * ROWS_PER_GROUP;
    int total_cols = NCOL_GROUPS * COLUMNS_PER_GROUP;

    // Print a header row index
    std::cout << "      ";
    for (int c = 0; c < total_cols; c++) {
        std::cout << std::setw(2) << c << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < NROW_GROUPS; i++) {
        for (int j = 0; j < ROWS_PER_GROUP; j++) {
            int row_index = i * ROWS_PER_GROUP + j;

            std::cout << "Row " << std::setw(2) << row_index << ": ";
            for (int k = 0; k < NCOL_GROUPS; k++) {
                for (int l = 0; l < COLUMNS_PER_GROUP; l++) {
                    int col_index = k * COLUMNS_PER_GROUP + l;
                    std::cout << memory[i][j][k][l] << "  ";
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << "[Crossbar] End Matrix State\n" << std::endl;
}