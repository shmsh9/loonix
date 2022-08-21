#ifndef EFI_RUNTIME_SERVICES_H_
#define EFI_RUNTIME_SERVICES_H_
#include "efi.h"
typedef enum {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown
} Efi_Reset_Type;
 
typedef
uint64_t (*Efi_Reset_System) 
(
    Efi_Reset_Type  	        	ResetType,
    uint64_t		              	ResetStatus,
    uint64_t				        DataSize,
    uint16_t                   		*ResetData
);

typedef struct s_efi_table_header {
	uint64_t                     signature;
	uint32_t                     rev;
	uint32_t                     size;
	uint32_t                     crc;
	uint32_t                     reserved;
} efi_Table_Header;

typedef struct s_efi_runtime_service_handle {
	efi_Table_Header header;
	efi_status_t (*GetTime)(struct efi_time *t, uintptr_t capabilities);
	void *                      SetTime;
	void *             GetWakeupTime;
	void *             SetWakeupTime;
	void *     SetVirtualAddressMap;
	void *             ConvertPointer;
	void *                GetVariable;
	void *      GetNextVariableName;
	void *                SetVariable;
	void *    GetNextHighMonotonicCount;
	void (*ResetSystem)(Efi_Reset_Type, UINTN, UINTN, void *);
	void *              UpdateCapsule;
	void *  QueryCapsuleCapabilities;
	void *         QueryVariableInfo;
} efi_runtime_services;


#endif
