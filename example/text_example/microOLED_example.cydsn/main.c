/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "OLED_SSD1306_1_SSD1306_API.h"

int main(void)
{
    struct OLED_SSD1306_1_OLED_Configuration my_config; 
    CyGlobalIntEnable; /* Enable global interrupts. */
    OLED_SSD1306_1_OLED_Start(); 
    OLED_SSD1306_1_OLED_create_default_config(&my_config); 
    OLED_SSD1306_1_OLED_mode_select(command_mode); 
    OLED_SSD1306_1_OLED_toggle_Reset(); 
    OLED_SSD1306_1_OLED_config(&my_config);
    OLED_SSD1306_1_OLED_set_addr_mode(0x02);
    OLED_SSD1306_1_OLED_clear_display(); 
    OLED_SSD1306_1_OLED_write_characters("Hi", 2, 2, 5); 
    //OLED_SSD1306_1_OLED_shutdown(); 

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
