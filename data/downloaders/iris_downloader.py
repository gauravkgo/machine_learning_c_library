
import pandas as pd
from sklearn import datasets

IRIS_CSV_FILE = "../datasets/iris.csv"

iris_dataset = datasets.load_iris()
iris_dataframe = pd.DataFrame(data = iris_dataset.data, columns = iris_dataset.feature_names)
iris_species = [iris_dataset.target_names[index] for index in iris_dataset.target]
iris_dataframe.insert(0, "species", iris_species)

iris_dataframe.to_csv(IRIS_CSV_FILE)
