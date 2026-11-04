#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX_FILES 20
#define MAX_PATH 256
#define MAX_CONTENT 4096

typedef struct {
    char path[MAX_PATH];
    char content[MAX_CONTENT];
    size_t size;
} file_node_t;

static file_node_t files[MAX_FILES];
static int file_count = 0;

// Obter atributos do ficheiro ou diretório
static int fs_getattr(const char *path, struct stat *stbuf) {
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    for (int i = 0; i < file_count; i++) {
        if (strcmp(path, files[i].path) == 0) {
            stbuf->st_mode = S_IFREG | 0644;
            stbuf->st_nlink = 1;
            stbuf->st_size = files[i].size;
            return 0;
        }
    }

    return -ENOENT;
}

// Listar o conteúdo do diretório
static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                      off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    if (strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    for (int i = 0; i < file_count; i++) {
        // Remove a barra inicial '/' para a listagem
        filler(buf, files[i].path + 1, NULL, 0);
    }

    return 0;
}

// Criar um novo ficheiro
static int fs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    (void) mode;
    (void) fi;

    if (file_count >= MAX_FILES)
        return -ENOSPC;

    for (int i = 0; i < file_count; i++) {
        if (strcmp(path, files[i].path) == 0)
            return 0;
    }

    strncpy(files[file_count].path, path, MAX_PATH - 1);
    files[file_count].content[0] = '\0';
    files[file_count].size = 0;
    file_count++;

    return 0;
}

// Ler o conteúdo de um ficheiro
static int fs_read(const char *path, char *buf, size_t size, off_t offset,
                   struct fuse_file_info *fi) {
    (void) fi;

    for (int i = 0; i < file_count; i++) {
        if (strcmp(path, files[i].path) == 0) {
            size_t len = files[i].size;
            if (offset < len) {
                if (offset + size > len)
                    size = len - offset;
                memcpy(buf, files[i].content + offset, size);
            } else {
                size = 0;
            }
            return size;
        }
    }

    return -ENOENT;
}

// Escrever dados num ficheiro
static int fs_write(const char *path, const char *buf, size_t size,
                    off_t offset, struct fuse_file_info *fi) {
    (void) fi;

    for (int i = 0; i < file_count; i++) {
        if (strcmp(path, files[i].path) == 0) {
            if (offset + size > MAX_CONTENT)
                return -ENOSPC;

            memcpy(files[i].content + offset, buf, size);
            if (offset + size > files[i].size)
                files[i].size = offset + size;

            return size;
        }
    }

    return -ENOENT;
}

static struct fuse_operations fs_oper = {
    .getattr = fs_getattr,
    .readdir = fs_readdir,
    .create  = fs_create,
    .read    = fs_read,
    .write   = fs_write,
};

int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &fs_oper, NULL);
}
