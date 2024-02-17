
/******************************************************************************/
/*                                D A T A _ H                                 */
/******************************************************************************/



#ifndef DATA_H
#define DATA_H



#include "libml.h"



/******************************************************************************/
/*                                    Data                                    */
/******************************************************************************/



table* csv_to_table(const char* csv_filename, int class_label_column_index, int* indices_of_wanted_data_columns_in_csv, int number_of_data_columns_wanted);
void clean_table(table* cleaning_table);
void display_table(table* displaying_table);
table* copy_table_subset(table* source_table, int* row_slice_indices, int row_slice_indices_len);
void delete_classes_from_table(table* target_table);



/******************************************************************************/
/*                                  Helpers                                   */
/******************************************************************************/



table* create_table(int number_of_table_rows, int number_of_table_columns);
int get_num_csv_rows(FILE* csv_pointer);
int get_num_csv_columns(FILE* csv_pointer);
int populate_table_from_csv(FILE* csv_pointer, table* populating_table, int class_label_column_index, int* indices_of_wanted_data_columns_in_csv, int number_of_data_columns_wanted, int number_of_csv_columns);
int get_index_of_int_in_array(int element, int* array, int array_length);
void set_current_csv_value(FILE* csv_pointer, char* csv_value_destination);
int get_total_from_slice_indices(int* slice_indices, int slice_indices_len);



#endif
