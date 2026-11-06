# Sistema de Arquivos FUSE em C (Entrega Parcial)

Este projeto contempla a implementação parcial de um sistema de arquivos em espaço de usuário (FUSE) desenvolvido em linguagem C.

---

## 🛠️ Pré-requisitos

Para compilar e executar o projeto em uma máquina virtual Linux (Ubuntu/Debian), instale as seguintes dependências:

```bash
sudo apt update
sudo apt install -y build-essential libfuse-dev pkg-config

---
```bash
gcc -Wall meu_fs.c `pkg-config fuse --cflags --libs` -o meu_fs
