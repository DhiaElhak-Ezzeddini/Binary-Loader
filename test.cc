#include <bfd.h>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "loader.h"  // Adjust path as necessary

// Assuming Binary, Section, and Symbol are defined in loader.h


int Print_Section_contents (asection *sec , bfd *sbfd){
    size_t size = bfd_section_size(sec) ;
    std::vector<uint8_t> buffer(size) ;
    if(bfd_get_section_contents(sbfd,sec,buffer.data(),0,size) != size){
        printf("Can't load the section's content") ;
        return 1 ;
    }
    printf("content of the desired section :\n") ;
    for(size_t i=0 ; i<buffer.size() ; i++){
        if(i%16==0 && i!=0){
            printf("\n") ;
        }
        printf("%02x" , buffer[i]) ;

    }
    printf("\n") ;
    return 0 ;
}

int main(int argc, char *argv[]) {
    size_t i;
    Binary bin;
    Section *sec;
    Symbol *sym;
    std::string fname;
    const char *sec_name;
    std::string section_name = argv[2] ;
    sec_name = section_name.c_str() ;

    if (argc < 2) {
        printf("Usage: %s <binary>  <section's name>\n", argv[0]);
        return 1;
    }

    fname.assign(argv[1]);


    // Load the binary file
    if (load_binary(fname, &bin, Binary::BIN_TYPE_AUTO, sec_name)<0) {
        return 1;
    }

    printf("Loaded binary '%s' %s/%s (%u bits) entry@0x%016jx\n",
           bin.filename.c_str(),
           bin.type_str.c_str(), 
           bin.archstr.c_str(),
           bin.bits, 
           bin.entry);

    // Print section information
    for (i = 0; i < bin.sections.size(); i++) {
        sec = &bin.sections[i]; 
	printf(" 0x%016jx %8ju %20s %s\n",
               sec->vma, 
               sec->size, 
               sec->name.c_str(),
               sec->type == Section::SEC_TYPE_CODE ? "CODE" : "DATA");
    }

    // Print symbol information if available
    if (bin.symbols.size() > 0) {
        printf("Scanned symbol tables:\n");
        for (i = 0; i < bin.symbols.size(); i++) {
            sym = &bin.symbols[i];
            printf(" %40s 0x%016jx %s\n",
                   sym->name.c_str(), 
                   sym->addr,
                   (sym->type & Symbol::SYM_TYPE_FUNC) ? "FUNC" : "");
        }
    }

    // Unload the binary
    unload_binary(&bin);
    return 0;
}

