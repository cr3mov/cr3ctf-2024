#pragma once
#include <Windows.h>

#include "jm/li.hpp"
#include "jm/syscalls/inline_syscall.inl"

extern "C" {
    NTSTATUS NTAPI NtSetInformationProcess(HANDLE, PROCESS_INFORMATION_CLASS, PVOID, ULONG);
    NTSTATUS NTAPI NtDelayExecution(BOOLEAN Alertable, PLARGE_INTEGER Interval);

    NTSTATUS NTAPI NtAllocateVirtualMemory(HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType,
                                           ULONG Protect);
    NTSTATUS NTAPI NtWriteVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, SIZE_T BufferSize, PSIZE_T NumberOfBytesWritten);
}

namespace syscaller {
    template <typename Ret = long, typename... Args>
    __forceinline auto do_call(std::uint32_t idx, Args... args) -> Ret {
        return (Ret)::jm::detail::syscall(idx, args...);
    }

    template <typename Ret, typename... Args, typename... Args2>
    __forceinline auto do_call(Ret (*fn)(Args...), Args2... args) -> Ret {
        const auto syscall_index = reinterpret_cast<uint32_t*>(fn)[1];
        return do_call<Ret>(syscall_index, args...);
    }
} // namespace syscaller

#define SYSCALL(v, ...) ::syscaller::do_call(LI_FN(v).get(), __VA_ARGS__);
