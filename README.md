![sharklog_banner](https://github.com/user-attachments/assets/52e1d32d-f748-4dd4-9bc6-aaa2b157c870)

# 🦈 SharkLog

**Disciplina:** Programação Imperativa e Funcional - 2025.2 <br>
**Instituição:** CESAR School  
**Autores:**  
- Eduardo Henrique Albuquerque (@duduhnrq)  
- Luiz Henrique Souza da Conceição (@LouisLuos)
- Pedro Henrique da Silva Marrocos (@Pedrinhosds16)
- Pedro Vinicius Silva de Souza (@PvssDev)
- Paulo César da Silva Marrocos (@paulosds2318)
- Cauã dos Santos Nascimento (@Santos-dev25)

---

## 🎯 Descrição do Jogo

O **SharkLog** é um jogo de terminal (CLI) inspirado na praia de Boa Viagem, em Recife. O jogador deve se aventurar pelo mar, tentando chegar o mais longe possível **sem encontrar um tubarão**!  

Cada passo dado nas águas recifenses é um risco — você controla um surfista que precisa acumular o máximo de pontos enquanto foge de tubarões famintos. A cada movimento, surge uma pergunta de lógica computacional — responda certo para avançar e pontuar; erre e você desequilibra, fica imóvel e vira alvo fácil. Os tubarões se aproximam cada vez mais… sobreviva, pense rápido e conquiste o maior score!

---

## 🕹️ Regras do Jogo

- Você controla um surfista que deve se mover pelo tabuleiro enquanto foge dos tubarões.  
- Ao longo do jogo, perguntas de lógica serão exibidas:
  - **1** → Verdadeiro  
  - **2** → Falso  
- **Acertou:** continua se movendo e ganha pontos.  
- **Errou:** fica imóvel por um turno e perde **1 vida**.  
- Tubarões surgem a cada rodada.  
- O jogo termina quando um tubarão alcança o jogador.  
- O objetivo é **marcar a maior pontuação possível** antes de ser pego.

---

## ⚙️ Como Compilar e Executar

### 🔧 Pré-requisitos
- Sistema operacional **Linux** ou **macOS**
- Compilador **GCC**
- Biblioteca **CLI-lib** instalada (https://github.com/tgfb/cli-lib)

### 💻 Compilação manual:
```bash
gcc src/*.c -Iinclude -lcli -o build/sharklog
```
### ▶️ Execução
Para iniciar o jogo, execute o binário gerado dentro da pasta `build`:

```bash
./build/sharklog
```

---

## 🧑‍💻 Créditos

Desenvolvido por alunos da **CESAR School**  
Sob orientação das disciplinas de:
- **Programação Imperativa e Funcional (PIF)**
- **Lógica Computacional**

---

## 📜 Licença
Este projeto pode ser utilizado sob a licença **MIT**.  
Sinta-se livre para estudar, modificar e expandir o SharkLog! 🦈

---

📘 Para detalhes técnicos do projeto, acesse a [Documentação Técnica](DOCUMENTACAO_TECNICA.md).
