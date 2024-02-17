
#include <stdio.h>
#include "libml.h"

int main(void)
{
  classifier* knn_classifier = 0;
  classifier_type knn_type = KNN;

  knn_classifier = create_classifier(knn_type);
  if (knn_classifier == 0) printf("Classifier is null.\n");
  if (knn_classifier -> initialise == 0) printf("Init is null.\n");
  if (knn_classifier -> info == 0) printf("Info is null.\n");
  if (knn_classifier -> train == 0) printf("Train is null.\n");
  if (knn_classifier -> classify == 0) printf("Classify is null.\n");
  if (knn_classifier -> clean == 0) printf("Clean is null.\n");

  printf("End of program.\n");

  return 0;
}

