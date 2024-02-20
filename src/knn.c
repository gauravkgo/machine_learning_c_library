
/******************************************************************************/
/*                                 K N N _ C                                  */
/******************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "knn.h"



/******************************************************************************/
/*                   K Nearest Neighbors Classifier Methods                   */
/******************************************************************************/



int knn_initialise(classifier* self, void* k_pointer)
{
  knn_data* data;
  int k;

  data = (knn_data*) calloc(1, sizeof(knn_data));
  if (data == 0) return MEMORY_ERROR;

  self -> data = data;
  k = *(int*) k_pointer;
  data -> k = k;

  return NO_ERROR;
}

void knn_info(classifier* self)
{
  if (self -> data == 0)
  {
    (void) printf("Error: KNN classifier data is not initialised.\n");
    return;
  }
  
  (void) printf(
    "KNN Classifier Data at %p:\n\tk: %d\n\tNumber of samples: %d\n",
    (void*) self, get_knn_data(self) -> k, get_knn_data(self) -> number_of_reference_samples
  );

  return;
}

int knn_train(classifier* self, table* training_table)
{
  int i;
  vector* train_samples;
  char** train_labels;

  get_knn_data(self) -> number_of_reference_samples = training_table -> rows;
  train_samples = create_vector_array(training_table -> columns, training_table -> rows);
  if (train_samples == 0) return MEMORY_ERROR;
  train_labels = create_labels(training_table -> rows);
  if (train_labels == 0) return MEMORY_ERROR;

  for (i = 0; i < get_knn_data(self) -> number_of_reference_samples; i ++)
  {
    copy_vector(&(train_samples[i]), &((training_table -> sample_vectors)[i]));
    copy_label(train_labels[i], (training_table -> class_labels)[i]);
  }

  get_knn_data(self) -> reference_samples = train_samples;
  get_knn_data(self) -> reference_labels = train_labels;

  return NO_ERROR;
}

int knn_classify(classifier* self, table* classifying_table)
{
  int i;
  scalar* shared_storage_of_distances = 0;
  char** new_copy_class_labels = 0;
  knn_data* data = 0;

  data = get_knn_data(self);
  
  shared_storage_of_distances = (scalar*) calloc(data -> number_of_reference_samples, sizeof(scalar));
  if (shared_storage_of_distances == 0) return MEMORY_ERROR;
  new_copy_class_labels = create_labels(data -> number_of_reference_samples);
  if (new_copy_class_labels == 0) {free(shared_storage_of_distances); return MEMORY_ERROR;}

  for (i = 0; i < classifying_table -> rows; i ++)
    knn_classify_one_sample(
      data, new_copy_class_labels,
      (classifying_table -> sample_vectors)[i], (classifying_table -> class_labels)[i],
      shared_storage_of_distances
    );

  free(shared_storage_of_distances);
  delete_labels(new_copy_class_labels, data -> number_of_reference_samples);

  return NO_ERROR;
}

int knn_clean(classifier* self)
{
  clean_knn_data(get_knn_data(self));
  (void) memset(self, 0, sizeof(classifier));
  free(self);
  return NO_ERROR;
}



/******************************************************************************/
/*                   K Nearest Neighbors Classifier Helpers                   */
/******************************************************************************/



knn_data* get_knn_data(classifier* self)
{
  return (knn_data*) (self -> data);
}

void knn_classify_one_sample(knn_data* data, char** sorting_train_labels, vector current_vector, char* destination_class_label, scalar* distances_array)
{
  int i;
  for (i = 0; i < data -> number_of_reference_samples; i ++) distances_array[i] = euclidean_distance((data -> reference_samples)[i], current_vector);
  for (i = 0; i < data -> number_of_reference_samples; i ++) copy_label(sorting_train_labels[i], (data -> reference_labels)[i]);
  store_labels_of_k_shortest_distances(data, sorting_train_labels, distances_array, data -> k);
  copy_label(destination_class_label, get_mode_of_k_labels(sorting_train_labels, data -> k));
  return;
}

void store_labels_of_k_shortest_distances(knn_data* data, char** sorting_train_labels, scalar* distances_array, int k)
{
  int distances_index;
  int distances_left_end_index;
  int labels_array_index = 0;
  scalar local_min_distance;
  int local_min_distance_index;
  char temp_swap[MAX_LABEL_LENGTH];

  for (distances_left_end_index = 0; distances_left_end_index < k; distances_left_end_index ++)
  {
    local_min_distance = distances_array[distances_left_end_index];
    local_min_distance_index = distances_left_end_index;
    for (distances_index = distances_left_end_index + 1; distances_index < data -> number_of_reference_samples; distances_index ++)
    {
      if (distances_array[distances_index] < local_min_distance)
      {
        local_min_distance = distances_array[distances_index];
        local_min_distance_index = distances_index;
      }
    }
    if (local_min_distance_index != distances_left_end_index)
    {
      distances_array[local_min_distance_index] = distances_array[distances_left_end_index];
      distances_array[distances_left_end_index] = local_min_distance;
      copy_label(temp_swap, (sorting_train_labels)[local_min_distance_index]);
      copy_label((sorting_train_labels)[local_min_distance_index], (sorting_train_labels)[distances_left_end_index]);
      copy_label((sorting_train_labels)[distances_left_end_index], temp_swap);
    }
    labels_array_index ++;
  }
  return;
}

char* get_mode_of_k_labels(char** k_labels, int k)
{
  int i, j;
  int max_number_of_matches = 0;
  int number_of_matches;
  char* curr_mode_label;
  char* curr_label;

  for (i = 0; i < k; i ++)
  {
    curr_label = k_labels[i];
    number_of_matches = 0;
    for (j = 0; j < k; j ++) number_of_matches += (strncmp(curr_label, k_labels[j], MAX_LABEL_LENGTH) == 0);
    if (number_of_matches > max_number_of_matches)
    {
      max_number_of_matches = number_of_matches;
      curr_mode_label = curr_label;
    }
  }
  return curr_mode_label;
}

void debugging_print_scalar_array(scalar* scalar_array, int scalar_array_length)
{
  int i = 0;
  (void) printf("[%.2f", scalar_array[i ++]);
  while (i < scalar_array_length) (void) printf(", %.2f", scalar_array[i ++]);
  (void) printf("]\n\n");
  return;
}

void debugging_print_str_array(char** str_array, int str_array_length)
{
  int i = 0;
  (void) printf("[%s", str_array[i ++]);
  while (i < str_array_length) (void) printf(", %s", str_array[i ++]);
  (void) printf("]\n\n");
  return;
}

void clean_knn_data(knn_data* data)
{
  if (data == 0) return;
  if (data -> reference_samples != 0) delete_vector_array(data -> reference_samples, data -> number_of_reference_samples);
  if (data -> reference_labels != 0) delete_labels(data -> reference_labels, data -> number_of_reference_samples);
  (void) memset(data, 0, sizeof(knn_data));
  free(data);
  return;
}
