#include "save/save.h"

#include <cstdio>
#include <cstddef>

namespace
{
    const uint32 kSaveMagic = 0x52465331u; /* "RFS1" */
    const uint32 kSaveVersion = 1u;

    bool write_bytes(std::FILE *file, const unsigned char *data, size_t size)
    {
        return std::fwrite(data, 1u, size, file) == size;
    }

    bool read_bytes(std::FILE *file, unsigned char *data, size_t size)
    {
        return std::fread(data, 1u, size, file) == size;
    }

    void encode_u32(uint32 value, unsigned char *out)
    {
        out[0] = static_cast<unsigned char>(value & 0xFFu);
        out[1] = static_cast<unsigned char>((value >> 8) & 0xFFu);
        out[2] = static_cast<unsigned char>((value >> 16) & 0xFFu);
        out[3] = static_cast<unsigned char>((value >> 24) & 0xFFu);
    }

    void encode_u64(uint64 value, unsigned char *out)
    {
        encode_u32(static_cast<uint32>(value & 0xFFFFFFFFULL), out);
        encode_u32(static_cast<uint32>(value >> 32), out + 4);
    }

    uint32 decode_u32(const unsigned char *data)
    {
        return static_cast<uint32>(data[0]) |
               (static_cast<uint32>(data[1]) << 8) |
               (static_cast<uint32>(data[2]) << 16) |
               (static_cast<uint32>(data[3]) << 24);
    }

    uint64 decode_u64(const unsigned char *data)
    {
        uint32 lo = decode_u32(data);
        uint32 hi = decode_u32(data + 4);
        return static_cast<uint64>(lo) | (static_cast<uint64>(hi) << 32);
    }

    bool write_u32(std::FILE *file, uint32 value)
    {
        unsigned char buffer[4];
        encode_u32(value, buffer);
        return write_bytes(file, buffer, sizeof(buffer));
    }

    bool write_u64(std::FILE *file, uint64 value)
    {
        unsigned char buffer[8];
        encode_u64(value, buffer);
        return write_bytes(file, buffer, sizeof(buffer));
    }

    bool read_u32(std::FILE *file, uint32 &value)
    {
        unsigned char buffer[4];
        if (!read_bytes(file, buffer, sizeof(buffer)))
        {
            return false;
        }
        value = decode_u32(buffer);
        return true;
    }

    bool read_u64(std::FILE *file, uint64 &value)
    {
        unsigned char buffer[8];
        if (!read_bytes(file, buffer, sizeof(buffer)))
        {
            return false;
        }
        value = decode_u64(buffer);
        return true;
    }
}

bool save_core_state(const CoreState &state, const char *path)
{
    if (path == 0)
    {
        return false;
    }

    std::FILE *file = std::fopen(path, "wb");
    if (file == 0)
    {
        return false;
    }

    bool ok = true;
    ok = ok && write_u32(file, kSaveMagic);
    ok = ok && write_u32(file, kSaveVersion);
    ok = ok && write_u64(file, state.tick);
    ok = ok && write_u64(file, state.rng.state);

    if (std::fclose(file) != 0)
    {
        ok = false;
    }

    return ok;
}

bool load_core_state(CoreState &state, const char *path)
{
    if (path == 0)
    {
        return false;
    }

    std::FILE *file = std::fopen(path, "rb");
    if (file == 0)
    {
        return false;
    }

    uint32 magic = 0u;
    uint32 version = 0u;
    uint64 tick = 0u;
    uint64 rng_state = 0u;

    bool ok = true;
    ok = ok && read_u32(file, magic);
    ok = ok && read_u32(file, version);
    ok = ok && read_u64(file, tick);
    ok = ok && read_u64(file, rng_state);

    if (std::fclose(file) != 0)
    {
        ok = false;
    }

    if (!ok)
    {
        return false;
    }

    if (magic != kSaveMagic || version != kSaveVersion)
    {
        return false;
    }

    state.tick = tick;
    state.rng.state = rng_state;
    return true;
}
