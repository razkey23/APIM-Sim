#ifndef CONFIG_H
#define CONFIG_H

#define INSTRUCTION_FILE "/home/razkey23/playground/acimsim/src/instructions.txt"

#define NROWS 128
#define NCOLS 128
#define NROW_GROUPS 16 // Number of row groups so we have 16 rows per group
#define NCOL_GROUPS 16 // Number of column groups so we have 16 columns per group
#define MEMCELL_LEVELS 2 // Precision of MemCell in bits
#define MAX_ROW_ACTIVATION 16 // Maximum number of rows that can be activated at a time, may be not used in first simulation
#define ADC_COLUMN_SHARING 4  // 4 columns share an ADC
#define INPUT_PRECISION 4  //  Precision of Input
#define WEIGHT_PRECISION 8 // Precision of Weights
#define ADC_SHARING_FACTOR 4 // 4 Columns share an ADC
#define ADC_GROUPS NCOLS/ADC_SHARING_FACTOR
#define ROWS_PER_GROUP 8
#define COLUMNS_PER_GROUP 8


#endif