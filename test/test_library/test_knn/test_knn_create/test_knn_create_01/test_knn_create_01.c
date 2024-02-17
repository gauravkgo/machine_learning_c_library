
#include <stdio.h>
#include "libml.h"

int main(void)
{
  classifier* knn_classifier = 0;
  classifier_type knn_type = KNN;

  knn_classifier = create_classifier(knn_type);
  if (knn_classifier != 0)
    printf("Classifier type is %d\n", (int) (knn_classifier -> type_of_classifier));
  else
    printf("Knn classifier is not created.\n");

  return 0;
}

