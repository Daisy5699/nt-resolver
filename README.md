# usage example
*entry.cxx*
```cpp
#include <resources/nt/nt.hxx>
#include <resources/ntos/ntos.hxx>

NTSTATUS DriverEntry( PDRIVER_OBJECT driver_obj , PUNICODE_STRING reg )
{
	NTSTATUS status = nt::g_resolver->initialize( );
	if ( !NT_SUCCESS( status ) )
		return status;

	HANDLE h_process = nullptr;
	OBJECT_ATTRIBUTES object_attributes;
	InitializeObjectAttributes( &object_attributes , nullptr , OBJ_KERNEL_HANDLE , nullptr , nullptr );
	nt::g_resolver->create_system_process( &h_process , PROCESS_ALL_ACCESS , &object_attributes );
}
```
