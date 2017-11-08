#ifndef KERNEL_PRINT_H
#define KERNEL_PRINT_H

namespace Print {
	void printf(const char* format, ...) __attribute__((__format__(printf, 1, 2)));
}

#endif
