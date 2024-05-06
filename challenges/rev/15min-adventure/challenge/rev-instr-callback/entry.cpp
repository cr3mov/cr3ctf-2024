#include "util/libucrt_patch.hpp"

#include "instrumentation/instrumentation.hpp"
#include "jm/li.hpp"
#include "util/defines.hpp"
#include "util/syscalls.hpp"

#include "keys.hpp"

#include <iostream>

/// \brief Stealthy init within the patched __p_argc() CRT function
U_FORCEINL void libcrt::argc_callback() {
    instrumentation::startup();
}

/// \brief Flag checker function, we don't want to inline it because
/// we don't want to raise any supicious thoughts on why we are allocating
/// and coyping memory for it using the ntapi syscalls
__declspec(noinline) bool check_flag(unsigned char* flag, std::size_t flag_size) {
    /// Encrypting using the keys
    auto keys = keys::get_keys1();
    for (std::size_t i = 0; i < flag_size; ++i) {
        flag[i] ^= keys[i];
    }

    /// Comparing the result
    auto encrypted_flag = keys::get_encrypted_flag();
    bool result = flag_size == keys::kSize;
    for (std::size_t i = 0; i < flag_size; ++i) {
        result &= flag[i] == encrypted_flag[i];
    }

    /// We don't want to treat the beginning of the correct flag as a correct flag
    return result;
}

/// \brief Allocate new region and copy the flag content there
__declspec(noinline) unsigned char* alloc_flag(char* flag, std::size_t flag_size) {
    /// Allocating memory for the flag and copying it
    /// By doing it via syscalls we can tamper the data within the instrumentation callback
    /// and nobody would even know!
    unsigned char* new_flag_ptr = nullptr;
    SIZE_T region_size = 0x1000;
    SYSCALL(NtAllocateVirtualMemory, (HANDLE)-1, &new_flag_ptr, nullptr, &region_size, MEM_COMMIT, PAGE_READWRITE);
    if (!new_flag_ptr) {
        std::puts(str_("Something went wrong... (1)\n"));
        return 0;
    }

    /// Memsetting it
    __stosb(reinterpret_cast<PBYTE>(new_flag_ptr), 0x0, region_size);

    /// Tell the instrumentation callback to tamper this data
    __asm {mov r12, COMMUNICATION_FLAG}
    __asm {mov r13, new_flag_ptr}

    /// Copying the data, hopefully everything works as it should and the
    /// instrumentation callback would tamper this data
    SIZE_T written = 0;
    auto stat = SYSCALL(NtWriteVirtualMemory, (HANDLE)-1, new_flag_ptr, flag, flag_size, &written);
    if (written != flag_size) {
        std::printf(str_("Something went wrong... (2, %d, %d, %x)\n"), written, flag_size, stat);
        return 0;
    }

    return new_flag_ptr;
}

/// \brief Entry
int main() {
    /// Ask for the flag
    char flag_value[keys::kSize + 10];
    std::puts(str_("Welcome to my little flag checker, please enter your flag"));
    std::fgets(flag_value, sizeof(flag_value) - 1, stdin);

    /// Estimating flag size
    std::size_t flag_size = 0;
    for (char* ptr = flag_value; *ptr && *ptr != '\n'; ++ptr)
        flag_size++;

    /// Min flag size is 1 byte
    if (flag_size == 0) {
        std::puts(str_("You didn't enter your flag, didn't you?"));
        return 1;
    }

    /// Encrypting the initial flag value for the communications
    for (std::size_t i = 0; i < flag_size; ++i)
        flag_value[i] ^= COMMUNICATION_ENC_KEY;

    /// It kinda sucks that we can't just inline all the syscalls here because ida is unable
    /// to process them properly, oh well.
    auto* new_flag_ptr = alloc_flag(flag_value, flag_size);

    /// And finally, verifying the flag
    if (check_flag(new_flag_ptr, flag_size)) {
        std::puts(str_("Correct!"));
    } else {
        std::puts(str_("Incorrect!"));
    }

    /// We are done
    ::exit(0);
    return 0;
}

// Just to be sure
static_assert(sizeof(void*) == 8, "please change the architecture to x64");
