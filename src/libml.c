
/******************************************************************************/
/*                                C O R E _ C                                 */
/******************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "libml.h"
#include "knn.h"
#include "data.h"



/******************************************************************************/
/*                                   Basic                                    */
/******************************************************************************/



vector* create_vector_array(int dimensionality, int array_size)
{
  int i;
  vector* vector_array = 0;

  vector_array = (vector*) calloc(array_size, sizeof(vector));
  if (vector_array == 0) return 0;

  for (i = 0; i < array_size; i ++)
  {
    vector_array[i].dimensionality = dimensionality;
    vector_array[i].elements = (scalar*) calloc(dimensionality, sizeof(scalar));
    if (vector_array[i].elements == 0)
    {
      delete_vector_array(vector_array, array_size);
      return 0;
    }
  }

  return vector_array;

}

void delete_vector_array(vector* vector_array, int array_size)
{
  int i;

  if (vector_array == 0) return;
  
  for (i = 0; i < array_size; i ++)
  {
    if (vector_array[i].elements != 0)
    {
      memset(vector_array[i].elements, 0, sizeof(scalar) * vector_array[i].dimensionality);
      free(vector_array[i].elements);
    }
  }

  memset(vector_array, 0, sizeof(vector) * array_size);
  free(vector_array);

}

void copy_vector(vector* destination_vector, vector* source_vector)
{
  int i, vector_size;
  vector_size = source_vector -> dimensionality;
  for (i = 0; i < vector_size; i ++) (destination_vector -> elements)[i] = (source_vector -> elements)[i];
  return;
}

char** create_labels(int array_size)
{
  int i;
  char** new_labels = 0;

  new_labels = (char**) calloc(array_size, sizeof(char*));
  if (new_labels == 0) return 0;
  for (i = 0; i < array_size; i ++)
  {
    new_labels[i] = (char*) calloc(MAX_LABEL_LENGTH, sizeof(char));
    if (new_labels[i] == 0) {delete_labels(new_labels, array_size); return 0;}
  }
  return new_labels;
}

void delete_labels(char** labels, int array_size)
{
  int i;

  if (labels == 0) return;
  for (i = 0; i < array_size; i ++)
  {
    if (labels[i] != 0)
    {
      (void) memset(labels[i], 0, sizeof(char) * MAX_LABEL_LENGTH);
      free(labels[i]);
    }
  }
  memset(labels, 0, sizeof(char*) * array_size);
  free(labels);
}

void copy_label(char* destination_label, char* source_label)
{
  (void) strncpy(destination_label, source_label, MAX_LABEL_LENGTH);
}

scalar euclidean_distance(vector a, vector b)
{
  return minkowski_distance(a, b, 2);
}

scalar minkowski_distance(vector a, vector b, int p)
{
  int i;
  scalar distance = 0;
  for (i = 0; i < a.dimensionality; i ++) distance += pow((a.elements[i] - b.elements[i]), p);
  distance = pow(distance, 1.0 / p);
  return distance;
}



/******************************************************************************/
/*                                    Data                                    */
/******************************************************************************/



table* create_table_from_csv(const char* filename, int class_column_index, int* data_columns_indices, int number_of_data_columns)
{
  table* samples_table = 0;
  samples_table = csv_to_table(filename, class_column_index, data_columns_indices, number_of_data_columns);
  return samples_table;
}

void delete_table(table* table)
{
  clean_table(table);
  return;
}

void print_table(table* table)
{
  display_table(table);
  return;
}

table* create_subset_table(table* source_table, int* row_slice_indices, int row_slice_indices_len)
{
  table* new_table = 0;
  new_table = copy_table_subset(source_table, row_slice_indices, row_slice_indices_len);
  return new_table;
}

void strip_table_classes(table* stripping_table)
{
  delete_classes_from_table(stripping_table);
  return;
}



/******************************************************************************/
/*                                 Classifier                                 */
/******************************************************************************/



classifier* create_classifier(classifier_type type_of_classifer)
{
  classifier* new_classifier = 0;

  new_classifier = (classifier*) calloc(1, sizeof(classifier));
  if (new_classifier == 0) return 0;
  
  new_classifier -> type_of_classifier = type_of_classifer;
  switch (type_of_classifer)
  {
    case KNN:
      new_classifier -> initialise = &knn_initialise;
      new_classifier -> info = &knn_info;
      new_classifier -> train = &knn_train;
      new_classifier -> classify = &knn_classify;
      new_classifier -> clean = &knn_clean;
      break;
    default:
      (void) memset(new_classifier, 0, sizeof(classifier));
      free(new_classifier);
      new_classifier = 0;
      break;
  }

  return new_classifier;
}

int delete_classifier(classifier* self)
{
  int exit_code;
  if (self == 0) return INPUT_ERROR;
  if (self -> clean == 0) return GENERAL_ERROR;
  exit_code = (self -> clean)(self);
  return exit_code;
}


