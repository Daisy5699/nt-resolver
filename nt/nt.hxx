#pragma once
#include <ntddk.h>
#include <ntdef.h>
#include <ntifs.h>
#include <shared\std\std.hxx>
#include <resources\ntos\ntos.hxx>

namespace ntos // from my ntos.hxx
{
   using fn_ps_create_system_process = NTSTATUS( NTAPI * )( OUT PHANDLE process_handle , IN ULONG desired_access ,
	   IN POBJECT_ATTRIBUTES object_attributes );
   using fn_ps_remove_create_thread_notify_routine = NTSTATUS( NTAPI * )( IN PCREATE_THREAD_NOTIFY_ROUTINE notify_routine );
}

namespace nt
{
	class c_resolver
	{
	public:
		ntos::fn_ps_create_system_process ps_create_system_process = nullptr;
		ntos::fn_ps_remove_create_thread_notify_routine ps_remove_create_thread_notify_routine = nullptr;

		NTSTATUS initialize( ) noexcept
		{
			NTSTATUS status = STATUS_SUCCESS;

			auto resolve = [ & ] ( const wchar_t * name , void *& out_fn ) -> bool
				{
					UNICODE_STRING ustring;
					RtlInitUnicodeString( &ustring , name );

					out_fn = MmGetSystemRoutineAddress( &ustring );
					if ( !out_fn )
					{
						KdPrint( "failed to resolve -> %wZ\n" , &ustring );
						status = STATUS_PROCEDURE_NOT_FOUND;
						return false;
					}

					KdPrint( "resolved %wZ -> %wZ -> %p" , &ustring , out_fn );
					return true;
				};

			resolve( L"PsCreateSystemProcess" , reinterpret_cast< void *& >( this->ps_create_system_process ) );
			resolve( L"PsRemoveCreateThreadNotifyRoutine" , reinterpret_cast< void *& >( this->ps_remove_create_thread_notify_routine ) );

			return status;
		}

		NTSTATUS create_system_process(OUT PHANDLE process_handle , IN ULONG desired_access , 
			IN POBJECT_ATTRIBUTES object_attributes ) const noexcept
		{
			NT_ASSERT( this->ps_create_system_process );
			return this->ps_create_system_process( process_handle , desired_access , object_attributes );
		}
	};
	inline nt::c_resolver * g_resolver = new nt::c_resolver( );
}
