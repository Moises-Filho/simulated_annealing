
import matplotlib.pyplot as plt
import pandas as pd

def plot_sa_results(csv_file='../output/resultado.csv'):
    data = pd.read_csv(csv_file)
    
    plt.figure(figsize=(12, 5))
    
    # Gráfico de Custo
    plt.plot(data['Iteration'], data['BestCost'], 'r-', label='Melhor Custo')
    plt.plot(data['Iteration'], data['Cost'], 'g--', alpha=0.5, label='Custo Atual')
    plt.title('Evolução do Custo')
    plt.xlabel('Iteração')
    plt.ylabel('Custo')
    plt.legend()
    plt.grid(True)
    plt.ticklabel_format(style='plain', axis='both')
    
    ax1 = plt.gca()
    ax2 = ax1.twinx()
    ax2.plot(data['Iteration'], data['Temperature'], 'b-', label='Temperatura')
    ax2.set_ylabel('Temperatura')
    ax2.ticklabel_format(style='plain', axis='y')
    ax2.grid(False)
    #ax2.title('Temperatura por Iteração')
    #plt.xlabel('Iteração')
    #plt.ylabel('Temperatura')
    #plt.grid(True)
    #plt.ticklabel_format(style='plain', axis='both')
    

    plt.tight_layout()
    plt.savefig('sa_results.png')
    plt.show()

if __name__ == "__main__":
    plot_sa_results()
