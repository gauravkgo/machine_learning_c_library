
#include <stdio.h>
#include "libml.h"

int main(void)
{
  char dataset[9] = "iris.csv";
  int data_col_indices[4] = {2, 3, 4, 5};
  int k = 10;
  table* datatable;
  table* train_table;
  table* predictions_table;
  int train_slices[20] = {
    0, 14, 15, 29, 30, 44, 45, 59, 60, 74,
    75, 89, 90, 104, 105, 119, 120, 134, 135, 149
  };
  int test_slices[20] = {
    14, 15, 29, 30, 44, 45, 59, 60, 74, 75,
    89, 90, 104, 105, 119, 120, 134, 135, 149, 150
  };

  classifier* knn_classifier = create_classifier(KNN);
  (knn_classifier -> initialise)(knn_classifier, &k);
  datatable = create_table_from_csv(dataset, 1, data_col_indices, 4);

  if ((datatable != 0) && (knn_classifier != 0))
  {
    train_table = create_subset_table(datatable, train_slices, 20);
    predictions_table = create_subset_table(datatable, test_slices, 20);
    strip_table_classes(predictions_table);
    
    (knn_classifier -> train)(knn_classifier, train_table);
    (knn_classifier -> classify)(knn_classifier, predictions_table);

    (void) printf("Knn delete exit code: %d\n", delete_classifier(knn_classifier));
    delete_table(datatable);
    delete_table(train_table);
    delete_table(predictions_table);
    
  }

  printf("End of program.\n");

  return 0;
}

