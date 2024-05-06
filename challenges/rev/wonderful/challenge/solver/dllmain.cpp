#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <optional>
#include "mh/minhook.h"

#if defined(NDEBUG)
#define DEBUG_LOG(...)
#else
#define DEBUG_LOG(...) printf(__VA_ARGS__)
#endif


namespace {
	char flag_value[128] = { 0 };
	std::uintptr_t g_base = 0;
	std::uintptr_t g_patch_addr = 0;

	enum class operation_t : std::uint8_t {
		ADD = 0x01,
		SUB = 0x29,
		XOR = 0x31,
	};

	std::optional<std::uint64_t> get_imm(std::uint8_t* region) {
		switch (*reinterpret_cast<std::uint16_t*>(region)) {
		case 0xBA48: // mov imm64
			return *reinterpret_cast<std::uint64_t*>(&region[2]);
		case 0x4C65: // peb check
			return 1ULL << 4;
		case 0x3148: // kusd check
			return 1ULL << 4;
		default:
			return std::nullopt;
		}
	}

	std::int8_t* find_flag_start(std::int8_t* ptr) {
		for (;;) {
			if (ptr[0] != 'c' || ptr[1] != 'r' || ptr[2] != '3') {
				ptr -= 1;
				continue;
			}

			return ptr;
		}

		std::unreachable();
	}

	void handle_vmexit(std::uint8_t* ptr, std::int8_t* char_ptr) {
		ptr += 9; // skip mov cl byte ptr [rcx]; and rcx, 0xFF

		const auto imm = get_imm(ptr);

		// other themida trash
		if (!imm.has_value()) {
			DEBUG_LOG("*** imm not found for %p\n", ptr);
			return;
		}

		DEBUG_LOG("*** vmexit to %p\n", ptr);

		// skip `mov rax, rcx` and all the shit before this insn
		for (std::size_t j = 0; true; ++j) {
			if (ptr[j] != 0x48 || ptr[j + 1] != 0x89 || ptr[j + 2] != 0xC8) {
				continue;
			}

			ptr += j + 3;
			break;
		}

		std::uint64_t expected_value = *reinterpret_cast<std::uint64_t*>(&ptr[5]);

		DEBUG_LOG("*** char_ptr: %p\n", char_ptr);
		size_t index = char_ptr - find_flag_start(char_ptr);
		DEBUG_LOG("*** expr: ");

		switch (static_cast<operation_t>(ptr[1])) {
		case operation_t::ADD:
			flag_value[index] = static_cast<char>(expected_value - *imm);
			DEBUG_LOG("s.add(flag[%lld] + 0x%llx == 0x%llx)\n", index, *imm, expected_value);
			break;
		case operation_t::XOR:
			flag_value[index] = static_cast<char>(expected_value ^ *imm);
			DEBUG_LOG("s.add(flag[%lld] ^ 0x%llx == 0x%llx)\n", index, *imm, expected_value);
			break;
		case operation_t::SUB:
			flag_value[index] = static_cast<char>(expected_value + *imm);
			DEBUG_LOG("s.add(flag[%lld] - 0x%llx == 0x%llx)\n", index, *imm, expected_value);
			break;
		default:
			printf("*** unknown operation!\n");
			break;
		}

		printf("flag: %s\n", flag_value);
	}

	void* orig_rtl_dispatch_exc = nullptr;
	void rtl_dispatch_exc(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT Context) {
		if (ExceptionRecord->ExceptionCode != EXCEPTION_BREAKPOINT && ExceptionRecord->ExceptionCode != EXCEPTION_SINGLE_STEP) {
			return reinterpret_cast<decltype(&rtl_dispatch_exc)>(orig_rtl_dispatch_exc)(ExceptionRecord, Context);
		}

		if (ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
			handle_vmexit(reinterpret_cast<std::uint8_t*>(Context->Rip), reinterpret_cast<std::int8_t*>(Context->Rcx));
			*reinterpret_cast<std::uint8_t*>(g_patch_addr) = 0xCC;
			return;
		}

		DEBUG_LOG("*** caught breakpoint at %p vs %p\n", ExceptionRecord->ExceptionAddress, g_patch_addr);
		*reinterpret_cast<std::uint8_t*>(ExceptionRecord->ExceptionAddress) = 0xC2;
		Context->EFlags |= 0x100;
	}

}

BOOL __stdcall DllMain(void*, std::uint32_t call_reason, void*) {
	if (call_reason != DLL_PROCESS_ATTACH) {
		return 1;
	}

	g_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(NULL));
	g_patch_addr = g_base + 0x1DA8E5;

	MH_Initialize();
	MH_CreateHook((void*)(uintptr_t(GetModuleHandleA("ntdll")) + 0x1E1E0), rtl_dispatch_exc, (void**)&orig_rtl_dispatch_exc); // RtlDispatchException
	MH_EnableHook(nullptr);

	*reinterpret_cast<std::uint8_t*>(g_patch_addr) = 0xCC;

	return 1;
}
