import pickle
def save2pickle(file_name, data):
    with open(file_name, 'wb') as f:
        pickle.dump(data, f)

def load_pickle(file_name):
    with open(file_name, 'rb') as f:
        return pickle.load(f)

