
#include <stdio.h>
#include "libml.h"

int main(void)
{
  char dataset[9] = "iris.csv";
  int data_col_indices[2] = {2, 3};
  table* datatable;

  datatable = create_table_from_csv(dataset, 1, data_col_indices, 2);
  if (datatable != 0)
  {
    printf("Table successfully created.\n");
    delete_table(datatable);
    printf("Table deleted.\n");
  }
  else printf("Table was not created.\n");

  printf("End of program.\n");

  return 0;
}

