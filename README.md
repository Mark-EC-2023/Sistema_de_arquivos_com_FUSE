# Sistema de Arquivos FUSE em C (Entrega Parcial)

Este projeto contempla a implementação parcial de um sistema de arquivos em espaço de usuário (FUSE) desenvolvido em linguagem C.

---

## Requisitos

Para compilar e executar o projeto em uma máquina virtual Linux (Ubuntu/Debian), instale as seguintes dependências:

```bash
sudo apt update
sudo apt install -y build-essential libfuse-dev pkg-config
gcc -Wall -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse meu_fs.c -o meu_fs -lfuse -pthread
```
