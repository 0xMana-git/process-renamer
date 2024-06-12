#include <ntifs.h>
#define WINDOWS_IGNORE_PACKING_MISMATCH

#include "peb.h"


using BYTE = unsigned __int8;
struct SE_AUDIT_PROCESS_CREATION_INFO
{
	OBJECT_NAME_INFORMATION* ImageFileName;
} *PSE_AUDIT_PROCESS_CREATION_INFO;


constexpr UINT64 SE_AUDIT_PROCESS_CREATION_INFO_OFFSET = 0x05c0;
constexpr UINT64 PEB_OFFSET = 0x0550;
constexpr UINT64 IMAGE_FILE_NAME_OFFSET = 0x05a8;


constexpr int pid = 4;

extern "C"
NTSTATUS DriverEntry(
	_In_  struct _DRIVER_OBJECT* DriverObject,
	_In_  PUNICODE_STRING RegistryPath
)
{	
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	PEPROCESS pEprocess; // our target process
	
	// enter your process ID here.
	NTSTATUS status = PsLookupProcessByProcessId((HANDLE)pid, &pEprocess); //lookup the process by its id;
	if (status != STATUS_SUCCESS)
	{
		DbgPrint("Unable to find process");
		return 0;
	}

	SE_AUDIT_PROCESS_CREATION_INFO procCreationInfo = *(SE_AUDIT_PROCESS_CREATION_INFO*)((UINT64)pEprocess + SE_AUDIT_PROCESS_CREATION_INFO_OFFSET);
	//PPEB pPeb = (PPEB)((UINT64)pEprocess + PEB_OFFSET);
	UNICODE_STRING oldNameDos, newNameDos;
	
	RtlCreateUnicodeString(&newNameDos, L"\\Device\\HardDiskVolume3\\did_u_know_that_jewz_did_911.exe");
	oldNameDos = procCreationInfo.ImageFileName->Name;
	

	RtlFreeUnicodeString(&oldNameDos);
	procCreationInfo.ImageFileName->Name = newNameDos;
	
	
	return STATUS_SUCCESS;
}