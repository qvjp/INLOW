#include <dirent.h>

void rewinddir(DIR* dir)
{
	dir->offset = 0;
}
