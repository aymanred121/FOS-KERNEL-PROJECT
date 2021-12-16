#include <inc/lib.h>

// malloc()
//	This function use FIRST FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

	int UH_UnAllocated_Space_Start=USER_HEAP_START;
	uint32 num_OF_Free_pages_In_heap = (USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE;
	uint32 allocation_Info[(USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE] = {0};
	uint32 num_OF_pages_In_heap = (USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE;

// 1 000 1

void* malloc(uint32 size)
{
	//TODO: [PROJECT 2021 - [2] User Heap] malloc() [User Side]
//	cprintf("start");
	size = ROUNDUP(size,PAGE_SIZE);
	if(size==0 || size > (num_OF_Free_pages_In_heap * PAGE_SIZE))
		return NULL;

	///////******* Searching for space ******////////
	int min_Satisfying_Frames_Found=-1;
	uint32 Start_Address_OF_Min_Size=0;
	for(int i=0 ; i <num_OF_pages_In_heap ;i++)
	{

		if(allocation_Info[i]==0)
		{
			int counter=0;
			for(int j = i ; j < num_OF_pages_In_heap ; j++ )
			{
				if((j*PAGE_SIZE)+USER_HEAP_START== (UH_UnAllocated_Space_Start) )
				{
					if(size < USER_HEAP_MAX-UH_UnAllocated_Space_Start)
					{
						if((USER_HEAP_MAX-UH_UnAllocated_Space_Start)/PAGE_SIZE <min_Satisfying_Frames_Found
							|| min_Satisfying_Frames_Found ==-1)
						{
							uint32 final_=(UH_UnAllocated_Space_Start-USER_HEAP_START)/PAGE_SIZE;
							 sys_allocateMem(UH_UnAllocated_Space_Start,size);
							allocation_Info[final_]=size/PAGE_SIZE;
							UH_UnAllocated_Space_Start+=size;
							num_OF_Free_pages_In_heap-=size/PAGE_SIZE;
							return (void *)(UH_UnAllocated_Space_Start-size);

						}
					}
				}
				if(allocation_Info[j]==0)
					counter++;
				else
				{
					if(size/PAGE_SIZE >= counter)
					{
						if(counter <min_Satisfying_Frames_Found
								|| min_Satisfying_Frames_Found ==-1)
						{
							min_Satisfying_Frames_Found=counter;
							Start_Address_OF_Min_Size=i;

						}
					}
					i=j-1;
					break;
				}
			}
		}
		else
		{
            i+=allocation_Info[i]-1;
		}
	}
	if(min_Satisfying_Frames_Found==-1)
		return NULL;
	else
	{
		uint32 final_va=(Start_Address_OF_Min_Size*PAGE_SIZE)+USER_HEAP_START;
	    sys_allocateMem(Start_Address_OF_Min_Size,size);
	    allocation_Info[Start_Address_OF_Min_Size]=size/PAGE_SIZE;
	    num_OF_Free_pages_In_heap-=size/PAGE_SIZE;
	    return (void *)final_va;

	}

}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.run tm1
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address)
{
	//TODO: [PROJECT 2021 - [2] User Heap] free() [User Side]
	uint32 index_in_the_array=((uint32)(virtual_address)-USER_HEAP_START)/PAGE_SIZE;
	sys_freeMem((uint32)virtual_address,allocation_Info[index_in_the_array]*PAGE_SIZE);
	allocation_Info[index_in_the_array]=0;
}

//==================================================================================//
//================================ OTHER FUNCTIONS =================================//
//==================================================================================//

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	panic("this function is not required...!!");
	return 0;
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	panic("this function is not required...!!");
	return 0;
}

void sfree(void* virtual_address)
{
	panic("this function is not required...!!");
}

void *realloc(void *virtual_address, uint32 new_size)
{
	panic("this function is not required...!!");
	return 0;
}

void expand(uint32 newSize)
{
	panic("this function is not required...!!");
}
void shrink(uint32 newSize)
{
	panic("this function is not required...!!");
}

void freeHeap(void* virtual_address)
{
	panic("this function is not required...!!");
}
