
/******************************************************************************/
/*                               L I B M L _ H                                */
/******************************************************************************/



#ifndef LIBML_H
#define LIBML_H



/******************************************************************************/
/*                                   Macros                                   */
/******************************************************************************/



#define NO_ERROR 0
#define GENERAL_ERROR 1
#define UNIMPLEMENTED 2
#define MEMORY_ERROR 3
#define INPUT_ERROR 4
#define FILE_ERROR 4

#define CLASSIFIER_TYPE_KNN 1

#define MAX_LABEL_LENGTH 64



/******************************************************************************/
/*                                   Basic                                    */
/******************************************************************************/



typedef enum
{
  ZERO = 0,
  KNN = 1
}
classifier_type;



typedef double scalar;

typedef struct vector_struct vector;
struct vector_struct
{
  int dimensionality;
  scalar* elements;
};

typedef struct matrix_struct matrix;
struct matrix_struct
{
  int rows;
  int columns;
  scalar* elements;
};

typedef struct tensor_struct tensor;
struct tensor_struct
{
  int order;
  int* shape;
  scalar* elements;
};


/* Sample description */
vector* create_vector_array(int dimensionality, int array_size);

void delete_vector_array(vector* vector_array, int array_size);
void copy_vector(vector* destination_vector, vector* source_vector);

char** create_labels(int array_size);
void delete_labels(char** labels, int array_size);
void copy_label(char* destination_label, char* source_label);



/*

functions
sum
distance
square root

*/



scalar euclidean_distance(vector a, vector b);
scalar minkowski_distance(vector a, vector b, int p);



/******************************************************************************/
/*                                    Data                                    */
/******************************************************************************/



typedef struct table_struct table;
struct table_struct
{
  int rows;
  int columns;
  char** column_labels;
  vector* sample_vectors;
  char** class_labels;
};

/* Assumes csv has header, and rest all rows are valid, and file ends in new line. */
table* create_table_from_csv(const char* filename, int class_column_index, int* data_columns_indices, int number_of_data_columns);
void delete_table(table* table);
void print_table(table* table);
table* create_subset_table(table* source_table, int* row_slice_indices, int row_slice_indices_len);
void strip_table_classes(table* stripping_table);




/******************************************************************************/
/*                                 Classifier                                 */
/******************************************************************************/



typedef struct classifier_struct classifier;
struct classifier_struct
{
  classifier_type type_of_classifier;
  int (* initialise) (classifier* self, void* data);
  void (* info) (classifier* self);
  int (* train) (classifier* self, table* training_table);
  int (* classify) (classifier* self, table* classifying_table);
  int (* clean) (classifier* self);
  void* data;
};

classifier* create_classifier(classifier_type type_of_classifer);
int delete_classifier(classifier* self);




#endif
