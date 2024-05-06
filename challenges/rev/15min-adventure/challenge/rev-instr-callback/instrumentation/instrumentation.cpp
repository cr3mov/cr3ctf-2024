#include "instrumentation.hpp"
#include "util/defines.hpp"
#include "util/syscalls.hpp"

#include "keys.hpp"

namespace {
    /// \brief A structure that holds native execution data
    struct {
        std::uint64_t rip;
        std::uint64_t rsp;
        std::uint64_t rcx;
        std::uint64_t rdx;
    } state;

    /// \brief Instrumentation callback itself
    void callback(CONTEXT* ctx) {
        using namespace keys;

        /// Restore the execution data
        ctx->Rip = state.rip;
        ctx->Rsp = state.rsp;
        ctx->Rcx = state.rcx;

        /// Obtain the data
        volatile auto return_address = reinterpret_cast<void*>(ctx->Rip);
        volatile auto return_value = reinterpret_cast<void*>(ctx->Rax);

        /// Tamper the data if r12 is set to the communication flag
        if (ctx->R12 == COMMUNICATION_FLAG && ctx->R13) {
            auto* flag_ptr = reinterpret_cast<unsigned char*>(ctx->R13);

            /// We only want to tamper the data that looks like a flag
            if (flag_ptr[0] == kFlagEncryptedStart_1 && flag_ptr[1] == kFlagEncryptedStart_2 && flag_ptr[2] == kFlagEncryptedStart_3 &&
                flag_ptr[3] == kFlagEncryptedStart_4 && (flag_ptr[4] == kFlagEncryptedStart_5 || flag_ptr[4] == kFlagEncryptedStart_5_1)) {
                auto keys2 = keys::get_keys2();

                for (std::size_t i = 0; flag_ptr[i]; ++i)
                    flag_ptr[i] = _rotl8(flag_ptr[i] ^ (keys2[i] + i) ^ i, 2);
            }
        }

        /// Restore the execution context
        RtlRestoreContext(ctx, nullptr);
    }

    /// Import the RtlCaptureContext
    /// \note es3n1n: We cannot call `RtlCaptureContext` directly, because it would contain the ptr to ptr.
    /// We don't want to deref anything and add some other instructions that could potentially
    /// Use the important registers and thus break something
    extern "C" void* __imp_RtlCaptureContext;

    /// \note es3n1n: Slightly modified version of estk's bridge by yours truly
    __declspec(noinline) __attribute__((naked)) inline void bridge() {
        __asm {
            ; Store the execution data
            mov qword ptr[state.rsp], rsp
            mov qword ptr[state.rip], r10
            mov qword ptr[state.rcx], rcx
            mov qword ptr[state.rdx], rdx

            ; Allocate CONTEXT structure data
            sub rsp, 0x4D0
            and rsp, -10h ; Align

            ; Capture the thread context
            mov rcx, rsp 
            call __imp_RtlCaptureContext

            ; Invoke instrumentation callback
            sub rsp, 20h ; Shadow stack space
            call callback
        }
    }
} // namespace

void instrumentation::startup() {
    /// Setup the instrumentation callback info struct
    struct {
        uint32_t version = 0;
        uint32_t reserved = 0;
        void (*callback)() = bridge;
    } info;

    /// Syscall the NtSetInformationProcess for the current process (HANDLE -1),
    /// with ProcessClass set to ProcessInstrumentationCallback
    constexpr auto kProcessInstrumentationCallback = 0x28;
    SYSCALL(NtSetInformationProcess, (HANDLE)-1, kProcessInstrumentationCallback, &info, sizeof(info));
}