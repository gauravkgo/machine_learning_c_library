
#include <stdio.h>
#include "libml.h"

int main(void)
{
  classifier* knn_classifier = 0;
  classifier_type knn_type = KNN;

  knn_classifier = create_classifier(knn_type);
  (knn_classifier -> info)(knn_classifier);

  printf("End of program.\n");

  return 0;
}

