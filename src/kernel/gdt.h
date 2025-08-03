#ifndef GDT_H
#define GDT_H
#include "types.h"

class global_descriptor_table
{
    public:
        class segment_descriptor
        {
            private:
                uint16_t limit_lo;
                uint16_t base_lo;
                uint8_t base_hi;
                uint8_t type;
                uint8_t flags_limit_hi;
                uint8_t base_vhi;
            public:
                segment_descriptor(uint32_t base, uint32_t limit, uint8_t type);
                uint32_t Base();
                uint32_t Limit();
        } __attribute__((packed));

        segment_descriptor null_segment_selector;
        segment_descriptor unused_segment_selector;
        segment_descriptor code_segment_selector_;
        segment_descriptor data_segment_selector_;
    public:
        global_descriptor_table();
        ~global_descriptor_table();

        uint16_t code_segment_selector();
        uint16_t data_segment_selector();
};

#endif /*GDT_H*/
