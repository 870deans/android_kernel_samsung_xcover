/* include/linux/android_pmem.h
 *
 * Copyright (C) 2007 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _ANDROID_PMEM_H_
#define _ANDROID_PMEM_H_

#define PMEM_IOCTL_MAGIC 'p'
#define PMEM_GET_PHYS		_IOW(PMEM_IOCTL_MAGIC, 1, unsigned int)
#define PMEM_MAP		_IOW(PMEM_IOCTL_MAGIC, 2, unsigned int)
#define PMEM_GET_SIZE		_IOW(PMEM_IOCTL_MAGIC, 3, unsigned int)
#define PMEM_UNMAP		_IOW(PMEM_IOCTL_MAGIC, 4, unsigned int)
/* This ioctl will allocate pmem space, backing the file, it will fail
 * if the file already has an allocation, pass it the len as the argument
 * to the ioctl */
#define PMEM_ALLOCATE		_IOW(PMEM_IOCTL_MAGIC, 5, unsigned int)
/* This will connect a one pmem file to another, pass the file that is already
 * backed in memory as the argument to the ioctl
 */
#define PMEM_CONNECT		_IOW(PMEM_IOCTL_MAGIC, 6, unsigned int)
/* Returns the total size of the pmem region it is sent to as a pmem_region
 * struct (with offset set to 0). 
 */
#define PMEM_GET_TOTAL_SIZE	_IOW(PMEM_IOCTL_MAGIC, 7, unsigned int)
#define PMEM_CACHE_FLUSH	_IOW(PMEM_IOCTL_MAGIC, 8, unsigned int)
/* Support streaming DMA mapping */
#define PMEM_MAP_REGION		_IOW(PMEM_IOCTL_MAGIC, 0x10, unsigned int)
#define PMEM_UNMAP_REGION	_IOW(PMEM_IOCTL_MAGIC, 0x11, unsigned int)

struct android_pmem_platform_data
{
	const char* name;
	/* starting physical address of memory region */
	unsigned long start;
	/* size of memory region */
	unsigned long size;
	/* set to indicate the region should not be managed with an allocator */
	unsigned no_allocator;
	/* set to indicate maps of this region should be cached, if a mix of
	 * cached and uncached is desired, set this and open the device with
	 * O_SYNC to get an uncached region */
	unsigned cached;
	/* enable a write buffer in the bus controller */
	unsigned buffered;
};

struct pmem_region {
	unsigned long offset;
	unsigned long len;
};

#include <linux/dma-mapping.h>

struct pmem_sync_region {
	struct pmem_region region;
	enum dma_data_direction dir;
};

#ifdef CONFIG_ANDROID_PMEM
int is_pmem_file(struct file *file);
int get_pmem_file(int fd, unsigned long *start, unsigned long *vstart,
		  unsigned long *end, struct file **filp);
int get_pmem_user_addr(struct file *file, unsigned long *start,
		       unsigned long *end);
void put_pmem_file(struct file* file);
void flush_pmem_file(struct file *file, unsigned long start, unsigned long len);
void sync_pmem_file(struct file *file, unsigned long start, unsigned long len,
		    unsigned int cmd, enum dma_data_direction dir);
int pmem_setup(struct android_pmem_platform_data *pdata,
	       long (*ioctl)(struct file *, unsigned int, unsigned long),
	       int (*release)(struct inode *, struct file *));
int pmem_remap(struct pmem_region *region, struct file *file,
	       unsigned operation);
void sync_pmem_area(void *vaddr, unsigned long addr, unsigned long len,
		    unsigned int cmd, enum dma_data_direction dir);

#else
static inline int is_pmem_file(struct file *file) { return 0; }
static inline int get_pmem_file(int fd, unsigned long *start,
				unsigned long *vstart, unsigned long *end,
				struct file **filp) { return -ENOSYS; }
static inline int get_pmem_user_addr(struct file *file, unsigned long *start,
				     unsigned long *end) { return -ENOSYS; }
static inline void put_pmem_file(struct file* file) { return; }
static inline void flush_pmem_file(struct file *file, unsigned long start,
				   unsigned long len) { return; }
static inline void sync_pmem_file(struct file *file, unsigned long start,
				   unsigned long len, unsigned int cmd,
				   enum dma_data_direction dir); { return; }
static inline int pmem_setup(struct android_pmem_platform_data *pdata,
	      long (*ioctl)(struct file *, unsigned int, unsigned long),
	      int (*release)(struct inode *, struct file *)) { return -ENOSYS; }

static inline int pmem_remap(struct pmem_region *region, struct file *file,
			     unsigned operation) { return -ENOSYS; }
static inline void sync_pmem_area(void *vaddr, unsigned long addr,
		unsigned long len, unsigned int cmd,
		enum dma_data_direction dir) { return; }
#endif

#endif //_ANDROID_PPP_H_

