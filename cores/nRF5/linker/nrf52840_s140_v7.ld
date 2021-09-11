/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

/* Total size of device FLASH in bytes */
TOTAL_FLASH_SIZE = 0x100000;

/* Total size of device RAM in bytes */
TOTAL_RAM_SIZE = 0x40000;

/* Size of an individual FLASH page in bytes */
FLASH_PAGE_SIZE = 4096;

/* Number of FLASH pages reserved for Nordic FDS.  NOTE: This MUST correspond
 * to the value specified for FDS_VIRTUAL_PAGES in app_config.h */
FDS_FLASH_PAGES = 2;

/* Number of FLASH pages reserved for OpenThread data storage. */
OT_DATA_FLASH_PAGES = 4;

BOOTLOADER_CONFIG_SIZE = 2048;
MBR_PARAMS_PAGE_SIZE = 1024;
BOOTLOADER_SETTINGS_SIZE = 1024;


MEMORY
{
    /* FLASH region occupied by the Nordic SoftDevice */
    SD_FLASH (rx) : ORIGIN = 0, LENGTH = 0x27000

    BOOTLOADER_CONFIG (r): ORIGIN = TOTAL_FLASH_SIZE - BOOTLOADER_CONFIG_SIZE, LENGTH = BOOTLOADER_CONFIG_SIZE

    /** Location of mbr params page in flash. */
    MBR_PARAMS_PAGE (rw) : ORIGIN = ORIGIN(BOOTLOADER_CONFIG) - MBR_PARAMS_PAGE_SIZE, LENGTH = MBR_PARAMS_PAGE_SIZE
  
    /** Location of bootloader setting in flash. */
    BOOTLOADER_SETTINGS (rw) : ORIGIN = ORIGIN(MBR_PARAMS_PAGE) - BOOTLOADER_SETTINGS_SIZE, LENGTH = BOOTLOADER_SETTINGS_SIZE

    /* FLASH region used for Nordic FDS value storage. */
    FDS_FLASH (rw) : ORIGIN = ORIGIN(BOOTLOADER_SETTINGS) - (FLASH_PAGE_SIZE * FDS_FLASH_PAGES), LENGTH = (FLASH_PAGE_SIZE * FDS_FLASH_PAGES)

    /* FLASH region used for OpenThread data storage. */ 
    ot_flash_data(rw) : ORIGIN = ORIGIN(FDS_FLASH) - (FLASH_PAGE_SIZE * OT_DATA_FLASH_PAGES), LENGTH = (FLASH_PAGE_SIZE * OT_DATA_FLASH_PAGES)
    
    /* FLASH region used for application code and read-only data. */
    FLASH (rx) : ORIGIN = ORIGIN(SD_FLASH) + LENGTH(SD_FLASH), LENGTH = ORIGIN(ot_flash_data) - ORIGIN(FLASH)

    /* RAM region used by the Nordic SoftDevice */
    SD_RAM (rw) : ORIGIN = 0x20000000, LENGTH = 0x5968
    RAM (rwx)   : ORIGIN = 0x20005968, LENGTH = 0x3a698
}

SECTIONS
{
  . = ALIGN(4);
  .ot_flash_data :
  {
    PROVIDE(__start_ot_flash_data = ORIGIN(ot_flash_data));
    KEEP(*(SORT(.ot_flash_data*)))
    PROVIDE(__stop_ot_flash_data = ORIGIN(ot_flash_data) + LENGTH(ot_flash_data));
  } > ot_flash_data
}

SECTIONS
{
  . = ALIGN(4);
  .mem_section_dummy_ram :
  {
  }
  .cli_sorted_cmd_ptrs :
  {
    PROVIDE(__start_cli_sorted_cmd_ptrs = .);
    KEEP(*(.cli_sorted_cmd_ptrs))
    PROVIDE(__stop_cli_sorted_cmd_ptrs = .);
  } > RAM
  .log_dynamic_data :
  {
    PROVIDE(__start_log_dynamic_data = .);
    KEEP(*(SORT(.log_dynamic_data*)))
    PROVIDE(__stop_log_dynamic_data = .);
  } > RAM
  .log_filter_data :
  {
    PROVIDE(__start_log_filter_data = .);
    KEEP(*(SORT(.log_filter_data*)))
    PROVIDE(__stop_log_filter_data = .);
  } > RAM
  .svc_data :
  {
    PROVIDE(__start_svc_data = .);
    KEEP(*(.svc_data))
    PROVIDE(__stop_svc_data = .);
  } > RAM
  
  .fs_data :
  {
    PROVIDE(__start_fs_data = .);
    KEEP(*(.fs_data))
    PROVIDE(__stop_fs_data = .);
  } > RAM
} INSERT AFTER .data;

SECTIONS
{
  .mem_section_dummy_rom :
  {
  }
  .sdh_ble_observers :
  {
    PROVIDE(__start_sdh_ble_observers = .);
    KEEP(*(SORT(.sdh_ble_observers*)))
    PROVIDE(__stop_sdh_ble_observers = .);
  } > FLASH
  .sdh_soc_observers :
  {
    PROVIDE(__start_sdh_soc_observers = .);
    KEEP(*(SORT(.sdh_soc_observers*)))
    PROVIDE(__stop_sdh_soc_observers = .);
  } > FLASH
  .sdh_stack_observers :
  {
    PROVIDE(__start_sdh_stack_observers = .);
    KEEP(*(SORT(.sdh_stack_observers*)))
    PROVIDE(__stop_sdh_stack_observers = .);
  } > FLASH
  .sdh_req_observers :
  {
    PROVIDE(__start_sdh_req_observers = .);
    KEEP(*(SORT(.sdh_req_observers*)))
    PROVIDE(__stop_sdh_req_observers = .);
  } > FLASH
  .sdh_state_observers :
  {
    PROVIDE(__start_sdh_state_observers = .);
    KEEP(*(SORT(.sdh_state_observers*)))
    PROVIDE(__stop_sdh_state_observers = .);
  } > FLASH
    .nrf_queue :
  {
    PROVIDE(__start_nrf_queue = .);
    KEEP(*(.nrf_queue))
    PROVIDE(__stop_nrf_queue = .);
  } > FLASH
    .nrf_balloc :
  {
    PROVIDE(__start_nrf_balloc = .);
    KEEP(*(.nrf_balloc))
    PROVIDE(__stop_nrf_balloc = .);
  } > FLASH
    .cli_command :
  {
    PROVIDE(__start_cli_command = .);
    KEEP(*(.cli_command))
    PROVIDE(__stop_cli_command = .);
  } > FLASH
  .crypto_data :
  {
    PROVIDE(__start_crypto_data = .);
    KEEP(*(SORT(.crypto_data*)))
    PROVIDE(__stop_crypto_data = .);
  } > FLASH
  .pwr_mgmt_data :
  {
    PROVIDE(__start_pwr_mgmt_data = .);
    KEEP(*(SORT(.pwr_mgmt_data*)))
    PROVIDE(__stop_pwr_mgmt_data = .);
  } > FLASH
  .log_const_data :
  {
    PROVIDE(__start_log_const_data = .);
    KEEP(*(SORT(.log_const_data*)))
    PROVIDE(__stop_log_const_data = .);
  } > FLASH
  .log_backends :
  {
    PROVIDE(__start_log_backends = .);
    KEEP(*(SORT(.log_backends*)))
    PROVIDE(__stop_log_backends = .);
  } > FLASH

} INSERT AFTER .text

ASSERT(__start_ot_flash_data > __etext, "Section ot_flash_data overlaps text section")

INCLUDE "nrf52_common.ld"
