/* Interface for reading / writing to memory or other components */

typedef uint8 (*read_byte_function)(uint16 address);
typedef void (*write_byte_function)(uint16 address, uint8 byte);
typedef uint16 (*read_word_function)(uint16 address);
typedef void (*write_word_function)(uint16 address, uint16 word);

/* Basic interface for a memory manager */

struct memory_handler_struct {
    read_byte_function read_byte_func;
    write_byte_function write_byte_func;
    read_word_function read_word_func;
    write_word_function write_word_func;
};

enum memory_handler_type {
    MT_RAM,
    MT_SAM
};

struct memory_range_handler_struct {
    uint16 low_addr;
    uint16 high_addr;
    enum memory_handler_type et;
};

/* Read / write functions, vary dependent on component or chip being
   referenced */

uint8 basic_read_byte_from_memory(uint16 address);
void basic_write_byte_to_memory(uint16 address, uint8 byte);
uint16 basic_read_word_from_memory(uint16 address);
void basic_write_word_to_memory(uint16 address, uint16 word);

/* Memory handler routine for SAM managed flags */
uint8 sam_read_byte_from_memory(uint16 address);
void sam_write_byte_to_memory(uint16 address, uint8 byte);

/* Memory handler routine specific to the coco architecture */
uint8 coco_read_byte_from_memory(uint16 address);
void coco_write_byte_to_memory(uint16 address, uint8 byte);
uint16 coco_read_word_from_memory(uint16 address);
void coco_write_word_to_memory(uint16 address, uint16 word);
