
/******************************************************************************/
/*                                D A T A _ C                                 */
/******************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"




/******************************************************************************/
/*                                    Data                                    */
/******************************************************************************/



table* csv_to_table(const char* csv_filename, int class_label_column_index, int* indices_of_wanted_data_columns_in_csv, int number_of_data_columns_wanted)
{
  FILE* csv_pointer = 0;
  table* new_table = 0;
  int number_of_csv_rows, number_of_csv_columns;
  int exit_code;

  csv_pointer = fopen(csv_filename, "r");
  if (csv_pointer == 0) {(void) printf("Error: Failed to open csv file.\n"); goto fail_create_table_from_csv;}

  number_of_csv_rows = get_num_csv_rows(csv_pointer);
  number_of_csv_columns = get_num_csv_columns(csv_pointer);

  new_table = create_table(number_of_csv_rows, number_of_data_columns_wanted);
  if (new_table == 0) {(void) printf("Error: Failed to allocate memory for table.\n"); goto fail_create_table_from_csv;}

  exit_code = (populate_table_from_csv(csv_pointer, new_table, class_label_column_index, indices_of_wanted_data_columns_in_csv, number_of_data_columns_wanted, number_of_csv_columns));
  if (exit_code != NO_ERROR) {(void) printf("Error: Failed to populate table from csv.\n"); goto fail_create_table_from_csv;}

  return new_table;

fail_create_table_from_csv:

  if (csv_pointer != 0) (void) fclose(csv_pointer);
  if (new_table != 0) clean_table(new_table);

  return 0;
}

void clean_table(table* cleaning_table)
{
  if (cleaning_table == 0) return;
  if (cleaning_table -> column_labels != 0) delete_labels(cleaning_table -> column_labels, cleaning_table -> columns);
  if (cleaning_table -> sample_vectors != 0) delete_vector_array(cleaning_table -> sample_vectors, cleaning_table -> rows);
  if (cleaning_table -> class_labels != 0) delete_labels(cleaning_table -> class_labels, cleaning_table -> rows);
  (void) memset(cleaning_table, 0, sizeof(table));
  free(cleaning_table);
}

void display_table(table* displaying_table)
{
  int i, j;

  if (displaying_table == 0) {(void) printf("Error: Table not initialised.\n"); return;}

  (void) printf("Table at %p:\n", (void*) displaying_table);
  (void) printf("Class");
  for (i = 0; i < displaying_table -> columns; i ++) (void) printf("\t%s", (displaying_table -> column_labels)[i]);
  (void) printf("\n");

  for (i = 0; i < displaying_table -> rows; i ++)
  {
    (void) printf("%s", (displaying_table -> class_labels)[i]);
    for (j = 0; j < displaying_table -> columns; j ++) (void) printf("\t%f", (displaying_table -> sample_vectors)[i].elements[j]);
    (void) printf("\n");
  }
  return;
}

table* copy_table_subset(table* source_table, int* row_slice_indices, int row_slice_indices_len)
{
  int i, source_table_row_index, subset_table_row_index = 0;
  table* subset_table = 0;
  int num_subset_table_rows;

  num_subset_table_rows = get_total_from_slice_indices(row_slice_indices, row_slice_indices_len);
  subset_table = create_table(num_subset_table_rows, source_table -> columns);
  if (subset_table == 0) return 0;

  for (i = 0; i < source_table -> columns; i ++) copy_label((subset_table -> column_labels)[i], (source_table -> column_labels)[i]);

  for (i = 0; i < row_slice_indices_len; i += 2)
  {
    for (source_table_row_index = row_slice_indices[i]; source_table_row_index < row_slice_indices[i + 1]; source_table_row_index ++)
    {
      copy_label((subset_table -> class_labels)[subset_table_row_index], (source_table -> class_labels)[source_table_row_index]);
      copy_vector(&((subset_table -> sample_vectors)[subset_table_row_index]), &((source_table -> sample_vectors)[source_table_row_index]));
      subset_table_row_index ++;
    }
  }

  return subset_table;
}

void delete_classes_from_table(table* target_table)
{
  int i;
  for (i = 0; i < target_table -> rows; i ++) (void) memset((target_table -> class_labels)[i], '\0', sizeof(char) * MAX_LABEL_LENGTH);
  return;
}



/******************************************************************************/
/*                                  Helpers                                   */
/******************************************************************************/



table* create_table(int number_of_table_rows, int number_of_table_columns)
{
  table* new_table;
  char** new_column_labels;
  vector* new_sample_vectors;
  char** new_class_labels;

  new_table = (table*) calloc(1, sizeof(table));
  if (new_table == 0) {return 0;}

  new_table -> rows = number_of_table_rows;
  new_table -> columns = number_of_table_columns;

  new_column_labels = create_labels(number_of_table_columns);
  if (new_column_labels == 0) {clean_table(new_table); return 0;}
  new_table -> column_labels = new_column_labels;

  new_sample_vectors = create_vector_array(number_of_table_columns, number_of_table_rows);
  if (new_sample_vectors == 0) {clean_table(new_table); return 0;}
  new_table -> sample_vectors = new_sample_vectors;

  new_class_labels = create_labels(number_of_table_rows);
  if (new_class_labels == 0) {clean_table(new_table); return 0;}
  new_table -> class_labels = new_class_labels;

  return new_table;
}

int get_num_csv_rows(FILE* csv_pointer)
{
  int num_csv_rows = 0;
  fseek(csv_pointer, 0, SEEK_SET);
  while (!feof(csv_pointer)) if (fgetc(csv_pointer) == '\n') num_csv_rows ++;
  num_csv_rows --;
  return num_csv_rows;
}

int get_num_csv_columns(FILE* csv_pointer)
{
  int num_csv_columns = 0;
  char c;
  fseek(csv_pointer, 0, SEEK_SET);
  while ((c = fgetc(csv_pointer)) != '\n') num_csv_columns += (c == ',');
  if (c == '\n') num_csv_columns ++;
  return num_csv_columns;
}

int populate_table_from_csv(FILE* csv_pointer, table* populating_table, int class_label_column_index, int* indices_of_wanted_data_columns_in_csv, int number_of_data_columns_wanted, int number_of_csv_columns)
{
  int current_row, current_column;
  int number_of_rows;
  int index_of_current_column_in_wanted_data_columns_array;
  char current_csv_value[MAX_LABEL_LENGTH];

  fseek(csv_pointer, 0, SEEK_SET);
  number_of_rows = populating_table -> rows;
  for (current_column = 0; current_column < number_of_csv_columns; current_column ++)
  {
    set_current_csv_value(csv_pointer, current_csv_value);
    index_of_current_column_in_wanted_data_columns_array = get_index_of_int_in_array(current_column, indices_of_wanted_data_columns_in_csv, number_of_data_columns_wanted);
    if (index_of_current_column_in_wanted_data_columns_array >= 0) copy_label((populating_table -> column_labels)[index_of_current_column_in_wanted_data_columns_array], current_csv_value);
  }

  for (current_row = 0; current_row < number_of_rows; current_row ++)
  {
    for (current_column = 0; current_column < number_of_csv_columns; current_column ++)
    {
      set_current_csv_value(csv_pointer, current_csv_value);
      if (current_column == class_label_column_index) copy_label((populating_table -> class_labels)[current_row], current_csv_value);
      else
      {
        index_of_current_column_in_wanted_data_columns_array = get_index_of_int_in_array(current_column, indices_of_wanted_data_columns_in_csv, number_of_data_columns_wanted);
        if (index_of_current_column_in_wanted_data_columns_array >= 0) ((populating_table -> sample_vectors)[current_row].elements)[index_of_current_column_in_wanted_data_columns_array] = atof(current_csv_value);
      }
    }
  }

  return NO_ERROR;
}

int get_index_of_int_in_array(int element, int* array, int array_length)
{
  int i;
  for (i = 0; i < array_length; i ++) if (array[i] == element) return i;
  return -1;
}

void set_current_csv_value(FILE* csv_pointer, char* csv_value_destination)
{
  int i = 0;
  char c = fgetc(csv_pointer);
  while ((c != ',') && (c != '\n') && (i < MAX_LABEL_LENGTH - 1))
  {
    csv_value_destination[i ++] = c;
    c = fgetc(csv_pointer);
  }
  csv_value_destination[i] = '\0';
  return;
}

int get_total_from_slice_indices(int* slice_indices, int slice_indices_len)
{
  int i, total = 0;
  for (i = 0; i < slice_indices_len; i += 2) total += slice_indices[i + 1] - slice_indices[i];
  return total;
}


