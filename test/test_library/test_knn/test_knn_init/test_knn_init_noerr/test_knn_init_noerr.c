
#include <stdio.h>
#include "libml.h"

int main(void)
{
  classifier* knn_classifier = 0;
  classifier_type knn_type = KNN;
  int k = 5;

  knn_classifier = create_classifier(knn_type);
  (knn_classifier -> initialise)(knn_classifier, &k);
  if (knn_classifier -> data == 0) printf("Knn data is null.\n");

  printf("End of program.\n");

  return 0;
}

