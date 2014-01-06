
    .text
    .global _ResetHandler

_ResetHandler:
    movw    sp, #__main_thread_stack_end__

    call    !!___early_init

; copy initialized .data
    ;; we're copying from 00:[_romdatastart] to 0F:[_datastart]
    ;; and our data is not in the mirrored area.
    mov     es, #0
    sel     rb0
    movw    hl, #_data
    movw    de, #_textdata
    sel     rb1
    movw    ax, #_edata
    subw    ax, #_data
1:
    cmpw    ax, #0
    bz      $1f
    decw    ax
    sel     rb0
    mov     a, es:[de]
    mov     [hl], a
    incw    de
    incw    hl
    sel     rb1
    br      $1b
1:
    sel     rb0

; clear .bss section
    movw    hl, #_bss_start
    movw    ax, #0
    sel     rb1
    movw    ax, #_bss_end
    subw    ax, #_bss_start
1:
    cmpw    ax, #0
    bz      $1f
    decw    ax
    sel     rb0
    mov     [hl], a
    incw    hl
    sel     rb1
    br      $1b
1:
    sel     rb0
    ei
    call    !!_main

1:
    br      $1b
