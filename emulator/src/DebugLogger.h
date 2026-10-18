#pragma once

#include <cstdio>
#include <cstdint>

class DebugLogger
{
public:
    static DebugLogger& instance()
    {
        static DebugLogger logger;
        return logger;
    }

    void open(const char* filepath = "cpu_execution.log")
    {
        if (!file)
        {
            file = std::fopen(filepath, "w");
        }
    }

    void close()
    {
        if (file)
        {
            std::fclose(file);
            file = nullptr;
        }
    }

    // Log CPU register state, current opcode, and timer peripherals
    inline void log_step(uint16_t pc, uint8_t opcode, uint16_t af, uint16_t bc,
                         uint16_t de, uint16_t hl, uint16_t sp, uint8_t div,
                         uint8_t tima, uint8_t tma, uint8_t tac, uint8_t iflags,
                         uint8_t ienable, bool ime, uint32_t ticks)
    {
        if (!file) return;

        // Format: PC [OP] A F B C D E H L SP | DIV TIMA TMA TAC | IF IE IME | TICKS
        std::fprintf(file,
            "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X (%02X) "
            "| DIV:%02X TIMA:%02X TMA:%02X TAC:%02X | IF:%02X IE:%02X IME:%d | T:%u\n",
            (af >> 8) & 0xFF, af & 0xFF,
            (bc >> 8) & 0xFF, bc & 0xFF,
            (de >> 8) & 0xFF, de & 0xFF,
            (hl >> 8) & 0xFF, hl & 0xFF,
            sp, pc, opcode,
            div, tima, tma, tac,
            iflags, ienable, ime ? 1 : 0,
            ticks
        );
    }

    void flush()
    {
        if (file) std::fflush(file);
    }

private:
    DebugLogger() : file(nullptr) {}
    ~DebugLogger() { close(); }

    DebugLogger(const DebugLogger&) = delete;
    DebugLogger& operator=(const DebugLogger&) = delete;

    std::FILE* file;
};