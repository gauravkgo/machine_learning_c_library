
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
    printf("%d rows and %d columns.\n", datatable -> rows, datatable -> columns);
    printf("Column labels exist : %d\n", ((datatable -> column_labels) != 0));
    printf("Samples exist : %d\n", ((datatable -> sample_vectors) != 0));
    printf("Sample classes exist : %d\n", ((datatable -> class_labels) != 0));
    printf("Last sample : %s: %f, %f\n",
      (datatable -> class_labels)[149],
      ((datatable -> sample_vectors)[149].elements)[0],
      ((datatable -> sample_vectors)[149].elements)[1]);
  }
  else printf("Table was not created.\n");

  printf("End of program.\n");

  return 0;
}

