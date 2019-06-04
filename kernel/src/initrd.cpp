#include <stdlib.h>
#include <string.h>
#include <tar.h>
#include <inlow/kernel/file.h>
#include <inlow/kernel/initrd.h>
#include <inlow/kernel/print.h>

struct TarHeader {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
};

DirectoryVnode* Initrd::loadInitrd(inlow_vir_addr_t initrd) {
    DirectoryVnode* root = new DirectoryVnode();
    TarHeader* header = (TarHeader*) initrd;

    while (strcmp(header->magic, TMAGIC) == 0) {
        // TODO: Add support for subdirectories

        size_t size = (size_t) strtoul(header->size, NULL, 10);

        if (header->typeflag == REGTYPE || header->typeflag == AREGTYPE) {
            FileVnode* file = new FileVnode(header + 1, size);
            root->addChildNode(strdup(header->name), file);

            Print::printf("File: /%s, size = %zu\n", header->name, size);
        }

        header += 1 + ALIGN_UP(size, 512) / 512;
    }

    return root;
}