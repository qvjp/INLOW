#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <tar.h>
#include <inlow/kernel/file.h>
#include <inlow/kernel/initrd.h>
#include <inlow/kernel/print.h>

struct TarHeader
{
		char name[100];
		char mode[8];
		char uid[8];
		char gui[8];
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

Reference<DirectoryVnode> Initrd::loadInitrd(vaddr_t initrd)
{
		Reference<DirectoryVnode> root(new DirectoryVnode(Reference<DirectoryVnode>(), 0755, 0, 0));
		TarHeader* header = (TarHeader*) initrd;

		while (strcmp(header->magic, TMAGIC) == 0)
		{
				size_t size = (size_t) strtoul(header->size, nullptr, 8);
				char* path;
				if (header->prefix[0])
				{
						path = (char*) malloc(strlen(header->name) + strlen(header->prefix) + 2);
						stpcpy(stpcpy(stpcpy(path, header->prefix), "/"), header->name);
				}
				else
				{
						path = strdup(header->name);
				}
				char* path2 = strdup(path);
				char* dirName = dirname(path);
				char* fileName = basename(path2);
				Reference<DirectoryVnode> directory = (Reference<DirectoryVnode>)resolvePath(root, dirName);
				if (!directory)
				{
						Print::printf("Could not add '%s' to nonexistent directory '%s'.\n", fileName, dirName);
						return root;
				}
				Reference<Vnode> newFile;
				mode_t mode = (mode_t) strtol(header->mode, nullptr, 8);
				if (header->typeflag == REGTYPE || header->typeflag == AREGTYPE)
				{
						newFile = Reference<Vnode>(new FileVnode(header + 1, size, mode, directory->dev, 0));
						header += 1 + ALIGNUP(size, 512) / 512;
				}
				else if (header->typeflag == DIRTYPE)
				{
						newFile = Reference<Vnode>(new DirectoryVnode(directory, mode, directory->dev, 0));
						header++;
				}
				else
				{
						Print::printf("Unknow typeflag '%c'\n", header->typeflag);
						return root;
				}
				directory->link(fileName, newFile);
				// Print::printf("File: %s/%s, size = %zu\n", dirName, fileName, size);

				free(path);
				free(path2);
		}
		return root;
}
