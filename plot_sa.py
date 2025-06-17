
import matplotlib.pyplot as plt
import pandas as pd

def plot_sa_results(csv_file='sa_data.csv'):
    data = pd.read_csv(csv_file)
    
    plt.figure(figsize=(12, 5))
    
    # Gráfico de Temperatura
    plt.subplot(1, 2, 1)
    plt.plot(data['Iteration'], data['Temperature'], 'b-')
    plt.title('Temperatura por Iteração')
    plt.xlabel('Iteração')
    plt.ylabel('Temperatura')
    plt.grid(True)
    
    # Gráfico de Custo
    plt.subplot(1, 2, 2)
    plt.plot(data['Iteration'], data['BestCost'], 'r-', label='Melhor Custo')
    plt.plot(data['Iteration'], data['CurrentCost'], 'g--', alpha=0.5, label='Custo Atual')
    plt.title('Evolução do Custo')
    plt.xlabel('Iteração')
    plt.ylabel('Custo')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.savefig('sa_results.png')
    plt.show()

if __name__ == "__main__":
    plot_sa_results()
