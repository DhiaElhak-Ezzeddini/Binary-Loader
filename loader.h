#ifndef LOADER_H
#define LOADER_H

#include <stdint.h>
#include <string>
#include <vector>

class Binary;
class Section;
class Symbol;

class Symbol {
public:
    enum Symbol_type {
        SYM_TYPE_UKN = 0,
        SYM_TYPE_FUNC = 1
    };

    Symbol() : type(SYM_TYPE_UKN), name(), addr(0) {}

    Symbol_type type;
    std::string name;
    uint64_t addr;
};

class Section {
public:
    enum Section_type {
        SEC_TYPE_NONE = 0,
        SEC_TYPE_CODE = 1,
        SEC_TYPE_DATA = 2
    };

    Section() : binary(NULL), type(SEC_TYPE_NONE), vma(0), size(0), bytes(NULL) {}

    bool contains(uint64_t addr) {
        return (addr >= vma) && ((addr - vma) < size);
    }

    Binary *binary;
    Section_type type;
    uint64_t vma;
    uint64_t size;
    std::string name;
    uint8_t *bytes;
};

class Binary {
public:
    enum Binary_type {
        BIN_TYPE_AUTO = 0,
        BIN_TYPE_ELF = 1,
        BIN_TYPE_PE = 2
    };

    enum Bin_arch {
        ARCH_NONE = 0,
        ARCH_X86 = 1
    };

    Binary() : type(BIN_TYPE_AUTO), arch(ARCH_NONE), bits(0), entry(0) {}

    Section* get_text_section() {
        for (auto &s : sections) {
            if (s.name == ".text") return &s;
        }
        return NULL;
    }

    std::string filename;
    Binary_type type;
    std::string type_str;
    Bin_arch arch;
    std::string archstr;
    unsigned bits;
    uint64_t entry;
    std::vector<Section> sections;
    std::vector<Symbol> symbols;
};

int load_binary(std::string &fname, Binary *bin, Binary::Binary_type type , const char *sec_name);
void unload_binary(Binary *bin);

#endif /* LOADER_H */

