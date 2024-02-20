
/******************************************************************************/
/*                                 K N N _ H                                  */
/******************************************************************************/



#ifndef KNN_H
#define KNN_H



#include "libml.h"



/******************************************************************************/
/*                   K Nearest Neighbors Classifier Methods                   */
/******************************************************************************/



int knn_initialise(classifier* self, void* k_pointer);
void knn_info(classifier* self);
int knn_train(classifier* self, table* training_table);
int knn_classify(classifier* self, table* classifying_table);
int knn_clean(classifier* self);



/******************************************************************************/
/*                    K Nearest Neighbors Classifier Data                     */
/******************************************************************************/



typedef struct knn_data_struct knn_data;
struct knn_data_struct
{
  int k;
  int number_of_reference_samples;
  vector* reference_samples;
  char** reference_labels;
};



/******************************************************************************/
/*                   K Nearest Neighbors Classifier Helpers                   */
/******************************************************************************/



knn_data* get_knn_data(classifier* self);
void knn_classify_one_sample(knn_data* data, char** train_labels_copy, vector current_vector, char* destination_class_label, scalar* distances_array);
void store_labels_of_k_shortest_distances(knn_data* data, char** train_labels_copy, scalar* distances_array, int k);
char* get_mode_of_k_labels(char** k_labels, int k);
void debugging_print_scalar_array(scalar* scalar_array, int scalar_array_length);
void debugging_print_str_array(char** str_array, int str_array_length);
void clean_knn_data(knn_data* data);



#endif
