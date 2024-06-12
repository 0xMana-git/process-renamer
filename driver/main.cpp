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

BYTE bufferrr[0x1000];

void TestWrite() {

	PCUCHAR buffer = (unsigned const char[])"hi :333";  //  for example
	ULONG bufferSize = sizeof(buffer);

	UNICODE_STRING      filePath;   //  Must be with DOS prefix: \??\C:\MyFolder\logs.txt
	HANDLE              hFile;
	OBJECT_ATTRIBUTES   ObjectAttributes;
	IO_STATUS_BLOCK     IoStatusBlock;
	RtlCreateUnicodeString(&filePath, L"\\??\\C:\\logs.txt");
	InitializeObjectAttributes(&ObjectAttributes, &filePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	NTSTATUS Status = ZwCreateFile(&hFile, FILE_GENERIC_READ | FILE_GENERIC_WRITE, &ObjectAttributes,
		&IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_CREATE,
		FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

	Status = ZwWriteFile(hFile, NULL, NULL, NULL, &IoStatusBlock, (PVOID)buffer, bufferSize, NULL, NULL);
	RtlFreeUnicodeString(&filePath);
	ZwClose(hFile);
}

void TestWrite(UNICODE_STRING buf, PCWCHAR name) {

	PCUCHAR buffer = (PCUCHAR)buf.Buffer;  //  for example
	ULONG bufferSize = buf.MaximumLength;

	UNICODE_STRING      filePath;   //  Must be with DOS prefix: \??\C:\MyFolder\logs.txt
	HANDLE              hFile;
	OBJECT_ATTRIBUTES   ObjectAttributes;
	IO_STATUS_BLOCK     IoStatusBlock;
	RtlCreateUnicodeString(&filePath, name);
	InitializeObjectAttributes(&ObjectAttributes, &filePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	NTSTATUS Status = ZwCreateFile(&hFile, FILE_GENERIC_READ | FILE_GENERIC_WRITE, &ObjectAttributes,
		&IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_CREATE,
		FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

	Status = ZwWriteFile(hFile, NULL, NULL, NULL, &IoStatusBlock, (PVOID)buffer, bufferSize, NULL, NULL);
	RtlFreeUnicodeString(&filePath);
	ZwClose(hFile);
}



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
	NTSTATUS status = PsLookupProcessByProcessId((HANDLE)9692, &pEprocess); //lookup the process by its id;
	if (status != STATUS_SUCCESS)
	{
		TestWrite();
		return 0;
	}

	//KeWriteProcessMemory(Process, &Writeval, 0x010F29B0, sizeof(__int32));
	SE_AUDIT_PROCESS_CREATION_INFO procCreationInfo = *(SE_AUDIT_PROCESS_CREATION_INFO*)((UINT64)pEprocess + SE_AUDIT_PROCESS_CREATION_INFO_OFFSET);
	//PPEB pPeb = (PPEB)((UINT64)pEprocess + PEB_OFFSET);
	UNICODE_STRING oldNameDos, newNameDos;
	
	RtlCreateUnicodeString(&newNameDos, L"\\Device\\HardDiskVolume3\\did_u_know_that_jewz_did_911.exe");
	oldNameDos = procCreationInfo.ImageFileName->Name;
	

	RtlFreeUnicodeString(&oldNameDos);
	procCreationInfo.ImageFileName->Name = newNameDos;
	
	
	return STATUS_SUCCESS;
}