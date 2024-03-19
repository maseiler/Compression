import matplotlib.pyplot as plt
import pandas as pd

if __name__ == "__main__":
    df = pd.read_csv("data.txt")
    ax = plt.plot(df)
    plt.ylabel("Value")
    plt.xlabel("Position in buffer")
    plt.show()
