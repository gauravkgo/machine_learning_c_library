
#include <stdio.h>
#include "libml.h"

int main(void)
{
  char dataset[9] = "iris.csv";
  int data_col_indices[4] = {2, 3, 4, 5};
  table* data;

  classifier* knn_classifier = 0;
  classifier_type knn_type = KNN;
  int k = 5;

  knn_classifier = create_classifier(knn_type);
  (knn_classifier -> initialise)(knn_classifier, &k);

  data = create_table_from_csv(dataset, 1, data_col_indices, 4);
  if (data != 0) (knn_classifier -> train)(knn_classifier, data);

  (knn_classifier -> info)(knn_classifier);

  printf("End of program.\n");

  return 0;
}

