import os
import matplotlib.pyplot as plt
import pandas as pd

def plot_sa_results(csv_file):
    if not os.path.exists(csv_file):
        print(f"Arquivo não encontrado: {csv_file}")
        return

    if os.path.getsize(csv_file) == 0:
        print(f"Arquivo vazio: {csv_file}")
        return

    data = pd.read_csv(csv_file)
    print(f"Arquivo carregado. Tamanho: {len(data)} linhas")
    print("Colunas:", data.columns.tolist())

    # Carregar dados
    data = pd.read_csv(csv_file)
    
    # Criar figura com 2 subplots
    plt.figure(figsize=(12, 6))
    
    # Gráfico 1: Temperatura por Iteração
    plt.subplot(1, 2, 1)
    plt.plot(data['Iteration'], data['Temperature'], 'b-')
    plt.title('Temperatura por Iteração')
    plt.xlabel('Iteração')
    plt.ylabel('Temperatura')
    plt.grid(True)
    
    # Gráfico 2: Qualidade da Solução por Iteração
    plt.subplot(1, 2, 2)
    plt.plot(data['Iteration'], data['BestCost'], 'r-', label='Melhor Custo')
    plt.plot(data['Iteration'], data['Cost'], 'g--', alpha=0.5, label='Custo Atual')
    plt.title('Qualidade da Solução por Iteração')
    plt.xlabel('Iteração')
    plt.ylabel('Custo')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.savefig('sa_results.png')
    plt.show()

if __name__ == "__main__":
    plot_sa_results('../output/resultado.csv')