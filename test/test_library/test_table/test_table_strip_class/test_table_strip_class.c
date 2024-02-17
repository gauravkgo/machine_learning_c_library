
#include <stdio.h>
#include "libml.h"

int main(void)
{
  char dataset[9] = "iris.csv";
  int data_col_indices[4] = {2, 3, 4, 5};
  table* datatable;
  table* subset_table;
  int row_slice_indices[20] = {
    14, 15, 29, 30, 44, 45, 59, 60, 74, 75,
    89, 90, 104, 105, 119, 120, 134, 135, 149, 150
  };

  datatable = create_table_from_csv(dataset, 1, data_col_indices, 4);
  if (datatable != 0)
  {
    printf("Table successfully created.\n");
    subset_table = create_subset_table(datatable, row_slice_indices, 20);
    strip_table_classes(subset_table);
    print_table(subset_table);
  }
  else printf("Table was not created.\n");

  printf("End of program.\n");

  return 0;
}

